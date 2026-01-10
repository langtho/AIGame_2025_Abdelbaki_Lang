#include "EvaluationLogic/MinMaxIDDFS.h"
#include "GameRules.h"
#include "EvaluationLogic/Evaluate.h"
#include <limits>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

// Static members initialization
bool MinMaxIDDFS::z_initialized = false;
uint64_t MinMaxIDDFS::z_red[16][100];
uint64_t MinMaxIDDFS::z_blue[16][100];
uint64_t MinMaxIDDFS::z_trans[16][100];
uint64_t MinMaxIDDFS::z_turn;

MinMaxIDDFS::MinMaxIDDFS(int time_ms) : time_limit_ms(time_ms), time_out(false) {
    if (!z_initialized) {
        initZobrist();
    }
    tt.reserve(1000000); // Increase capacity for Persistent TT
}

void MinMaxIDDFS::initZobrist() {
    std::mt19937_64 rng(12345);
    std::uniform_int_distribution<uint64_t> dist;

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 100; ++j) {
            z_red[i][j] = dist(rng);
            z_blue[i][j] = dist(rng);
            z_trans[i][j] = dist(rng);
        }
    }
    z_turn = dist(rng);
    z_initialized = true;
}

uint64_t MinMaxIDDFS::computeHash(const State& state) {
    uint64_t h = 0;
    if (state.player_playing) h ^= z_turn;

    for (int i = 0; i < 16; ++i) {
        int r = std::min(state.board.fields[i].red_seeds, 99);
        int b = std::min(state.board.fields[i].blue_seeds, 99);
        int t = std::min(state.board.fields[i].transparent_seeds, 99);
        
        if (r > 0) h ^= z_red[i][r];
        if (b > 0) h ^= z_blue[i][b];
        if (t > 0) h ^= z_trans[i][t];
    }
    return h;
}

int MinMaxIDDFS::_minmax(const State &state, int depth, int alpha, int beta, bool maximizing_player, bool original_player_is_p1) {
if (time_out) return 0;
    
// Check time more frequently (every 1024 nodes instead of 2048) to avoid overshooting
if ((++node_count & 1023) == 0) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() >= time_limit_ms) {
            time_out = true;
            return 0;
        }
    }

    uint64_t hash = computeHash(state);
    if (tt.find(hash) != tt.end()) {
        const TTEntry& entry = tt[hash];
        if (entry.depth >= depth) {
            if (entry.flag == 0) return entry.value;
            if (entry.flag == 1) alpha = std::max(alpha, entry.value);
            else if (entry.flag == 2) beta = std::min(beta, entry.value);
            if (beta <= alpha) return entry.value;
        }
    }

    if (depth == 0 || GameRules::gameOver(state)) {
        return Evaluate::evaluate_state(state, original_player_is_p1);
    }

    auto moves = GameRules::getPossibleMoves(state);
    if (moves.empty()) {
        return Evaluate::evaluate_state(state, original_player_is_p1);
    }

    // --- MOVE ORDERING (Lazy) ---
    std::pair<int, Color> best_move_tt = {-1, red};
    if (tt.count(hash)) best_move_tt = tt[hash].best_move;

    // Helper lambda for priority (Fast lookup, no playMove!)
    auto getPriority = [&](const std::pair<int, Color>& m) {
        if (m.first == best_move_tt.first && m.second == best_move_tt.second) return 10000000;
        if (m == killer_moves[depth][0]) return 90000;
        if (m == killer_moves[depth][1]) return 80000;
        return 0;
    };

    // Sort moves: Best priority first
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, Color>& a, const std::pair<int, Color>& b) {
        return getPriority(a) > getPriority(b);
    });

    int value = maximizing_player ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    std::pair<int, Color> best_move_this_node = moves[0];
    int original_alpha = alpha;

    for (const auto& move : moves) {
        State next_State = GameRules::playMove(state, move.first, move.second);
        int eval = _minmax(next_State, depth - 1, alpha, beta, !maximizing_player, original_player_is_p1);
        
        if (time_out) return 0;

        if (maximizing_player) {
            if (eval > value) { 
                value = eval; 
                best_move_this_node = move; 
            }
            alpha = std::max(alpha, value);
        } else {
            if (eval < value) { 
                value = eval; 
                best_move_this_node = move; 
            }
            beta = std::min(beta, value);
        }
        
        if (beta <= alpha) {
            // Beta Cutoff (Pruning)
            if (!time_out) {
                // Update Killer Moves
                if (move != killer_moves[depth][0]) {
                    killer_moves[depth][1] = killer_moves[depth][0];
                    killer_moves[depth][0] = move;
                }
            }
            break;
        }
    }

    if (!time_out) {
        TTEntry entry;
        entry.depth = depth;
        entry.value = value;
        entry.best_move = best_move_this_node;
        if (value <= original_alpha) entry.flag = 2;
        else if (value >= beta) entry.flag = 1;
        else entry.flag = 0;
        
        // Replacement Strategy: Only overwrite if the new entry is from a deeper (or equal) search
        auto it = tt.find(hash);
        if (it == tt.end() || depth >= it->second.depth) {
            tt[hash] = entry;
        }
    }

    return value;
}

std::pair<int, Color> MinMaxIDDFS::find_best_move(const State &state) {
start_time = std::chrono::steady_clock::now();
time_out = false;
node_count = 0;  // Reset node counter for new search
tt.clear();
    
    // Reset Heuristics for new search
    for(auto& d : killer_moves) { d[0] = {-1, red}; d[1] = {-1, red}; }
    
    std::pair<int, Color> best_move = {-1, red};
    int best_val = -999999; // Store value for Aspiration Windows

    auto moves = GameRules::getPossibleMoves(state);
    if (!moves.empty()) best_move = moves[0];

    for (int d = 1; d <= 30; ++d) {
        int val = _minmax(state, d, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true, state.player_playing);
        
        if (time_out) break;

        uint64_t hash = computeHash(state);
        if (tt.count(hash)) {
            best_move = tt[hash].best_move;
            best_val = val;
            if (val > 90000 || val < -90000) break; // Updated threshold for new score scaling
        }
    }

    return best_move;
}