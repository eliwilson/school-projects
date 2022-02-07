/* Eli Wilson - 10/5/2021
 * lab 5 Bit matrices
 * Defines bitmatrix, HTE, and BM_Hash functions allow user to create, store, and perform operations on bit matrices
*/

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bitmatrix.hpp"
#include <iostream>
using namespace std;

Bitmatrix::Bitmatrix(int rows, int cols) // Creates rows x cols matrix of 0s 
{
	if(rows <= 0) throw((string) "Bad rows");
	if(cols <= 0) throw((string) "Bad cols");
	
	M.resize(rows);
	for(size_t i = 0; i < M.size(); i++){
		for(int j = 0; j < cols; j++){
			M[i].push_back('0');
		}
	}
}
           
Bitmatrix::Bitmatrix(const string &fn) // Reads bm from file
{
	ifstream fin;
	string line;
	int c = 0;

	fin.open(fn.c_str());
	if(fin.fail()) throw((string) "Can't open file");
	
	// Reads non-empty lines and copies non-whitespace characters to M
	while(getline(fin, line)){
		if(line.size() != 0){
			M.push_back("");
			for(size_t i = 0; i < line.size(); i++){
				if(line[i] != ' '){
					M[c].push_back(line[i]);
				}
			}
			c++;
		}
	}
}
        
Bitmatrix *Bitmatrix::Copy() const // Returns copy of stored matrix
{
	Bitmatrix *b = new Bitmatrix(M.size(), M[0].size()); //creates empty matrix of same size
	char v;

	// Copies values from M
	for(size_t i = 0; i < M.size(); i++){
		for(size_t j = 0; j < M[0].size(); j++){
			v = M[i][j];
			b->Set(i, j, v);
		}
	}

	return b;
}
                 
bool Bitmatrix::Write(const string &fn) const // writes stored matrix to file
{
	ofstream fl;
	fl.open(fn.c_str());

	// Puts each bit into file individually
	for(size_t i = 0; i < M.size(); i++){
		for(size_t j = 0; j < M[0].size(); j++){
			fl.put(M[i][j]);
		}
		fl.put('\n');
	}
	

	return true;
}
 
void Bitmatrix::Print(size_t w) const // Outputs stored matrix
{
	bool s = !(w == 0); // checks if spaces are wanted
	for(size_t i = 0; i < M.size(); i++){
		if(s && i % w == 0 && i != 0){ // puts newline at wth rows
			cout << endl;
		}
		for(size_t j = 0; j < M[i].size(); j++){
			if(s && j % w == 0 && j != 0){ // adds space at wth cols
				cout << " ";	
			}
			cout << M[i][j]; // prints bit
		}
		cout << endl;
	}

}
                 
// Creates checkerboard PGM from matrix with 0=white 1=gray and black border
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
	ofstream fl;
	int rows = p*this->Rows();
	int cols = p*this->Cols();
	int bwidth = cols + (border * (this->Cols() + 1));
	int bheight = rows + (border * (this->Rows() + 1));
	
	fl.open(fn.c_str());
	fl << "P2\n" << bwidth << " " << bheight << "\n255\n"; // header
	
	for(int i = 0; i < this->Rows(); i++){ // M row
		// horizontal border between rows
		for(int x = 0; x < border; x++){ 
			for(int y = 0; y < bwidth; y++){
				fl << "0 ";
			}
			fl << '\n';
		}

		for(int a = 0; a < p; a++){ // Row within box

			for(int j = 0; j < this->Cols(); j++){ // M col

				int val = 100;
				if(M[i][j] == '0') val = 255;

				//vertical border between cols
				for(int z = 0; z < border; z++){
						fl << "0 ";
				}

				for(int b = 0; b < p; b++){ // Col within box
					fl << val << " "; 

				}	
			}

			//last vertical border
			for(int z = 0; z < border; z++){
					fl << "0 ";
			}

			fl << '\n';
		}
	}

	//last horizontal border
	for(int x = 0; x < border; x++){
		for(int y = 0; y < bwidth; y++){
			fl << "0 ";
		}
		fl << '\n';
	}

	fl.close();
	

	return true;
	
	
}

int Bitmatrix::Rows() const // rowcount
{
  return (int)M.size();
}
                            
int Bitmatrix::Cols() const // column count
{
  return (int)M[0].size();
}
                            
char Bitmatrix::Val(int row, int col) const // returns value at index
{
	if(row < (int)M.size() && col < (int)M[0].size()){
		return M[row][col];
	}
	
	return 'x';
}
            
bool Bitmatrix::Set(int row, int col, char val) // sets bit at index to val
{
	if(row >= (int)M.size() || col >= (int)M[0].size() || row < 0 || col < 0){
		return false;
	}
	
	if(val == 0 || val == '0'){
		M[row][col] = '0';
		return true;
	}else if(val == 1 || val == '1'){
		M[row][col] = '1';
		return true;
	}
	
	return false;
}
        
bool Bitmatrix::Swap_Rows(int r1, int r2) // swaps rows r1 and r2
{
	if(r1 >= this->Rows() || r2 >= this->Rows()) return false;
	string rCopy = M[r1];
	M[r1] = M[r2];
	M[r2] = rCopy;
	return true;
}
              
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2) // row r1 += row r2
{
	if(r1 >= this->Rows() || r2 >= this->Rows()){
		return false;
	}
	
	char sum;
	for(size_t i = 0; i < M[r1].size(); i++){
		if(M[r1][i] != M[r2][i]){ // != checks if sum is 1 or 0
			sum = '1';
		}else{
			sum = '0';
		}
		M[r1][i] = sum;
	}
	
	return true;
}
      

Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2) // adds two matrices
{

	int r = a1->Rows();
	int c = a1->Cols();
	if(r != a2->Rows() || c != a2->Cols()){
		return NULL;
	}
	
	Bitmatrix *bm = new Bitmatrix(r, c);
	char v;

	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			if(a1->Val(i, j) != a2->Val(i, j)){ // != checks if v is 0 or 1
				v = '1';
			}else{
				v = '0';
			}
			bm->Set(i, j, v);

		}
	}
	
	return bm;
}

Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2) // a1 x a2
{
	int r = a1->Rows();
	int c = a2->Cols();
	int l = a1->Cols();
	if(a1->Cols() != l){
		return NULL;
	}
	
	Bitmatrix *bm = new Bitmatrix(r, c);
	int sum = 0;
	char v;


	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			for(int k = 0; k < l; k++){
				sum += (a1->Val(i, k) - '0')*(a2->Val(k, j) - '0');
			}
			v = (sum % 2) + '0';
			bm->Set(i, j, v);
			sum = 0;

		}
	}
	
	return bm;
	
	
}

Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows) // returns matrix with specified rows from a1
{

	int r = rows.size();
	int c = a1->Cols();

	if(rows.size() == 0) return NULL;
	
	Bitmatrix *bm = new Bitmatrix(r, c);
	char v;

	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			if(rows[i] >= a1->Rows()) return NULL;
			v = a1->Val(rows[i], j);
			
			bm->Set(i, j, v);

		}
	}
	
	return bm;
}
	
Bitmatrix *Inverse(const Bitmatrix *m) // returns inverse of m
{
	if(m->Rows() != m->Cols()) return NULL;

	Bitmatrix *id = new Bitmatrix(m->Rows(), m->Cols());
	Bitmatrix *mc = new Bitmatrix(m->Rows(), m->Cols());
	mc = m->Copy();


	for(int i = 0; i < id->Rows(); i++){
		id->Set(i, i, 1);
	}

	for(int i = 0; i < mc->Rows(); i++){ // First loop for taking inverse
		// checks if row should be swapped
		if(mc->Val(i, i) == '0'){
			bool invertible = false;
			for(int j = i+1; j < mc->Rows(); j++){
				// checks for 1 in correct place
				if(mc->Val(j, i) == '1'){
					invertible = true;
					id->Swap_Rows(i, j);
					mc->Swap_Rows(i, j);
				}
			}
			if(!invertible) return NULL;
		}
		// checks for 1s below leading 1
		for(int j = i+1; j < mc->Rows(); j++){
			if(mc->Val(j, i) == '1'){
				id->R1_Plus_Equals_R2(j, i);
				mc->R1_Plus_Equals_R2(j, i);
			}
		}
	}
	
	for(int i = mc->Rows()-1; i >= 0; i--){ // second loop cancels 1s above leading 1s
		for(int j = i-1; j >= 0; j--){
			if(mc->Val(j, i) == '1'){
				id->R1_Plus_Equals_R2(j, i);
				mc->R1_Plus_Equals_R2(j, i);
			}
		
		}
	
	}


	
	return id;

}



BM_Hash::BM_Hash(int size)
{

	Table.resize(size);

}

unsigned int djb_hash(const string &s) // hash function
{
  size_t i;
  unsigned int h;
  
  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

bool BM_Hash::Store(const string &key, Bitmatrix *bm) // stores bm in hash table
{
	int ind = djb_hash(key) % Table.size();
	
	if(bm->Rows() == 0 || bm->Cols() == 0){
		return false;
	}

	HTE *hash = new HTE;
	hash->key = key;
	hash->bm = bm;
	for(size_t i = 0; i < Table[ind].size(); i++){
		if(Table[ind][i].key == key){
			return false;
		}
	}
	Table[ind].push_back(*hash);

	return true;
}
    
Bitmatrix *BM_Hash::Recall(const string &key) const // takes bm with "key" from hash table
{
	int ind = djb_hash(key) % Table.size();
	
	for(size_t i = 0; i < Table[ind].size(); i++){
		if(Table[ind][i].key == key && Table[ind][i].bm != NULL){
			return Table[ind][i].bm;
		}
	}
	
	return NULL;
}
      
vector <HTE> BM_Hash::All() const // returns all hash table entries
{
	vector <HTE> rv;
	for(size_t i = 0; i < Table.size(); i++){
		for(size_t j = 0; j < Table[i].size(); j++){
			rv.push_back(Table[i][j]);
		}
	}

	return rv;
}

