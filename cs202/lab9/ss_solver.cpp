/* Eli Wilson
 * Lab 9
 * Finds Solution to Shapeshifter game from input using recursion
 */
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class Shifter {
  public:

    /* Read_Grid_And_Shapes() initializes the grid from argc/argv, and the
       reads from standard input to get the shapes. */

    bool Read_Grid_And_Shapes(int argc, const char **argv);

    /* Apply_Shape() applies the shape in Shapes[index] to the grid,
       starting at row r and column c.  You'll note, if you call
       Apply_Shape twice with the same arguments, you'll end up 
       with the same grid as before the two calls. */

    void Apply_Shape(int index, int r, int c);

    /* Find_Solution() is the recursive procedure.  It tries all possible
       starting positions for Shape[index], and calls Find_Solution() 
       recursively to see if that's a correct way to use Shape[index].
       If a recursive call returns false, then it "undoes" the shape by
       calling Apply_Shape() again. */

    bool Find_Solution(int index);

    /* This prints the solution on standard output. */

    void Print_Solution() const;

  protected:

    /* This is the grid.  I have this be a vector of 0's and 1's, because it makes
       it easy to print the grid out. */

    vector <string> Grid;               

    /* These are the shapes, in the order in which they appear in standard input. */

    vector < vector <string> > Shapes;     

    /* These two vectors hold the starting positions of the shapes, both as you
       are finding a solution, and when you are done finding a solution. */

    vector <int> Solution_Rows;            
    vector <int> Solution_Cols;            
};

int main(int argc, const char **argv){

	Shifter *game = new Shifter;

   game->Read_Grid_And_Shapes(argc, argv);
   if(game->Find_Solution(0)){
      game->Print_Solution();
   }

   (void) argc;
   delete game;
	return 0;
}

bool Shifter::Read_Grid_And_Shapes(int argc, const char **argv){
   istringstream ss;
	string g; // line of grid
   int line;
   int shape;
   string s; // shape with spaces between rows

   // stores cmd line args in Grid
   for(int i = 1; i < argc; i++){
      ss.str(argv[i]);
      ss >> g;
      Grid.push_back(g);
      ss.clear();
   }

   // sorts lines of input into vector of shapes
   line = 0;
   while(getline(cin, s)){
      Shapes.resize(Shapes.size()+1);
      shape = 0;
      Shapes[line].push_back("");
      for(size_t i = 0; i < s.size(); i++){
         if(s[i] != ' '){ // adds bit to shape
            Shapes[line][shape].push_back(s[i]);
         }else{ // moves to next row of shape
            shape++;
            Shapes[line].push_back("");
         }
      }
      line++;
   }

   Solution_Rows.resize(Shapes.size());
   Solution_Cols.resize(Shapes.size());

   return true;

}

void Shifter::Apply_Shape(int index, int r, int c){
   // iterates through each bit of shape and changes corresponding element of grid
   for(size_t i = 0; i < Shapes[index].size(); i++){
      for(size_t j = 0; j < Shapes[index][i].size(); j++){
            if(Shapes[index][i][j] == '1'){
               if(Grid[r+i][c+j] == '1'){
                  Grid[r+i][c+j] = '0';
               }else{
                  Grid[r+i][c+j] = '1';
               }
            }
      }
   }
}

bool Shifter::Find_Solution(int index){// ADD BASE CASE
   bool solved; // matrix full of 1s

   // Applies shape in every valid position
   for(size_t i = 0; i < Grid.size()-Shapes[index].size()+1; i++){
      for(size_t j = 0; j < Grid[0].size()-Shapes[index][0].size()+1; j++){
         this->Apply_Shape(index, i, j);
         
         // Checks for solved puzzle (base case)
         solved = true;
         for(size_t k = 0; k < Grid.size(); k++){
            for(size_t l = 0; l < Grid[0].size(); l++){
               if(Grid[k][l] == '0'){
                  solved = false;
               }
            }
         }
         if(solved){
            Solution_Rows[index] = i;
            Solution_Cols[index] = j;
            return true;
         }

         // Runs next Find_Solution if not at last index
         // Resets change if it fails
         if((unsigned)index+1 != Shapes.size()){
            if(!this->Find_Solution(index+1)){
               this->Apply_Shape(index, i, j);
            }else{
               Solution_Rows[index] = i;
               Solution_Cols[index] = j;
               return true;
            }
         }else{
            this->Apply_Shape(index, i, j);
         }
      }
   }

   return false;
}

void Shifter::Print_Solution() const{ //prints shape row col
	for(size_t i = 0; i < Shapes.size(); i++){
      for(size_t j = 0; j < Shapes[i].size(); j++){
         cout << Shapes[i][j] << " ";
      }
      cout << Solution_Rows[i] << " " << Solution_Cols[i] << endl;
   }
}
