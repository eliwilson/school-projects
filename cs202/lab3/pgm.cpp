/* Eli Wilson 9/14/21
 * Lab 3 - Function definitions for Pgm class
 * allows user to Read, Write, Create, Rotate,
 * Add border, Panel, and Crop
 */

#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file)
{
  ifstream fin;
  string s;
  size_t i, j, r, c, v;

  fin.open(file.c_str());
  if (fin.fail()) return false;
  
  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const std::string &file) const{    // Writes a PGM file to std output
	if(Pixels.size() <= 0){
		cout << "WRITE failed" << endl;
		return true;
	}else if(Pixels[0].size() <= 0){	
		cout << "WRITE failed" << endl;
		return true;
	}

	ofstream fl;
	fl.open(file.c_str());

	// File header
	fl << "P2" << endl;
	fl << Pixels[0].size() << " " << Pixels.size() << endl;
	fl << "255" << endl;

	// loops through pixels and writes to file
	int c = 0;
	for(size_t i = 0; i < Pixels.size(); i++){
		for(size_t j = 0; j < Pixels[i].size(); j++){
			fl << Pixels[i][j];
			c++;
			if(c % 20 == 0){ // newline after 20 pixels
				fl << endl;
			}else if(c != (int)(Pixels.size()*Pixels[i].size())){
				fl << " ";
			}
		}
	}
	if(c % 20 != 0) fl << endl;
	fl.close();

	return true;
}
bool Pgm::Create(size_t r, size_t c, size_t pv){   // Creates a rxc PGM with the given pixel value
	if(r <= 0 || c <= 0 || pv > 255){
		cout << "CREATE failed" << endl;
		return true;
	}
	Pixels.resize(r);
	
	//Puts pv into each element of the vector
	for(size_t i = 0; i < r; i++){
		for(size_t j = 0; j < c; j++){
			Pixels[i].push_back(pv);
		}
	}

	return true;
}
bool Pgm::Clockwise(){                             // Rotates 90 degrees clockwise
	//Stores rotated Pixels into cw
	vector < vector <int> > cw;
	cw.resize(Pixels[0].size());
	for(size_t i = 0; i < Pixels[0].size(); i++){
		for(size_t j = Pixels.size(); j > 0; j--){
			cw[i].push_back(Pixels[j-1][i]);
		}
	}

	//copies cw into Pixels
	Pixels.clear();
	Pixels.resize(cw.size());
	for(size_t i = 0; i < cw.size(); i++){
		for(size_t j = 0; j < cw[0].size(); j++){
			Pixels[i].push_back(cw[i][j]);
		}
	}
	return true;
}
bool Pgm::Cclockwise(){                            // Rotates 90 degrees counterclockwise
	//Stores rotated Pixels into ccw
	vector < vector <int> > ccw;
	ccw.resize(Pixels[0].size());
	for(size_t i = 0; i < Pixels[0].size(); i++){
		for(size_t j = 0; j < Pixels.size(); j++){
			ccw.at(i).push_back(Pixels.at(j).at(Pixels[0].size()-i-1));
		}
	}
	//Copies ccw into Pixels
	Pixels.clear();
	Pixels.resize(ccw.size());
	for(size_t i = 0; i < ccw.size(); i++){
		for(size_t j = 0; j < ccw[0].size(); j++){
			Pixels.at(i).push_back(ccw[i][j]);
		}
	}
	return true;
}
bool Pgm::Pad(size_t w, size_t pv){                // Adds a border of w pixels with the given val
	if(pv > 255){
		cout << "PAD failed" << endl;
		return true;
	}

	vector < vector <int> > pd;
	pd.resize(Pixels.size() + (2 * w)); // sets height
	for(size_t i = 0; i < w; i++){
		for(size_t j = 0; j < (Pixels[0].size() + (2 * w)); j++){
			pd[i].push_back(pv); // top pad
			pd[pd.size()-i-1].push_back(pv); // bottom pad
		}
	}
	for(size_t i = w; i < (Pixels.size()+w); i++){
		for(size_t j = 0; j < w; j++) pd[i].push_back(pv); // left pad
		for(size_t k = 0; k < Pixels[0].size(); k++){
			pd[i].push_back(Pixels[i-w][k]); // pixels
		}
		for(size_t j = 0; j < w; j++) pd[i].push_back(pv); // right pad

	}

	// copies pd into Pixels
	Pixels.clear();
	Pixels.resize(pd.size());
	for(size_t i = 0; i < pd.size(); i++){
		for(size_t j = 0; j < pd[0].size(); j++){
			Pixels[i].push_back(pd[i][j]);
		}
	}

	return true;
}
bool Pgm::Panel(size_t r, size_t c){               // Makes rxc copies of the PGM
	if(r == 0 || c == 0){
		cout << "PANEL failed" << endl;
		return true;
	}
	//makes copy of pixels
	vector < vector <int> > cpy;
	cpy.resize(Pixels.size());
	for(size_t i = 0; i < Pixels.size(); i++){
		for(size_t j = 0; j < Pixels[0].size(); j++){
			cpy[i].push_back(Pixels[i][j]);
		}
	}
	Pixels.clear();
	size_t newSize = cpy.size() * r;
	Pixels.resize(newSize);
	// adds every element of cpy for every panel
	for(size_t i = 0; i < r; i++){
		for(size_t j = 0; j < c; j++){
			for(size_t k = 0; k < cpy.size(); k++){
				for(size_t l = 0; l < cpy[0].size(); l++){
					Pixels[k + (i * cpy.size())].push_back(cpy[k][l]);
				}
			}
		}
	}

	
	return true;
}
bool Pgm::Crop(size_t r,    size_t c, size_t rows, size_t cols){
	if((r+rows) > Pixels.size() || (c+cols) > Pixels[0].size()){
		cout << "CROP failed" << endl;
		return true;
	}
	//copies Pixels
	vector < vector <int> > cpy;
	cpy.resize(Pixels.size());
	for(size_t i = 0; i < Pixels.size(); i++){
		for(size_t j = 0; j < Pixels[0].size(); j++){
			cpy[i].push_back(Pixels[i][j]);
		}
	}
	Pixels.clear();
	Pixels.resize(rows);
	//gets desired pixels from cpy
	for(size_t i = r; i < (rows + r); i++){
		for(size_t j = c; j < (cols + c); j++){
			Pixels[i-r].push_back(cpy[i][j]);
		}
	}

	return true;
}
