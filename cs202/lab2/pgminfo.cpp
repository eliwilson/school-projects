/* Eli Wilson 9/6/2021 
 * Lab 2 - pgminfo
 * reads a pgm file, checks for errors, prints file info
*/
#include <iostream>
#include <cstdio> 
using namespace std;

int main(){
	string p2;
	int cols;
	int rows;
	int max;
	int val;
	int total = 0;
	int numPix = 0;
	
	// Reads header lines
	cin >> p2 >> cols >> rows >> max;
	int pixels = rows*cols;

	// Error checking
	if(p2 != "P2"){
		cerr << "Bad PGM file -- first word is not P2" << endl;
		return 0;
	}
	if(rows < 0){	
		cerr << "Bad PGM file -- No row specification" << endl;
		return 0;
	}
	if(cols < 0){	
		cerr << "Bad PGM file -- No column specification" << endl;
		return 0;
	}
	if(max != 255){	
		cerr << "Bad PGM file -- No max specification" << endl;
		return 0;
	}

	// Reads pixels, adds values to calculate average
	while(cin >> val){
		numPix ++;
		if(numPix > pixels){
			cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
			return 0;
		}
		if(!(val <= 255) || !(val >= 0)){
			cerr << "Bad PGM file -- pixel " << numPix-1 << " is not a number between 0 and 255" << endl;
			return 0;
		}
		total += val;
	}
	if(numPix != pixels){	
		cerr << "Bad PGM file -- Wrong number of pixels" << endl;
		return 0;
	}

	// Prints info
	printf("# Rows:%12d\n", rows);
	printf("# Columns:%9d\n", cols);
	printf("# Pixels:%10d\n", pixels);
	printf("Avg Pixel:%9.3lf\n", (double)total/(double)pixels);

	return 0;
}
