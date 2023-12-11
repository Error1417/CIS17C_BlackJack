/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Ismael Perez
 * Purpose:
 * 
 * version 1: Recursions in game class line around 610, calls end round to play again
 * version 2: recursive sort in hand class line around 226, replace for old sort
 * version 3: hashing in game class line around 365, gives each round its own hash.
 * version 4: trees added in main line 27, example of how tree could have been incorperated to game
 * version 5: graphs in main to show player earnings, don't know if I implemented it correctly
 * 
 * Created on December 10 , 2022, 2:27 PM
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "BlackJack.h"

using namespace std;

// Player structure
struct Player {
    string name;
    int highestEarning;
};

// Edge structure
struct Edge {
    int from;  // Index of the player in the vector
    int to;    // Index of another player
    string relation; // Description of the relationship
};

// Graph structure
struct Graph {
    vector<Player> players;
    vector<Edge> edges;

    void addPlayer(const string& name, int earning) {
        players.push_back({name, earning});
    }

    void addEdge(int fromIdx, int toIdx, const string& relation) {
        if (fromIdx < players.size() && toIdx < players.size()) {
            edges.push_back({fromIdx, toIdx, relation});
        }
    }

    void displayGraph() {
        cout << endl << endl;
        cout << "Potential Blackjack Players Graph:" << endl;
        for (int i = 0; i < players.size(); i++) {
            cout << players[i].name << " (Earning: $" << players[i].highestEarning << ")" << endl;
        }

        cout << "\nRelationships:" << endl;
        for (int j = 0; j < edges.size(); j++) {
            cout << players[edges[j].from].name << " - " << edges[j].relation << " - " << players[edges[j].to].name << endl;
        }
    }
};

// TreeNode structure
struct TreeNode {
    int roundNumber;  // Key: Round number
    string outcome;   // Value: Outcome of the round
    TreeNode *left, *right;

    TreeNode(int round, string out) : roundNumber(round), outcome(out), left(nullptr), right(nullptr) {}
};

// Function prototypes
TreeNode* insert(TreeNode* node, int roundNumber, string outcome);
void inOrderTraversal(TreeNode* node);
void deleteTree(TreeNode* node);


int main() {
    Game g;
    Graph blackjackGraph;
    
    // Adding players
    blackjackGraph.addPlayer("Alice", 5000);
    blackjackGraph.addPlayer("Bob", 7000);
    blackjackGraph.addPlayer("Charlie", 3500);
    blackjackGraph.addPlayer("Diana", 6000);


    // Play the game
    try {
        g.play();
    }
    catch(BadSuit e) {
        cout << "*** Bad card suit is given.\n";
        exit(1);
    }
    catch(BadNumberDecks e) {
        cout << "*** Bad number of decks is given.\n";
        exit(1);
    }
    
    TreeNode* root = nullptr;

    // Example: Inserting round outcomes into the BST
    //this part would be the part that gets replaced, but I was having problems 
    //possibly variable type differences
    root = insert(root, 2, "Win");
    root = insert(root, 1, "Lose");
    root = insert(root, 3, "Tie");

    cout << endl;
    cout << "Using Trees to keep track of the Round Outcome" << endl;
    cout << "Possible Round Outcomes:" << endl;
    inOrderTraversal(root);
    
    
    // Adding relationships based on earnings or other criteria
    blackjackGraph.addEdge(0, 1, "has less earnings than");
    blackjackGraph.addEdge(1, 2, "has more earnings than");
    blackjackGraph.addEdge(3, 0, "has more earnings than");

    // Display the graph
    blackjackGraph.displayGraph();
    
    //Delete Tree
    deleteTree(root);

    return 0;
}

// Function definitions
TreeNode* insert(TreeNode* node, int roundNumber, string outcome) {
    if (node == nullptr) {
        return new TreeNode(roundNumber, outcome);
    }
    if (roundNumber < node->roundNumber) {
        node->left = insert(node->left, roundNumber, outcome);
    } else if (roundNumber > node->roundNumber) {
        node->right = insert(node->right, roundNumber, outcome);
    }
    return node;
}

void inOrderTraversal(TreeNode* node) {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        cout << "Round " << node->roundNumber << ": " << node->outcome << endl;
        inOrderTraversal(node->right);
    }
}

void deleteTree(TreeNode* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}