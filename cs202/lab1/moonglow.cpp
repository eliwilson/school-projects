/*
Eli Wilson
8/25/2021
Lab 1: Reads a file containing student names and test scores and outputs the name and final grade
*/
#include <iostream>
#include <string>

using namespace std;

int main()
{
    double score;
    double total = 0;
    int avgCtr = 0;
    double avg;
    string str;
    bool avgMode;
    bool nameMode;
    string name;

    while(true){
        if(cin >> score){ // checks for number input
            if(avgMode){ // checks if input is a score to be averaged
                avg += score;
                avgCtr++;
            }else{
                total += score;
            }
        }else{
            cin.clear();
            if (avgMode){ // checks if at the end of a string of averages, adds total, resets counter
                if(avgCtr > 0){ // prevents /0 error
                    total += (avg/avgCtr);
                }
                avgCtr = 0;
                avg = 0;
                avgMode = false;
            }
            if(cin >> str){
                if (str == "AVERAGE"){
                    avgMode = true;
                }else if(str == "NAME"){
                    nameMode = true;
                }else if(nameMode){
                    name = str;
                    nameMode = false;
                }
            }
            
        }

        if(cin.eof()){
            cout << name << " " << total << endl;
            break;
        }
    }

    
    return 0;
}