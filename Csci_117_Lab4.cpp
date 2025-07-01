// Lab 4, Interpreter, Efrain Diaz Valdez
///////////////////
//// key code for Prog2.
//// 
//// $> g++ p2-key-for-students.cpp -o p2
//// $> ./p2  p2-data
///////////////////

#include <cstdlib> //for atoi()
#include <iostream>
#include <fstream>
#include <string>
#include <vector> // for symbol table
#include <cctype> // for isdigit()
#include <math.h> // for pow(..)
#include <climits> // for placeholder

using namespace std;

// global variables
string prog; //prog for reading the entire input program (expression) from file
int indexx = 0;  //global index for program string

// symbol table definitions
struct Symbol {
  char id;
  string type;
  int val;
};

vector<Symbol> symbolTable; // actual symbol table

// prototype functions declarations
void Declarations(), Declaration(string word), Statements(), Statement(string word), AssignSt(string word), PrintSt();
void skipSpaces();
string readWord();
char readChar();
int findSymbolIndex(char id); // returns table index
int Exp(), Term(), Exp2(int), Term2(int), Fact(), Fact2(int), Num();

// skipsSpaces
void skipSpaces() {
  while (isspace(prog[indexx])) indexx++; // Skip whitespace
}

// read word until a space is met
string readWord() {
  string word;
  while (indexx < prog.length() && !isspace(prog[indexx])) { // isspace also treats '\n' as space and skips
    word += prog[indexx++];
  }
  return word;
}

// read a character skips spaces
char readChar() {
  if (indexx < prog.length())   //if not the end of program string
    { 
      char id = prog.at(indexx++); //get one chr from program string
      while (id == ' ' && (indexx < prog.length())) //if space, skip and read until a char
      {
        id = prog.at(indexx++);
      }
      return id;
    } else {
      cerr << "Error: unexpected end of input!" << endl;
      exit(1);
    }
}

// returns table Index of id if found -1 otherwise
int findSymbolIndex(char id) {
  for (int i = 0; i < symbolTable.size(); ++i) {
    if (symbolTable[i].id == id) {
      return i; // id found, return table index
    }
  }
  return -1; // not found
}

// <Prog> ::= program <Declarations> begin <Statements> end
int main()
{ 
  ifstream file("input.txt");
  string line; // helper

  // file opening
  if(!file) {
    cerr << "Error, file did not open!" << endl;
    exit(1);
  } 
    
  // reading file
  while(getline(file, line)) {
    // adding the '\n' everytime a line has been read
    prog += line + ' '; // Concatenate lines without modifying input, doesn't include '\n'
  }
  file.close();

  // Read the first word
  // should be keyword 'program'
  string word;
  skipSpaces();
  word = readWord();

  // Check if the first word is "program"
  if (word == "program") { // succesfully handles program and not program
    Declarations();
    Statements();
  } else {
    cerr << "Syntax error: 'program' keyword expected." << endl;
    exit(1);
  }

  //cout<<"result= "<<Exp()<<endl;

  return 0;
}

// <Declarations> ::= <Declaration> | <Declaration><Declarations> | ε
void Declarations() { // successfully gets here from the main
  // Read a word
  string word;
  skipSpaces();
  word = readWord();

  // Check if the word is "begin"
  if (word == "begin") { // not handling 'begin' keyword, do we need to?
    return; // Exit the function
  } else {
    // Handle 'int' or 'double' declarations
    Declaration(word);
    // Call declarations recursively
    Declarations();
  }
}

// <Declaration> ::= <Type><Id-list>; // handled!
void Declaration(string word) { // type

  // int or double? otherwise syntax error
  if (word != "int" && word != "double") {
    cerr << "Syntax error: 'int' or 'double' expected type!" << endl;
    exit(1);
  }
  string type = word;
  char id = readChar(); // reading a char, skips leading spaces

  // id has to be a valid letter
  if (!isalpha(id)) {
    cerr << "Lexical error: valid variable name expected." << endl;
    exit(1);
  }

  // adding first id, type to our symbol table
  int placeholder = INT_MIN; // placeholder for unitialized variable
  symbolTable.push_back({id, type, placeholder}); // for now 0, need to change
  cout << "Added: " << id << " of type " << type << " with placeholder " << placeholder << endl; // for debugging
  id = readChar(); // get next char before loop, if ';' then we don't loop at all

  // Will probably rewrite since this can end up being an infinite loop
  // This is a note for my eyes only => Handle one case: single variable declaration, add to table; then multiple declaration case: check if ',' loop, then read nextChar if ';', could also have variable flag as well, brainstorm.
  while (id != ';') { // looping to check if there are anymore declarations
    if (id == ',') { // skip ','
    }
    else if (isalpha(id)) {
      symbolTable.push_back({id, type, placeholder}); // need a placeholder to use for uninitialized variable currently 0
      cout << "Added: " << id << " of type " << type << " with placeholder " << placeholder << endl;
    } else {
      cerr << "Lexical error: valid variable name expected." << endl;
      exit(1);
    }
    id = readChar(); // read next char
  } // while ends
}

// <Statements> ::= <Statement><Statements> | ε
void Statements() {
  // implement parsing for multiple statements
  // Example: a = 2*(5+200); print a;
  skipSpaces();
  string word = readWord();

  if (word == "end") { // handle empty case
    return; // exit the function
  } else {
    Statement(word);
    Statements(); // Recursively call
  }
}

// <Statement> ::= <Assign-St> | <Print-St>
void Statement(string word) {
  if (word == "print") {
    PrintSt();
  } else {
    AssignSt(word);
  }
}

// I think it's handled, test without using printSt for now
// <Assign-St> ::= <Id>=<Exp>; | <Id>=<Id>;
void AssignSt(string word) { // word => id
  // Implement parsing for assignment statements
  // Example: a = 2*(5+200);
  cout << "AssignSt word= " << word << endl;
  if (word.size() != 1 || !isalpha(word[0])) {
    cerr << "Syntax error: single valid variable name expected." << endl;
    exit(1);
  }
  char id = word[0];

  // Assuming the next char is '='
  char nextChar = readChar();
  if (nextChar != '=') {
    cerr << "Syntax error: '=' expected." << endl;
    exit(1);
  }
  // nextChar is '=', indexx updated to point to expression
  // reading next char to check if it's alpha, if so check for unitialized placeholder
  char nextId = readChar(); // reading after '='
  int temp; // local storage (.val or Exp later)
  if (!isalpha(nextId) && !isdigit(nextId) && nextId == '*') {
    cerr << "Lexical Error: invalid assignment using '" << nextId << "'." << endl;
    exit(1);  
  }
  if (isalpha(nextId)) { // 1st lvl if/else
    // It's a variable, so we need to find its value
    int index = findSymbolIndex(nextId); // search symbol, get index
    if (index != -1) { // found symbol, 2nd lvl if
      if (symbolTable[index].val == INT_MIN) { // 3rd lv inside if
        cerr << "Semantic error: variable '" << nextId << "' is uninitialized." 
             << "Trying to assign to variable '" << id << "'." << endl;
        exit(1);
      } // 3rd level inside if
      char lastChar = readChar(); // expecting ';'
      cout << "lastChar= " << lastChar << "." << endl;
      // if not ';' syntax error
      if (lastChar != ';') {
        cerr << "Syntax error: expected a ';' after initializing variable '" << id << "'.";
        exit(1);
      }
      temp = symbolTable[index].val; // correct value
      cout << "Variable source id '" << nextId << "' = " << symbolTable[index].val << endl; // debugging
      // go find target id to assigned source's value
      int targetIndex = findSymbolIndex(id); // target's indexx
      symbolTable[targetIndex].val = temp; // target = source
      cout << "Variable target id '" << id << "' = " << symbolTable[targetIndex].val << endl;
    } else { // 2nd lvl inside if/else
        cerr << "Semantic error: variable '" << nextId << "' not found." << endl;
        exit(1);
    } // 2nd lvl if/else ends
  } else { // 1st lvl if/else
    // It's a number, we need to evaluate expression
    indexx--; // adjusting a position back to get to the correct indexx, modified at work
    temp = Exp(); // Adjust to properly evaluate the expression
    indexx--; // adjusting
    char lastChar = readChar();
    //cout << "lastChar= " << lastChar << endl;
    int localIndex = findSymbolIndex(id); // searching for tIndex
    if (localIndex != -1) {
      symbolTable[localIndex].val = temp;
      cout << "Variable '" << id << "' = " << symbolTable[localIndex].val << endl;
    } else {
      cerr << "Semantic error: variable '" << id << "' not found." << endl;
      exit(1);
    }
  }
}

// <Print-St> ::= print<Id>; | print<Exp>;
void PrintSt() {
  // Implement parsing for print statements
  // Example: print a; or print expression;

  char id = readChar(); // could be id or expression
  if (isalpha(id)) {
    char nextChar = readChar(); // to get ';'
    if (nextChar != ';') { // 2nd lvl if
      cerr << "Syntax error: missing ';' after variable '" << id << "'." << endl;
      exit(1);
    } // 2nd lvl if
    int tableIndex = findSymbolIndex(id);
    if (tableIndex != -1) { // 2nd lvl if
      if (symbolTable[tableIndex].val != INT_MIN) { // 3rd lvl inside if
        cout << symbolTable[tableIndex].val << endl; // print value of declared variable
      } else {
          cerr << "Semantic error: trying to print unitialized variable '" << id << "'" << endl;
          exit(1); 
      } // 3rd lvl inside if ends
    } else {
        cerr << "Semantic error: trying to print uninitialized variable '" << id << "'" << endl;
        exit(1);
    } // 2nd lvl if ends
  } else {
      // expression
      indexx--; // get back one position "readjusting"
      int temp = Exp(); // get expression
      indexx--; // readjusting
      // get next char to check for ';' otherwise invalid
      int nextChar = readChar();
      if (nextChar != ';') {
        cerr << "Syntax error: missing ';' at the end of expression = " << temp << "." << endl;
        exit(1);
      }
      cout << "Expression = " << temp << endl;
  } // 1st lvl if ends
}

// <Type> ::= int | double

// <Id-list> ::= <Id> | <Id>,<Id-list>

// <Id> ::= a|b|c|...|z|A|B|C|...|Z

int Exp()
{ return Exp2(Term()); 
}

int Term()
{ return Term2(Fact()); 
}

int Exp2(int inp)
{   
  int result = inp;
  if (indexx < prog.length()) //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '+')
      result = Exp2(result + Term());  //handles T+T, where T represents consecutive * or /
    else if (a == '-')                 //analogous for - operation
      result = Exp2(result - Term());
  }
  return result;
}

int Term2(int inp)
{   
  int result = inp;
  if (indexx < prog.length())   //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '*')
      result = Term2(result * Fact()); //handles consecutive * operators
    else if (a == '/')
      result = Term2(result / Fact()); //handles consecutive / operators
    else if (a == '+' || a == '-' || a == ')' || a == ';') //if + or -, get back one position
      indexx--;
  }
  return result;
}

int Fact()
{ return Fact2(Num());
}

int Fact2(int inp)
{ 
  int result = inp;
  if (indexx < prog.length())   //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '^')
      result = Fact2(pow(result, Fact()));
    else //this includes ')'
      indexx--; 
  }
  return result;
}

// Handles multiple digits as well
int Num()
{ 
  if (indexx < prog.length())
  { 
    char a = prog.at(indexx++);
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);

    if (a == '(')
      return Exp();
    
    // Read the entire number
    string numStr; // this is initialized to "", if not 'digit' skips code block
    while (isdigit(a) && (indexx < prog.length())) {
      numStr += a;
      if (indexx < prog.length()) {
        a = prog.at(indexx++);
      } else {
        break;
      }
    }
    // numStr is still empty, 
    // Move back one position if the last character read is not a digit
    if (!isdigit(a)) {
      indexx--;
    }

    return atoi(numStr.c_str()); // converts the string to a numeric number and return
  } else {
    cerr << "Invalid Expression!" << endl;
    exit(1);
  }
  //return 0;
}