/*
 * KnightsTour.cpp
 *
 *  Created on: May 22, 2011
 *      Author: jxchen
 */

#define MAX_INPUT_LENGTH 11
#define MAX_POSSIBLE_MOVES 8

#include <iostream>
#include <algorithm>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int* findTour(int r, int c, int startR, int startC, int *board, bool *solvedPtr);
int* tourHelper(int r, int c, int x, int y, int *resultBoard, int *movesBoard, int step, bool *solvedPtr);
int* initializeBoard(int r, int c);
void updateMovesBoard(int r, int c, int x, int y, int *movesBoard, int amount);
list<pair<int,int> > loadMoves(int r, int c, int x, int y, int *movesBoard, int *resultBoard);
list<pair<int,int> > getPossibleKnightMoves(int x, int y);

int main(void) {
	int r,c,startR,startC;

	string rowColStr;
	while (true) {
		cout << "Please enter number of board dimensions in following format: r c ";
		getline(cin,rowColStr);
		if (rowColStr.size() > MAX_INPUT_LENGTH) {
			cout << "input string too long" << endl;
			continue;
		}
		char temp[rowColStr.size()+1];
		strcpy(temp,rowColStr.c_str());

		char *cur = strtok(temp, " ");
		r = atoi(cur);
		cur = strtok(NULL, " ");
		if (cur == NULL) {
			cout << "invalid board dimensions" << endl;
			continue;
		}

		c = atoi(cur);
		if (r <= 0 || c <= 0) {
			cout << "invalid board dimensions" << endl;
			continue;
		}
		break;
	}

	string startPosStr;
	while (true) {
		cout << "Please enter starting position: r c ";
		getline(cin,startPosStr);
		if (startPosStr.size() > MAX_INPUT_LENGTH) {
			cout << "input string too long" << endl;
			continue;
		}
		char temp[startPosStr.size()+1];
		strcpy(temp,startPosStr.c_str());

		char *cur = strtok(temp, " ");
		startR = atoi(cur);
		cur = strtok(NULL, " ");
		if (cur == NULL) {
			cout << "invalid start position" << endl;
			continue;
		}

		startC = atoi(cur);
		if (startR < 0 || startR >= r || startC < 0 || startC >= c) {
			cout << "invalid start position" << endl;
			continue;
		}
		break;
	}

	bool solved = false;
	int *resultBoard = findTour(r,c,startR,startC,initializeBoard(r,c),&solved);
	//int *resultBoard = findGoldenTour(r,c,startR,startC,initializeBoard(r,c));

	if (solved) {
		//print out result
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				cout << resultBoard[i*c + j] << "\t";
			}
			cout << endl;
		}
	} else {
		cout << "No solution found" << endl;
	}

	delete[] resultBoard;
}

int* findTour(int r, int c, int startR, int startC, int *movesBoard, bool *solvedPtr) {
	int *resultBoard = new int[r*c];
	for (int i = 0; i < r*c; i++) {
		resultBoard[i] = 0;
	}

	resultBoard = tourHelper(r, c, startR, startC, resultBoard, movesBoard,0, solvedPtr);

	delete[] movesBoard;
	return resultBoard;
}

int* tourHelper(int r, int c, int x, int y, int *resultBoard, int *movesBoard, int step, bool *solvedPtr) {
	//mark that this position has been placed
	step++;
	resultBoard[x*c + y] = step;
	if (step == r*c) {
		*solvedPtr = true;
		return resultBoard;
	}

	//evaluate current position
	updateMovesBoard(r,c,x,y,movesBoard,-1);

	list<pair<int,int> > moves = loadMoves(r, c, x, y, movesBoard, resultBoard);

	while (!moves.empty()) {
		resultBoard = tourHelper(r,c,moves.back().first, moves.back().second, resultBoard, movesBoard, step, solvedPtr);
		if (*solvedPtr == true) {
			return resultBoard;
		}
		moves.pop_back();
	}
	//no more moves, backtrack
	updateMovesBoard(r,c,x,y,movesBoard,1);
	resultBoard[x*c+y] = 0;
	return resultBoard;
}

list<pair<int,int> > loadMoves(int r, int c, int x, int y, int *movesBoard, int *resultBoard) {
	list<pair<int,int> > posMoves = getPossibleKnightMoves(x,y);
	list<pair<int,int> > retMoves;
	for (list<pair<int,int> >::iterator it = posMoves.begin(); it != posMoves.end(); it++) {
		int tempX = (*it).first;
		int tempY = (*it).second;
		if (tempX >= 0 && tempX < r && tempY >= 0 && tempY < c && resultBoard[tempX*c+tempY] == 0) {
			pair<int,int> newPair = make_pair(tempX,tempY);
			bool inserted = false;
			for (list<pair<int,int> >::iterator it = retMoves.begin(); it != retMoves.end(); it++) {
				if (movesBoard[tempX*c+tempY] >= movesBoard[(*it).first*c + (*it).second]) {
					retMoves.insert(it,newPair);
					inserted = true;
					break;
				}
			}
			if (!inserted) {
				retMoves.push_back(newPair);
			}
		}
	}

	return retMoves;
}

void updateMovesBoard(int r, int c, int x, int y, int *movesBoard, int amount) {
	list<pair<int,int> > posMoves = getPossibleKnightMoves(x,y);
	for (list<pair<int,int> >::iterator it = posMoves.begin(); it != posMoves.end(); it++) {
		int tempX = (*it).first;
		int tempY = (*it).second;
		if (tempX >= 0 && tempX < r && tempY >= 0 && tempY < c) {
			movesBoard[tempX*c+tempY] += amount;
		}
	}
}

int* initializeBoard(int r, int c) {
	int *initBoard = new int[r*c];

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			list<pair<int,int> > posMoves = getPossibleKnightMoves(i,j);
			int accum = 0;
			for (list<pair<int,int> >::iterator it = posMoves.begin(); it != posMoves.end(); it++) {
				int tempX = (*it).first;
				int tempY = (*it).second;
				if (tempX >= 0 && tempX < r && tempY >= 0 && tempY < c) {
					accum++;
				}
			}
			initBoard[i*c + j] = accum;
		}
	}
	return initBoard;
}

list<pair<int,int> > getPossibleKnightMoves(int x, int y) {
	list<pair<int,int> > moves;

	moves.push_back(make_pair(x-2,y-1));
	moves.push_back(make_pair(x-2,y+1));
	moves.push_back(make_pair(x-1,y-2));
	moves.push_back(make_pair(x-1,y+2));
	moves.push_back(make_pair(x+1,y-2));
	moves.push_back(make_pair(x+1,y+2));
	moves.push_back(make_pair(x+2,y-1));
	moves.push_back(make_pair(x+2,y+1));
	return moves;
}
