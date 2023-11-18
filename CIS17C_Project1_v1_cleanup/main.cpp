/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Ismael Perez
 * Purpose:
 * version 1: Clean up code
 * version 2: plan on adding lists
 * 
 *
 * 
 * Created on December, 2022, 2:27 PM
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "BlackJack.h"

using namespace std;

// Main function
int main() {
        int choice;
        //class objects
	Game g;
        Decks dks;
        Card c;
        
        //the black Jack game is here
	try {
		g.play();
	}
        //if the program some how gets a bad suit 
	catch(BadSuit e) {
		cout << "*** Bad card suit is given.\n";
		exit(1);
	}
        //if the user enters a wrong deck number
	catch(BadNumberDecks e) {
		cout << "*** Bad number of decks is given.\n";
		exit(1);
	};
	return 0;
}