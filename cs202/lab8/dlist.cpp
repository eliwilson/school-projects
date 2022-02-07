/* Eli Wilson 11/1/2021
 * lab8 Linked Lists - Implements Doubly-Linked-Lists
 */

#include <iostream>
#include "dlist.hpp"
using namespace std;


Dnode* Dnode::Next(){
	return flink;
}

Dnode* Dnode::Prev(){
	return blink;
}

/* Constructors, Destructor, Assignment Overload */
Dlist::Dlist(){
	Dnode *n = new Dnode;
	n->s = "";
	n->flink = n;
	n->blink = n;
	sentinel = n;
	size = 0;
}

Dlist::Dlist(const Dlist &d){
		
	Dnode *sent = new Dnode;

	sent->s = "";
	sent->flink = sent;
	sent->blink = sent;
	sentinel = sent;

	*this = d;

}

Dlist& Dlist::operator= (const Dlist &d){
	Dlist tmp;
	Dnode *n;
	Clear();

	// adds nodes to tmp
	for(n = d.Begin(); n != d.End(); n = n->flink){
		tmp.Push_Back(n->s);
	}

	// moves nodes to this
	while(!tmp.Empty()){
		Push_Back(tmp.Pop_Front());
	}


	return *this;
}

Dlist::~Dlist(){
	this->Clear();

	delete sentinel;
}

//Erases all nodes but sentinel
void Dlist::Clear(){
	Dnode *i;

	for(i = this->Begin(); i != this->End(); i = i->flink){
		this->Erase(i);
	}
}
  
// returns whether dlist is empty
bool Dlist::Empty() const{
	return size == 0;
}

//returns size
size_t Dlist::Size() const{
	return size;
}

// inserts node after sentinel
void Dlist::Push_Front(const std::string &s){
	this->Insert_After(s, sentinel);
}

// inserts node before sentinel
void Dlist::Push_Back(const std::string &s){
	this->Insert_Before(s, sentinel);
	
}

// deletes sentinel->flink node
string Dlist::Pop_Front(){
	string s = this->Begin()->s;
	this->Erase(this->Begin());
	return s;
}

// Erases sentinel->blink
string Dlist::Pop_Back(){
	string s = sentinel->blink->s;
	this->Erase(sentinel->blink);
	return s;

}


Dnode * Dlist::Begin() const{
	return sentinel->flink; // node after sentinel
}
     
Dnode * Dlist::End() const{
	return sentinel;

}

Dnode * Dlist::Rbegin() const{
	return sentinel->blink; // node before sentinel

}

Dnode * Dlist::Rend() const{
	return sentinel;

}

void Dlist::Insert_Before(const std::string &s, Dnode *n){
	Dnode *prev; // node before newnode
	Dnode *newnode; // node being added

	prev = n->blink;

	// sets newnode vars
	newnode = new Dnode;
	newnode->s = s;
	newnode->flink = n;
	newnode->blink = prev;

	// prev and n point to newnode since it is between them now
	prev->flink = newnode;
	n->blink = newnode;

	size++;

}

void Dlist::Insert_After(const std::string &s, Dnode *n){
	Dnode *next; // node after newnode
	Dnode *newnode; // node being added

	next = n->flink;

	// sets newnode vars
	newnode = new Dnode;
	newnode->s = s;
	newnode->blink = n;
	newnode->flink = next;

	// next and n point to newnode since it is between them now
	next->blink = newnode;
	n->flink = newnode;

	size++;


}

void Dlist::Erase(Dnode *n){
	// nodes on both sides of node being deleted
	Dnode *prev;
	Dnode *next;

	prev =	n->blink;
	next = n->flink;

	// points them at eachother instead of node being deleted.
	next->blink = prev;
	prev->flink = next;

	delete n;
	size--;
	
}

