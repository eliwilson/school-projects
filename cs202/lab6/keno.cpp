/* Eli Wilson 10/18/21
 * lab 6 - Keno
 * This program uses the fraction class to print the probability of bets from standard input
 */

#include <iostream>
#include <cstdio>
#include <map>
#include "fraction.hpp"

using namespace std;

int main(){
	double bet, pay, prob, ret, sum;
	int balls, ctch;
	Fraction frac;

	// input bet amount and total number of balls
	cin >> bet >> balls;

	printf("Bet: %.2lf\n", bet);
	cout << "Balls Picked: " << balls << endl;
	
	// reads through bets, calculates binomial, outputs probability/return
	sum = 0;
	while(cin >> ctch >> pay){
		frac.Clear();
		frac.Multiply_Binom(80-balls, 20-ctch);
		frac.Multiply_Binom(balls, ctch);
		frac.Divide_Binom(80, 20);
		prob = frac.Calculate_Product();
		ret = prob*pay;
		sum += ret;
		cout << "  Probability of catching " << ctch << " of " << balls << ": " << prob <<  " -- Expected return: " << ret << endl;
	}

	// return/bet and normalized return
	printf("Your return per bet: %.2lf\n", sum-bet);
	printf("Normalized: %.2lf\n", (sum-bet)/bet);
	
	return 0;
}
