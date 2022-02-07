/* Eli Wilson lab1 CS302
 * Read info about songs from file, 
 * sorts them by artist and album,
 * then prints organized info
 *
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <fstream>
using namespace std;

class Song { 
  public:
    string title;
    int time;
    int track;
};

class Album {
  public:
    map <int, Song *> songs;
    string name;
    int time;
};

class Artist {
  public:
    map <string, Album *> albums;
    string name;
    int time;
    int nsongs;
};

// converts time string to int of total seconds
int timeToInt(string s){
	int minutes = 0;
	int seconds = 0;

	sscanf(s.c_str(), "%d:%d", &minutes, &seconds);

	return (minutes*60) + seconds;
}

// converts int of seconds to min:sec string
string intToTime(int timeInt){
	int minutes = timeInt/60;
	int seconds = timeInt%60;
	char buf[100];
	string s;
	sprintf(buf, "%d:%02d", minutes, seconds);
	s = buf;
	return s;
}

// changes underscores to spaces in string
void respace(string &s){
	for(size_t i = 0; i < s.size(); i++){
		if(s[i] == '_'){
				s[i] = ' ';
		}
	}
}

int main(int argc, char **argv){
	(void) argc;
	ifstream fl;
	string line;
	string title;
	string timeStr;
	string artist;
	string album;
	string genre;
	int track;
	int timeInt;
	map <string, Artist *> artists;
	map <string, Artist *>::iterator artIt;
    map <string, Album *>::iterator alIt;
    map <int, Song *>::iterator sIt;


	fl.open(argv[1]);

	while (fl >> title >> timeStr >> artist >> album >> genre  >> track){		
		timeInt = timeToInt(timeStr);
		respace(title);
		respace(artist);
		respace(album);

		// Create song
		Song* song = new Song;
		song->time = timeInt;
		song->title = title;
		song->track = track;

		// Add artists & update info
		artIt = artists.find(artist);
		Artist* art;
		if(artIt == artists.end()){
			art = new Artist;
			art->name = artist;
			art->time = timeInt;
			art->nsongs = 1;
			artists.insert(make_pair(artist, art));
		}else{
			art = artIt->second;
			art->time += timeInt;
			art->nsongs++;
		}

		// Add/Update album
		alIt = art->albums.find(album);
		Album* alb;
		if(alIt == art->albums.end()){
			alb = new Album;
			alb->time = timeInt;
			art->albums.insert(make_pair(album, alb));
				
		}else{
			alb = alIt->second;
			alb->time += timeInt;
		}
		alb->songs.insert(make_pair(track, song));

	}

	// Print Everything
	for (artIt = artists.begin(); artIt != artists.end(); artIt++) {
		cout << artIt->first << ": " << artIt->second->nsongs << ", " << intToTime(artIt->second->time) << endl; // Artists
		for (alIt = artIt->second->albums.begin(); alIt != artIt->second->albums.end(); alIt++) {
			cout << "        " << alIt->first << ": " << alIt->second->songs.size() << ", " << intToTime(alIt->second->time) << endl; // Albums
			for (sIt = alIt->second->songs.begin(); sIt != alIt->second->songs.end(); sIt++) {
				cout << "                " << sIt->first << ". " << sIt->second->title << ": " << intToTime(sIt->second->time) << endl; // Songs
				delete sIt->second;
			}
			delete alIt->second;
		}
		delete artIt->second;
	}

	




	return 0;
}
