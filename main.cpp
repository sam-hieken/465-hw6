// => I'm competing for BONUS Points <=

#include <fstream>

#include "zpp.h"

using namespace std;

int run(string);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Error: Only 1 argument allowed (ZPM file path)." << endl;
        return 1;
    }

    return run(string(argv[1]));
}

// Run the ZPM file.
// Returns the program's stauts code; 0 if successful, -1 if error.
int run(string filename) {
    interpreter interp;
    ifstream file(filename);
    
    string line;
    int lineNumber = 0;
    
    while (getline(file, line)) {
        ++lineNumber;
        
        // Skip if just a blank line.
        if (line.empty() || line.find_first_not_of(' ') == std::string::npos) 
            continue;
        
        else if (!interp.interpret(line)) {
            cerr << "RUNTIME ERROR: line " << lineNumber << endl;
            return -1;
        }

        // cout << line << endl << endl;
    }
    
    file.close();
    return 0;
}