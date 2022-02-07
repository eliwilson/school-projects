/* Eli Wilson 9/6/2021
 * Lab 2 bigwhite
 * Takes rows and columns as input arguments, makes white pgm with those dimensions
 */

#include <iostream>
#include <cstdio> 
using namespace std;

int main(int argc, char **argv){
	int cols = 0;
	int rows = 0;
	
	// stores dimensions into ints
	sscanf(argv[1], "%d", &rows);
	sscanf(argv[2], "%d", &cols);
	
	//cout << "rows, cols: " << rows << " " << cols << endl;
	
	// Error checking
	if(argc != 3){
		cerr << "usage: bigwhite rows cols" << endl;
		return 0;
	}
	if(rows <= 0){	
		cerr << "usage: bigwhite rows cols" << endl;
		return 0;
	}
	if(cols <= 0){	
		cerr << "usage: bigwhite rows cols" << endl;
		return 0;
	}

	// Outputs file
	printf("P2\n%d %d\n255\n", cols, rows);
	for(int i = 0; i < rows; i++){
		cout << "255";
		for(int j = 0; j < cols-1; j++){
			cout << " 255";
		}
		cout << endl;
	}

	return 0;
}
