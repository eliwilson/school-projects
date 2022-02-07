/*
Eli Wilson
8/25/2021
Lab 1: Prints increasing series of characters into a checkerboard of specified length/width
*/
#include <iostream>
using namespace std;

int main()
{
    int numRows;
    int numCols;
    char start;
    int cSize;
    int width;

    if(!(cin >> numRows >> numCols >> start >> cSize >> width)){ // Gets dimensions from user
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W" << endl;
		return 0;
	}

	// checks for invalid input
	if(numRows <= 0 || numCols <= 0 || start <= 0 || cSize <= 0 || width <= 0 || (start + cSize) > 127){
		return 0;
	}

    for(int i = 0; i < numRows; i++){
		for(int r = 0; r < width; r++){
			for(int j = 0; j < numCols; j++){
				char c = start + (i + j) % cSize; // finds current character
				for(int k = 0; k < width; k++){
					cout << c;
				}
			}
			cout << endl;
		}
    }
    return 0;
}