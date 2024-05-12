#include <map>
#include <vector>
#include <iostream>

#include "optype.h"
#include "value.cpp"

using namespace std;

bool isNumeric(string);

bool str_ends_with(const string&, const string&);
bool str_starts_with(const string&, const string&);

void ltrim(string&);
void rtrim(string&);

class interpreter {
private:
    map<string, value> variables;

public:
    interpreter();
    interpreter(map<string, value>);

    bool interpret(string&);

private:
    bool loop(vector<string>&);
    bool loop(string&, int);

    bool setVariable(string&);
    bool setVariable(string&, optype&);
    bool setVariable(string&, value, optype&);

    bool print(string&);
};