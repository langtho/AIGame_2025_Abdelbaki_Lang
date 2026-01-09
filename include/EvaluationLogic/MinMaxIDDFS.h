#ifndef AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_H
#define AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_H

#include "../State.h"
#include <unordered_map>
#include <chrono>
#include <vector>
#include <atomic>

// Entry for the Transposition Table (Cache)
struct TTEntry {
    int depth;
    int value;
    int flag; // 0: Exact, 1: LowerBound, 2: UpperBound
    std::pair<int, Color> best_move;
};

class MinMaxIDDFS {
private:
    // --- Time Management ---
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    int time_limit_ms;
    bool time_out;
    std::atomic<bool>* stop_flag = nullptr; // Pointer to external flag to stop search immediately (for Pondering)

    // --- Transposition Table ---
    std::unordered_map<uint64_t, TTEntry> tt;

    // --- Heuristics ---
    std::pair<int, Color> killer_moves[100][2]; // [depth][slot] - Store 2 killer moves per depth

    // --- Zobrist Hashing (for unique board IDs) ---
    static bool z_initialized;
    static uint64_t z_red[16][100];
    static uint64_t z_blue[16][100];
    static uint64_t z_trans[16][100];
    static uint64_t z_turn;

    void initZobrist();
    uint64_t computeHash(const State& state);

    int _minmax(const State& state, int depth, int alpha, int beta, bool maximizing_player, bool original_player_is_p1);

public:
    MinMaxIDDFS(int time_ms = 1950);
    std::pair<int, Color> find_best_move(const State& state);
    void setTimeLimit(int ms) { time_limit_ms = ms; }
    void setStopFlag(std::atomic<bool>* flag) { stop_flag = flag; }
};

#endif //AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_H