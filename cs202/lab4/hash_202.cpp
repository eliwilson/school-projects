/* Eli Wilson 9/20/2021
 * Lab4 - Hash tables with open addressing
 * This program defines the Hash_202 class, stores a hash table that can use Linear probing
 * or Double Hashing, and can use either an XOR or last7 hash function. Can also print and find elements
 */

#include "hash_202.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

string Hash_202::Set_Up(size_t table_size, const std::string &fxn, const std::string &collision)
{
	if(Keys.size() > 0){
		return "Hash table already set up";
	}
	if(table_size == 0){
		return "Bad table size";
	}
	
	Keys.resize(table_size);
	Vals.resize(table_size);

	// Sets hash function
	if(fxn == "Last7"){
		Fxn = 'L';
	}else if(fxn == "XOR"){
		Fxn = 'X';
	}else{
		return "Bad hash function";
	}

	// Sets collision resolution
	if(collision == "Linear"){
		Coll = 'L';
	}else if(collision == "Double"){
		Coll = 'D';
	}else{
		return "Bad collision resolution strategy";
	}

	Nkeys = 0;
	Nprobes = 0;

	return "";
}

string Hash_202::Add(const string &key, const string &val)
{
	size_t s = Keys.size();
	if(s == 0){
		return "Hash table not set up";
	}
	if(key == ""){
		return "Empty key";
	}
	for(size_t i = 0; i < key.size(); i++){
		if(key[i] < 48 || key[i] > 102 || (key[i] > 58 && key[i] < 97)){
			return "Bad key (not all hex digits)";
		}
	}
	if(val == ""){
		return "Empty val";
	}
	if(Nkeys == s){
		printf("A %s %s\n", key.c_str(), val.c_str());
		cout << "Hash table full" << endl;
		return "";
	}

	int keyInt;
	int lastInt = 0;
	int xorInt = 0;
	int inc;
	string last7;

	//LAST7
	if(Fxn == 'L' || Coll == 'D'){
		if(key.size() > 7){
			last7 = key.substr(key.size()-7);
		}else{
			last7 = key.substr(key.size()-7+(7-key.size()));
		}
		sscanf(last7.c_str(), "%x", &lastInt);
	}

	// XOR
	if(Fxn == 'X' || Coll == 'D'){
		xorInt = 0;
		string group;
		int groupInt;
		int loop = 0;
		if(key.size() % 7 != 0) loop = 1;

		for(size_t i = 0; i < (key.size() / 7)+loop; i++){
			group = key.substr(7*i, 7);
			sscanf(group.c_str(), "%x", &groupInt);
			xorInt ^= groupInt;
		}
	}
	
	// Sets number for indexing and increment for double hashing
	if(Fxn == 'L'){
		keyInt = lastInt;
		inc = xorInt % s;
	}else{
		keyInt = xorInt;
		inc = lastInt % s;
	}
	if(inc == 0) inc++;
	size_t ind = keyInt % s;

	// Uses collision resolution to until empty space is found for value
	size_t c = 0;
	while(c < s*3){
		if(Keys[ind].length() != 0){
			if(Keys[ind] == key) return "Key already in the table";
			if(Coll == 'L'){ // Linear
				if(ind == s-1){
					ind = 0;
				}else{
					ind++;
				}
			}
			if(Coll == 'D'){ // Double
				c++;
				for(int i = 0; i < inc; i++){
					if(ind == s-1){
						ind = 0;
					}else{
						ind++;
					}
					
				}
			}
		}else{	
			Keys[ind] = key;
			Vals[ind] = val;
			break;
		}
	}

	// Checks if key was inserted succesfully
	if(c == s*3){
		printf("A %s %s\n", key.c_str(), val.c_str());
		cout << "Cannot insert key" << endl;
	}else{
		Nkeys++;
	}

	return "";
}

// Finds value from key
string Hash_202::Find(const string &key)
{
	size_t s = Keys.size();
	int keyInt;
	int lastInt = 0;
	int xorInt = 0;
	int inc;
	string last7;

	//LAST7
	if(Fxn == 'L' || Coll == 'D'){
		if(key.size() > 7){
			last7 = key.substr(key.size()-7);
		}else{
			last7 = key.substr(key.size()-7+(7-key.size()));
		}
		sscanf(last7.c_str(), "%x", &lastInt);
	}

	// XOR
	if(Fxn == 'X' || Coll == 'D'){
		xorInt = 0;
		string group;
		int groupInt;
		int loop = 0;
		if(key.size() % 7 != 0) loop = 1;

		for(size_t i = 0; i < (key.size() / 7)+loop; i++){
			group = key.substr(7*i, 7);
			sscanf(group.c_str(), "%x", &groupInt);
			xorInt ^= groupInt;
		}
	}
	
	if(Fxn == 'L'){
		keyInt = lastInt;
		inc = xorInt % s;
	}else{
		keyInt = xorInt;
		inc = lastInt % s;
	}

	if(inc == 0) inc++;
	size_t ind = keyInt % s;

	// uses Collision resolution to search for key in table
	Nprobes = 0;
	while(Nprobes < s){
		if(Keys[ind] != key){
			if(Keys[ind].length() == 0) break;
			Nprobes++;
			if(Coll == 'L'){
				if(ind == s-1){
					ind = 0;
				}else{
					ind++;
				}
			}
			if(Coll == 'D'){
				for(int i = 0; i < inc; i++){
					if(ind == s-1){
						ind = 0;
					}else{
						ind++;
					}
				}
			}
		}else{
			return Vals[ind];
		}
	}
	Nprobes = 0;
	return "";
}

// Prints out all non empty Key/Values in table
void Hash_202::Print() const
{
	if(Keys.size() != 0){
		for(size_t i = 0; i < Keys.size(); i++){
			if(Keys[i].length() != 0){
				printf("%5d %s %s\n", (int)i, Keys[i].c_str(), Vals[i].c_str());
			}
		}
	}
}

// returns probes needed to find all entries
size_t Hash_202::Total_Probes()
{
	int probes = 0;
	for(size_t i = 0; i < Keys.size(); i++){
		if(Keys[i].length() != 0){
			Find(Keys[i]);
			probes += Nprobes;
		}
	}
	return probes;
}
