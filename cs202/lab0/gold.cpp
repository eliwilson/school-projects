/*
Eli Wilson
8/25/2021
Lab 0: Adds position in alphabet for each letter input
*/
#include <iostream>

using namespace std;

int main () {
	int ounces = 0;
	char c;
	char a = 'A';
	
	while(cin >> c){
		if(c >= 'A' && c <= 'Z'){ // checks if it is a letter
			ounces += c - a + 1; // adds position
		}
	}
	cout << ounces << endl; // outputs total
	return 0;
}
