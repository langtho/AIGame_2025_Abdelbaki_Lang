//
// Created by loko on 02/12/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_MCTS_H
#define AIGAME_2025_ABDELBAKI_LANG_MCTS_H
#include <vector>
#include "State.h"
#include "Color.h"
#include <unordered_map>
using namespace std;

struct Node {
    State state;
    Node* parent;
    vector<Node*> children;
    vector<pair<int,Color>> unused_moves;
    pair<int,Color> move_from_parent;
    bool previous_player;


    int visits;
    double wins;
    double minmaxscore;
    bool has_minmaxscore;
    Node(const State& s, Node* p, pair<int,Color> move):state(s),parent(p),move_from_parent(move),visits(0),wins(0.0) {
        previous_player = !state.player_playing;
    }

    ~Node() {
        for (Node* child: children) {
            delete child;
        }
    }
};

class MCTS {
public:
    MCTS(int iterations = 1000);
    ~MCTS();

    void initialize(const State& startState);

    pair<int,Color> find_best_move(const State& state);

    void advance_tree(pair<int,Color> move,const State& newState);
    double alphaBeta(const State &state, int depth, double alpha, double beta, bool maximizingPlayer, bool maximizing_player_is_p1) ;
    Node* find_best_child_via_minimax(Node* node, int depth);

    void saveKnowledge(const string& filename);
    void loadKnowledge(const string& filename);
private:
    int max_iterations;
    Node* root = nullptr;

    Node* select(Node* node);
    Node* expand(Node* node);
    int simulate(State state);
    void backpropagate(Node* node,int result);

    double uct_value(Node* node);
    bool is_fully_expanded(Node* node);

    static unordered_map<string, double> solution_cache;
};
#endif //AIGAME_2025_ABDELBAKI_LANG_MCTS_H