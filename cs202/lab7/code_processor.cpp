/* Eli Wilson 10/25/2021
 * lab7 - Code Processing
 * This program emulates a server which stores user data and keeps track of a reward system
 */
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "code_processor.hpp"

using namespace std;

unsigned int djb_hash(const string &s) // hash function used to determine valid codes
{
	size_t i;
	unsigned int h;

	h = 5381;

	for (i = 0; i < s.size(); i++) {
		h = (h << 5) + h + s[i];
	}
	return h;
}


bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity){

	//checks for valid parameters
	if(Prizes.find(id) != Prizes.end() || points <= 0 || quantity <= 0){
		return false;
	}

	Prize *p = new Prize;
	p->id = id;
	p->description = description;
	p->points = points;
	p->quantity = quantity;

	Prizes.insert(make_pair(id, p));

	return true;
}


bool Code_Processor::New_User(const string &username, const string &realname, int starting_points){
	
	//checks for valid parameters
	if(Names.find(username) != Names.end() || starting_points < 0){
		return false;
	}

	User *u = new User;
	u->username = username;
	u->realname = realname;
	u->points = starting_points;

	Names.insert(make_pair(username, u));

	return true;
}


bool Code_Processor::Delete_User(const string &username){
	
	map <string, User *>::const_iterator uit;
	set <string>::iterator sit;

	uit = Names.find(username);
	//checks if user doesnt exist
	if(uit == Names.end()){
		return false;
	}

	//iterates through users #s and deletes them
	for(sit = uit->second->phone_numbers.begin(); sit != uit->second->phone_numbers.end(); sit++){
		Phones.erase(*sit);
	}

	delete uit->second; // deletes user

	Names.erase(username);
	

	return true;
}


bool Code_Processor::Add_Phone(const string &username, const string &phone){
	
	map <string, User *>::const_iterator uit;

	uit = Names.find(username);
	// checks if user doesnt exist or phone # does
	if(uit == Names.end() || Phones.find(phone) != Phones.end()){
		return false;
	}

	// puts phone in user set and map
	uit->second->phone_numbers.insert(phone);
	Phones.insert(make_pair(phone, uit->second));
	
	return true;
}


bool Code_Processor::Remove_Phone(const string &username, const string &phone){
	
	map <string, User *>::const_iterator uit;

	uit = Names.find(username);

	//searches for number in user set instead of map to prevent finding another user's number
	if(uit == Names.end() || uit->second->phone_numbers.find(phone) == uit->second->phone_numbers.end()){
		return false;
	}

	//deletes phone from user set and map
	uit->second->phone_numbers.erase(phone);
	Phones.erase(phone);
	
	return true;
}

string Code_Processor::Show_Phones(const string &username) const{
	
	map <string, User *>::const_iterator uit;
	string nums = "";
	set <string>::const_iterator pit;

	uit = Names.find(username);
	if(uit == Names.end()){
		return "BAD USER";
	}
	
	//iterates through user's #s and adds them to ouput string
	for(pit = uit->second->phone_numbers.begin(); pit != uit->second->phone_numbers.end(); pit++){
		nums += *pit;
		nums += '\n';
	}
	
	return nums;
}


int Code_Processor::Enter_Code(const string &username, const string &code){

	map <string, User *>::const_iterator uit;
	unsigned int key;
	int worth = 0;

	uit = Names.find(username);
	if(Codes.find(code) != Codes.end() || uit == Names.end()){
		return -1;
	}

	key = djb_hash(code);

	//checks for valid code/code value, and adds to set
	if(key % 17 == 0){
		worth = 10;
		Codes.insert(code);
	}else if(key % 13 == 0){
		worth = 3;
		Codes.insert(code);
	}
	
	// adds to user's points
	uit->second->points += worth;
	
	return worth;
}


int Code_Processor::Text_Code(const string &phone, const string &code){
	
	map <string, User *>::const_iterator pit;
	int val;
	
	pit = Phones.find(phone);
	if(pit == Phones.end()){
		return -1;
	}

	// finds username from phone and runs previous function
	val = this->Enter_Code(pit->second->username, code);
	
	return val;
}


bool Code_Processor::Mark_Code_Used(const string &code){
	
	unsigned int key;

	if(Codes.find(code) != Codes.end()){
		return false;
	}

	key = djb_hash(code);

	//checks for valid code, adds to set
	if(key % 13 == 0 || key % 17 == 0){
		Codes.insert(code);
		return true;
	}else{
		return false;
	}
	
}

int Code_Processor::Balance(const string &username) const{
	
	map <string, User *>::const_iterator uit;

	uit = Names.find(username);
	if(uit == Names.end()){
		return -1;
	}

	return uit->second->points;
}


bool Code_Processor::Redeem_Prize(const string &username, const string &prize){

	map <string, Prize *>::const_iterator pit;
	map <string, User *>::const_iterator uit;

	pit = Prizes.find(prize);
	uit = Names.find(username);
	if(pit == Prizes.end() || uit == Names.end() || pit->second->points > uit->second->points){
		return false;
	}

	uit->second->points -= pit->second->points; // subtracts points from user
	pit->second->quantity--; // subtracts 1 from quantity

	if(pit->second->quantity == 0){ // deletes empty prize
		delete pit->second;
		Prizes.erase(prize);
	}
	
	return true;
}


Code_Processor::~Code_Processor(){ // destructor
	map <string, Prize *>::const_iterator pit;
	map <string, User *>::const_iterator uit;

	for(pit = Prizes.begin(); pit != Prizes.end(); pit++){
		delete pit->second;
	}

	for(uit = Names.begin(); uit != Names.end(); uit++){
		delete uit->second;
	}
	
}


bool Code_Processor::Write(const string &filename) const{
	
	map <string, Prize *>::const_iterator pit;
	map <string, User *>::const_iterator uit;
	set <string>::const_iterator cit;
	map <string, User *>::const_iterator phit;
	ofstream fl;
	fl.open(filename.c_str());

	if(Prizes.size() != 0){ // adds prize cmds
		for(pit = Prizes.begin(); pit != Prizes.end(); pit++){
			fl << "PRIZE " << pit->first << " " << pit->second->points << " " <<  pit->second->quantity << " " << pit->second->description << endl;
		}
	}

	if(Names.size() != 0){ // user cmds
		for(uit = Names.begin(); uit != Names.end(); uit++){
			fl << "ADD_USER " << uit->first << " " << uit->second->points << " " << uit->second->realname << endl;
		}
	}

	if(Codes.size() != 0){ // used code cmds
		for(cit = Codes.begin(); cit != Codes.end(); cit++){
			fl << "MARK_USED " << *cit << endl;
		}
	}
	
	if(Phones.size() > 0){ // phone # cmds
		for(phit = Phones.begin(); phit != Phones.end(); phit++){
			fl << "ADD_PHONE " << phit->second->username << " " << phit->first << endl;
		}
	}

	fl.close();
	
	return true;
}

