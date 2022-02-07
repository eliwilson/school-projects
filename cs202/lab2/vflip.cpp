/* Eli Wilson 9/6/2021
 * lab 2 - vflip
 * takes pgm from std input, outputs vertically flipped version
 */
#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

int main(){
	string p2;
	int cols;
	int rows;
	int max;
	int val;
	int numPix = 0;
	vector < vector <int> > row;

	//reads header
	cin >> p2 >> cols >> rows >> max;
	int pixels = rows*cols;

	// error checking
	if(p2 != "P2"){
		cerr << "Bad PGM file -- first word is not P2" << endl;
		return 0;
	}
	if(rows <= 0){	
		cerr << "Bad PGM file -- No row specification" << endl;
		return 0;
	}
	if(cols <= 0){	
		cerr << "Bad PGM file -- No column specification" << endl;
		return 0;
	}
	if(max != 255){	
		cerr << "Bad PGM file -- No 255 following the rows and columns" << endl;
		return 0;
	}
	
	//prints header
	printf("P2\n%d %d\n%d\n", cols, rows, max);
	
	row.resize(rows);
	//cout << "size: " << row.size() << endl;

	//reads, adds items to 3d vector
	while(cin >> val){
		row.at(numPix/cols).push_back(val);
		numPix ++;
		if(numPix > pixels){
			cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
			return 0;
		}
		if(!(val <= 255) || !(val >= 0)){
			cerr << "Bad PGM file -- pixel " << numPix-1 << " is not a number between 0 and 255" << endl;
			return 0;
		}
		
		//cout << "size: " << row.size() << endl;
		//cout << "ind: " << numPix/cols << endl;
	
	}

	// prints rows in reverse in lines of 15
	for(int i = rows-1; i >= 0; i--){
		for(int j = 0; j < cols; j++){
			printf(" %3d", row[i][j]);
			if((j+1)%15 == 0) cout << endl;
			//cout << row[i][j] << endl;
		}
		cout << endl;
	}
	if(numPix != pixels){
		cerr << "Bad PGM file -- pixel " << numPix << " is not a number between 0 and 255" << endl;
		return 0;
	}
	return 0;
}
