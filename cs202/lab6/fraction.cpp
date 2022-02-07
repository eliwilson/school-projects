/* Eli Wilson 10/18/21
 * lab 6 - Keno
 * This program defines the fraction class: stores a list of products in numerator and denominator
 * allows user to multiply in integers, factorials, and binomials
 */
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>
#include "fraction.hpp"
using namespace std;

void Fraction::Clear()
{
	numerator.clear();
	denominator.clear();
}

bool Fraction::Multiply_Number(int n)
{
	if(n <= 0){
		return false;
	}
	
	multiset <int>::const_iterator it;

	// checks for number in denominator to cancel with
	// if it isn't there it is added to numerator
	it = denominator.find(n);
	if(it != denominator.end()){
		denominator.erase(it);
	}else if(n != 1){
		numerator.insert(n);
	}

	return true;
}

bool Fraction::Divide_Number(int n)
{
	if(n <= 0){
		return false;
	}

	multiset<int>::iterator it;

	// checks for number in numerator to cancel with
	// if it isn't there it is added to denominator
	it = numerator.find(n);
	if(it != numerator.end()){
		numerator.erase(it);
	}else if(n != 1){
		denominator.insert(n);
	}

	return true;

}

bool Fraction::Multiply_Factorial(int n)
{
	if(n <= 0){
		return false;
	}
	multiset <int>::const_iterator it;

	for(int i = 2; i <= n; i++){
		this->Multiply_Number(i);
	}

	return true;
}
bool Fraction::Divide_Factorial(int n)
{
	if(n <= 0){
		return false;
	}
	multiset <int>::const_iterator it;

	for(int i = 2; i <= n; i++){
		this->Divide_Number(i);
	}

	return true;
}

bool Fraction::Multiply_Binom(int n, int k)
{
	if(n < 0 || k < 0){
		return false;
	}

	this->Multiply_Factorial(n);
	this->Divide_Factorial(k);
	this->Divide_Factorial(n-k);

	
	return true;

}

bool Fraction::Divide_Binom(int n, int k)
{
	if(n < 0){
		return false;
	}
	this->Divide_Factorial(n);
	this->Multiply_Factorial(k);
	this->Multiply_Factorial(n-k);

	return true;
}

void Fraction::Invert()
{
	multiset <int> dcopy;
	dcopy = denominator;
	denominator = numerator;
	numerator = dcopy;
}

void Fraction::Print() const
{
	multiset <int>::const_iterator nit;

	if(numerator.size() == 0){
		cout << "1";
	}
	for (nit = numerator.begin(); nit != numerator.end(); nit++) {
		if(nit != numerator.begin()){
			cout << " * ";
		}
		cout << *nit;
	}

	for (nit = denominator.begin(); nit != denominator.end(); nit++) {
		cout << " / ";
		cout << *nit;
	}
	cout << endl;
}
double Fraction::Calculate_Product() const
{
	multiset <int>::const_iterator nit;
	double prod = 1;

	for (nit = numerator.begin(); nit != numerator.end(); nit++) {
		prod *= (double)*nit;
	}

	for (nit = denominator.begin(); nit != denominator.end(); nit++) {
		prod /= (double)*nit;
	}
	
	return prod;
}
