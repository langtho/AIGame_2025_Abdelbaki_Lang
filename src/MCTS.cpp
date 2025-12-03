//
// Created by loko on 02/12/2025.
//

#include "../include/MCTS.h"

#include <complex>
#include <limits>
#include <algorithm>
#include "GameRules.h"
using namespace std;

MCTS::MCTS(int iterations) : max_iterations(iterations) {
    srand(time(nullptr));
}

MCTS::~MCTS() {
    if (root != nullptr) {
        delete root;
    }
}

void MCTS::initialize(const State &startState) {
    if (root!=nullptr) {
        delete root;
    }

    root =new Node(startState,nullptr,{-1,red});
    root->unused_moves = GameRules::getPossibleMoves(startState);
}

void MCTS::advance_tree(pair<int, Color> move, const State &newState) {
    if (root==nullptr) {
        initialize(newState);
        return;
    }

    Node* nextRoot =nullptr;
    for (Node* child: root->children) {
        if (child->move_from_parent.first == move.first && child->move_from_parent.second == move.second) {
            nextRoot =child;
            break;
        }
    }

    if (nextRoot != nullptr) {
        auto it = find(root->children.begin(),root->children.end(),nextRoot);
        if (it != root->children.end()) {
            root->children.erase(it);
        }
        delete root;
        root= nextRoot;
        root->parent = nullptr;
    }else {
        delete root;
        root = new Node(newState,nullptr,{-1,red});
        root->unused_moves = GameRules::getPossibleMoves(newState);
    }
}


pair<int, Color> MCTS::find_best_move(const State &state) {
    if (root == nullptr) {
        cerr << "Error: MCTS not initialized!" << endl;
        return {-1, red};
    }

    for (int i = 0; i < max_iterations; ++i) {
        Node* node = select(root);

        if (!GameRules::gameOver(node->state) && !node->unused_moves.empty()) {
            node = expand(node);
        }

        int result = simulate(node->state);
        backpropagate(node, result);
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
    while (node->unused_moves.empty()&&!node->children.empty()) {
        double best_utc = -numeric_limits<double>::max();
        Node* best_child = nullptr;

        for (Node* child: node->children) {
            double uct = uct_value(child);
            if (uct>best_utc) {
                best_utc =uct;
                best_child = child;
            }
        }
        node = best_child;
    }
    return node;
}

Node* MCTS::expand(Node* node) {
    int index = rand() % node->unused_moves.size();
    pair<int,Color> move = node->unused_moves[index];

    node->unused_moves.erase(node->unused_moves.begin()+index);
    State next_state = GameRules::playMove(node->state,move.first,move.second);

    Node* child= new Node(next_state,node,move);
    child->unused_moves = GameRules::getPossibleMoves(next_state);

    node->children.push_back(child);
    return child;
}

int MCTS::simulate(State state) {
    int depth = 0;
    while (!GameRules::gameOver(state)) {
        vector<pair<int,Color>> moves = GameRules::getPossibleMoves(state);

        if (moves.empty()) {
            break;
        }

        int idx =rand()%moves.size();
        state = GameRules::playMove(state,moves[idx].first,moves[idx].second);

        depth++;
        if (depth>200) break;
    }

    if (state.score_p1 > state.score_p2) return 1;
    if (state.score_p2 > state.score_p1) return -1;
    return 0;
}

void MCTS::backpropagate(Node* node, int result) {
    while (node != nullptr) {
        node->visits++;

        if (node->previous_player) {
            if (result==1) node->wins += 1;
            else if (result ==0) node->wins += 0.5;
        }else {
            if (result==-1) node->wins += 1;
            else if (result ==0) node->wins += 0.5;
        }
        node = node->parent;
    }
}

double MCTS::uct_value(Node *node) {
    if (node->visits == 0) return numeric_limits<double>::max();
    double c=1.412;
    return (node->wins / (double)node->visits)+ c* sqrt(log((double)node->parent->visits)/(double)node->visits);
}