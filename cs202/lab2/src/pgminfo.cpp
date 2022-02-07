#include <iostream>
using namespace std;

int main(){
	string p2;
	int cols;
	int rows;
	int max;
	int val;
	int total = 0;

	cin >> p2 >> cols >> rows >> max;
	int pixels = rows*cols;

	if(p2 != "P2"){
		cerr << "Bad PGM file -- no p2" << endl;
		return 0;
	}
	if(

	while(cin >> val){
		total += val;
	}
	printf("# Rows:%12d\n", rows);
	printf("# Columns:%9d\n", cols);
	printf("# Pixels:%10d\n", pixels);
	printf("Avg Pixel:%9.3lf\n", (double)total/(double)pixels);

	return 0;
}
