/* Eli Wilson
 * Lab 9 - enum
 * Prints all possible binary numbers of a specified length and amount of 1's
 */
#include <iostream>
#include <sstream>
using namespace std;

void do_enumeration(string &s, int index, int n_ones){
	
	int ones = n_ones;
	unsigned int ind = index;
	bool bit = (s.size() - ind) <= (unsigned)ones; // determines if bit is set as 1 or 0

	if(ind == s.size()){ // Base case - past end of string
		cout << s << endl;
		return;
	}
	
	// checks if bit has to be one and sets to zero otherwise
	if(bit){
		s[ind] = '1';
		ones--;
	}else{
		s[ind] = '0';
	}

	// calls enum twice if bit was set to zero
	do_enumeration(s, ind+1, ones);
	if(!bit && ones > 0){
		s[ind] = '1';
		ones--;
		do_enumeration(s, ind+1, ones);
	}
	
	

}

int main(int argc, char **argv){
	istringstream ss;
	string s;
	int len;
	int ones;

	ss.str(argv[1]);
	ss >> len;
	ss.clear();
	ss.str(argv[2]);
	ss >> ones;
	
	for(int i = 0; i < len; i++) s+='-';

	do_enumeration(s, 0, ones);






	return 0;
}
