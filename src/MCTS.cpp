//
// Created by loko on 02/12/2025.
// Optimisation Finale: Smart Greedy Simulation + Hybrid Solver
//

#include "../include/MCTS.h"
#include "../include/EvaluationLogic/Evaluate.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include "GameRules.h"

using namespace std;

// Initialisation de la map statique
unordered_map<string, double> MCTS::solution_cache;

MCTS::MCTS(int iterations) : max_iterations(iterations) {
    srand(time(nullptr));
}

MCTS::~MCTS() {
    if (root != nullptr) {
        delete root;
    }
}

// --- GESTION DU SAVOIR (BINAIRE RAPIDE) ---
void MCTS::saveKnowledge(const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;

    size_t size = solution_cache.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& entry : solution_cache) {
        const string& key = entry.first;
        double val = entry.second;

        size_t keyLen = key.size();
        file.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
        file.write(key.c_str(), keyLen);
        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    // cout << "Knowledge saved! Entries: " << size << endl;
    file.close();
}

void MCTS::loadKnowledge(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "No knowledge file found. Starting fresh." << endl;
        return;
    }

    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    solution_cache.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        size_t keyLen;
        file.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
        string key(keyLen, ' ');
        file.read(&key[0], keyLen);
        double val;
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        solution_cache[key] = val;
    }
    cout << "Knowledge loaded! Entries: " << count << endl;
    file.close();
}

// --- HELPER: Board to String Key ---
string boardToString(const State& s) {
    string key = "";
    key += to_string(s.score_p1) + "," + to_string(s.score_p2) + "|";
    for(int i=0; i<16; i++) {
        key += to_string(s.board.fields[i].total_seeds) + ",";
    }
    key += (s.player_playing ? "P1" : "P2");
    return key;
}

// --- MCTS CORE ---

void MCTS::initialize(const State &startState) {
    if (root != nullptr) {
        delete root;
    }
    root = new Node(startState, nullptr, {-1, red});
    root->unused_moves = GameRules::getPossibleMoves(startState);
}

void MCTS::advance_tree(pair<int, Color> move, const State &newState) {
    if (root == nullptr) {
        initialize(newState);
        return;
    }
    Node* nextRoot = nullptr;
    for (Node* child: root->children) {
        if (child->move_from_parent.first == move.first && child->move_from_parent.second == move.second) {
            nextRoot = child;
            break;
        }
    }
    if (nextRoot != nullptr) {
        for (size_t i = 0; i < root->children.size(); ++i) {
            if (root->children[i] == nextRoot) {
                root->children.erase(root->children.begin() + i);
                break;
            }
        }
        delete root;
        root = nextRoot;
        root->parent = nullptr;
    } else {
        delete root;
        root = new Node(newState, nullptr, {-1, red});
        root->unused_moves = GameRules::getPossibleMoves(newState);
    }
}

// --- FIND BEST MOVE (HYBRID LOGIC) ---
pair<int, Color> MCTS::find_best_move(const State &state) {
    int seedsOnBoard = 0;
    for(int i=0; i<16; i++) seedsOnBoard += state.board.fields[i].total_seeds;

    // --- FIX 126s LAG: SOLVER SEUIL ABAISSÉ ---
    // On n'active le Solver que si le plateau est VRAIMENT simple (< 28 graines).
    if (seedsOnBoard < 28) {
        int solver_depth;
        if (seedsOnBoard < 12)      solver_depth = 20; // God Mode
        else if (seedsOnBoard < 20) solver_depth = 10; // Très profond
        else                        solver_depth = 6;  // Rapide

        vector<pair<int, Color>> moves = GameRules::getPossibleMoves(state);
        if (moves.empty()) return {-1, red};

        double bestValue = -numeric_limits<double>::max();
        pair<int, Color> bestMove = moves[0];
        bool am_i_p1 = state.player_playing;

        for (auto& move : moves) {
            State nextState = GameRules::playMove(state, move.first, move.second);
            double val = alphaBeta(nextState, solver_depth, -numeric_limits<double>::max(), numeric_limits<double>::max(), false, am_i_p1);
            if (val > bestValue) {
                bestValue = val;
                bestMove = move;
            }
        }
        return bestMove;
    }

    // --- STANDARD MCTS ---
    if (root == nullptr) initialize(state);
    if (root->state.score_p1 != state.score_p1 || root->state.moves_played != state.moves_played) {
        initialize(state);
    }

    for (int i = 0; i < max_iterations; ++i) {
        Node* node = select(root);
        if (!GameRules::gameOver(node->state) && !node->unused_moves.empty()) {
            node = expand(node);
        }
        int result = simulate(node->state);
        backpropagate(node, result);
    }

    if (root->children.empty()) {
        auto moves = GameRules::getPossibleMoves(state);
        if (!moves.empty()) return moves[0];
        return {-1, red};
    }

    pair<int, Color> best_move = {-1, red};
    int max_visits = -1;
    for (Node* child : root->children) {
        if (child->visits > max_visits) {
            max_visits = child->visits;
            best_move = child->move_from_parent;
        }
    }
    return best_move;
}

Node *MCTS::select(Node *node) {
    while (node->unused_moves.empty() && !node->children.empty()) {
        double best_uct = -numeric_limits<double>::max();
        Node* best_child = nullptr;
        for (Node* child: node->children) {
            double uct = uct_value(child);
            if (uct > best_uct) {
                best_uct = uct;
                best_child = child;
            }
        }
        if (best_child == nullptr) break;
        node = best_child;
    }
    return node;
}

Node* MCTS::expand(Node* node) {
    int index = rand() % node->unused_moves.size();
    pair<int,Color> move = node->unused_moves[index];
    node->unused_moves.erase(node->unused_moves.begin() + index);

    State next_state = GameRules::playMove(node->state, move.first, move.second);
    Node* child = new Node(next_state, node, move);
    child->unused_moves = GameRules::getPossibleMoves(next_state);
    node->children.push_back(child);
    return child;
}

// --- SIMULATION INTELLIGENTE (SMART GREEDY) ---
int MCTS::simulate(State state) {
    int seedsOnBoard = 0;
    for(int i=0; i<16; i++) seedsOnBoard += state.board.fields[i].total_seeds;

    // Endgame définition : < 50 graines
    bool is_endgame = (seedsOnBoard < 50);
    int depth = 0;
    int max_sim_depth = is_endgame ? 500 : 20;

    Evaluate eval;

    while (!GameRules::gameOver(state) && depth < max_sim_depth) {
        vector<pair<int,Color>> possible_moves = GameRules::getPossibleMoves(state);
        if (possible_moves.empty()) break;

        pair<int, Color> selected_move = {-1, red};

        // --- SMART LOGIC ---
        // On essaie de jouer les coups qui capturent (Greedy)
        // Cela simule un adversaire compétent (comme MinMax)
        // On mélange pour éviter les biais de position
        int start_idx = rand() % possible_moves.size();

        // Probabilité d'être intelligent (90%). 10% de bruit pour la variété.
        bool use_intelligence = ((rand() % 100) < 90);

        if (use_intelligence) {
            for (int i = 0; i < possible_moves.size(); i++) {
                int idx = (start_idx + i) % possible_moves.size();
                pair<int, Color> move = possible_moves[idx];

                // Simulation rapide du coup
                State tempState = GameRules::playMove(state, move.first, move.second);

                // Gain de score immédiat ?
                int score_diff = (state.player_playing)
                                 ? (tempState.score_p1 - state.score_p1)
                                 : (tempState.score_p2 - state.score_p2);

                if (score_diff > 0) {
                    selected_move = move;
                    state = tempState; // Appliquer
                    break;
                }
            }
        }

        // Si aucun coup gagnant trouvé (ou random), on joue au hasard
        if (selected_move.first == -1) {
            int idx = rand() % possible_moves.size();
            selected_move = possible_moves[idx];
            state = GameRules::playMove(state, selected_move.first, selected_move.second);
        }

        depth++;

        // Mercy Rule (Cutoff)
        if (!is_endgame && depth > 5) {
            int diff = state.score_p1 - state.score_p2;
            if (diff > 14) return 1;
            if (diff < -14) return -1;
        }
    }

    if (GameRules::gameOver(state)) {
        if (state.score_p1 > state.score_p2) return 1;
        if (state.score_p2 > state.score_p1) return -1;
        return 0;
    }

    int score = eval.evaluate_stateNEW(state, true);
    if (score > 10) return 1;
    if (score < -10) return -1;
    return 0;
}

void MCTS::backpropagate(Node* node, int result) {
    while (node != nullptr) {
        node->visits++;
        bool mover_was_p1 = node->previous_player;
        if (mover_was_p1) {
            if (result == 1) node->wins += 1.0;
            else if (result == 0) node->wins += 0.5;
        } else {
            if (result == -1) node->wins += 1.0;
            else if (result == 0) node->wins += 0.5;
        }
        node = node->parent;
    }
}

double MCTS::uct_value(Node *node) {
    if (node->visits == 0) return numeric_limits<double>::max();
    // 1.414 pour une meilleure exploration (éviter les pièges locaux)
    double c = 1.414;
    return (node->wins / (double)node->visits) + c * sqrt(log((double)node->parent->visits) / (double)node->visits);
}

// --- ALPHA BETA SOLVER (Avec Move Ordering) ---
double MCTS::alphaBeta(const State &state, int depth, double alpha, double beta, bool maximizingPlayer, bool maximizing_player_is_p1) {
    bool use_cache = (depth > 2);
    string state_key = "";
    if (use_cache) {
        state_key = boardToString(state);
        if (solution_cache.count(state_key)) return solution_cache[state_key];
    }

    if (depth == 0 || GameRules::gameOver(state)) {
        Evaluate eval;
        double val = (double)eval.evaluate_stateNEW(state, maximizing_player_is_p1);
        if (use_cache) solution_cache[state_key] = val;
        return val;
    }

    vector<pair<int, Color>> moves = GameRules::getPossibleMoves(state);
    if (moves.empty()) {
         Evaluate eval;
         double val = (double)eval.evaluate_stateNEW(state, maximizing_player_is_p1);
         if (use_cache) solution_cache[state_key] = val;
         return val;
    }

    // Move Ordering Rapide
    auto evaluateMoveSimple = [&](const pair<int, Color>& move) -> int {
        State temp = GameRules::playMove(state, move.first, move.second);
        int score_diff = maximizing_player_is_p1
                         ? (temp.score_p1 - state.score_p1)
                         : (temp.score_p2 - state.score_p2);
        return score_diff;
    };
    sort(moves.begin(), moves.end(), [&](const pair<int, Color>& a, const pair<int, Color>& b) {
        return evaluateMoveSimple(a) > evaluateMoveSimple(b);
    });

    double bestVal;
    if (maximizingPlayer) {
        double maxEval = -numeric_limits<double>::max();
        for (const auto& move : moves) {
            State next_state = GameRules::playMove(state, move.first, move.second);
            double eval = alphaBeta(next_state, depth - 1, alpha, beta, false, maximizing_player_is_p1);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
        bestVal = maxEval;
    } else {
        double minEval = numeric_limits<double>::max();
        for (const auto& move : moves) {
            State next_state = GameRules::playMove(state, move.first, move.second);
            double eval = alphaBeta(next_state, depth - 1, alpha, beta, true, maximizing_player_is_p1);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        bestVal = minEval;
    }

    if (use_cache) solution_cache[state_key] = bestVal;
    return bestVal;
}