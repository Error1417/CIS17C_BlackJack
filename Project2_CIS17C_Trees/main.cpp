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
 * 
 * 
 * Created on December 10 , 2022, 2:27 PM
 */

#include <iostream>
#include <string>
#include "BlackJack.h"

using namespace std;

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
    cout << "Round Outcomes:" << endl;
    inOrderTraversal(root);

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