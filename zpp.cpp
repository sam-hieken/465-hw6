#include "zpp.h"
#include <sstream>
#include <regex>

interpreter::interpreter() {}

interpreter::interpreter(map<string, value> variables) {
    this->variables = variables;
}

bool interpreter::interpret(string& line) {
    ltrim(line);
    rtrim(line);
		
    // Splits the line by spaces and stores it in a list.
    vector<string> div;
    istringstream iss(line);
    string word;

    // Splitting the string by whitespace and storing in vector
    while (iss >> word)
        div.push_back(word);

    // No valid line has less than two tokens.
    if (div.size() < 2) return false;
    
    const string operation = div[0];

    if (operation == "FOR")
        return loop(div);
        
    else if (operation == "PRINT")
        return print(div[1]);
        
    return setVariable(line);
}

bool interpreter::loop(vector<string> line) {
    // Error checks

    const string times = line[1];
    const string endFor = line[line.size() - 1];
    
    // Runtime error, either number not specified, or endfor not specified.
    if (!isNumeric(times) || !(endFor == "ENDFOR"))
        return false;

    // Stores all of the lines of code inside the loop.
    string loopContent;
    for (auto it = line.begin() + 2; it != line.end() - 1; ++it)
        loopContent += (it != line.begin() + 2 ? " " : "") + *it;
    
    // Actual loop logic
    
    return loop(loopContent, atoi(times.c_str()));
}

bool interpreter::loop(string& loopContent, int i) {
    regex pattern("(?<=;)(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
    sregex_token_iterator it(loopContent.begin(), loopContent.end(), pattern, -1); 
    vector<std::string> resultArray(it, sregex_token_iterator()); 
    
    for (int j = 0; j < i; j++) 
        for (string line : resultArray)
            if (!interpret(line))
                return false;
        
    return true;
}

bool interpreter::setVariable(string& line) {
    if (!str_ends_with(line, " ;"))
        return false;
    
    // Split string by space
    vector<string> div; 
    istringstream iss(line);
    string word;

    while (iss >> word)
        div.push_back(word);
    
    // Need a name, operation, and value at minimum for assignment.
    if (div.size() < 3)
        return false;

    optype op;

    if (div[1] == "=")
        op = optype::SET;

    else if (div[1] == "+=")
        op = optype::ADD;
        
    else if (div[1] == "-=") 
        op = optype::SUBTRACT;
        
    else if (div[1] == "*=")
        op = optype::MULTIPLY;

    else return false;

    return setVariable(line, op);
}

bool interpreter::setVariable(string& line, optype& oper) {
    vector<string> div;
    istringstream iss(line);
    string word;

    while (iss >> word)
        div.push_back(word);
    
    string name = div[0];
    string op = div[1];
    
    // div[2] isn't sufficient; if it's a string literal, could
    // have spaces in it.
    const unsigned n = line.find(op, 0) + op.length();
    const unsigned m = line.find_last_of(" ;");

    string val = line.substr(
            // Get's the part of string between operator and line end.
            n, m - n);

    ltrim(val);
    rtrim(val);
    
    // String
    if (str_starts_with(val, "\"") && str_ends_with(val, "\""))
        return setVariable(name, value(val.substr(1, val.length() - 2)), oper);
    
    // Integer
    else if (isNumeric(val))
        return setVariable(name, value(atoi(val.c_str())), oper);
    
    // Not int or String, so we treat as variable.
    
    value variable = this->variables[val];
    
    // Variable passed as value not declared; runtime error.
    if (!this->variables.count(val)) return false;
    
    return setVariable(name, variable, oper);
}

bool interpreter::setVariable(string& name, value val, optype& oper) {
    const bool isNumber = val.getType() == VAL_INT;
    
    // Ensure value is either a string or int.
    if (!isNumber && !(val.getType() == VAL_STRING))
        return false;
    
    // No checks needed; variables can change datatypes
    else if (oper == optype::SET) {
        this->variables[name] = val;
        return true;
    }
    
    value prevValue = this->variables[name];
    const bool prevIsNumber = prevValue.getType() == VAL_INT;
    
    // No value assigned to variable already; cannot operate 
    // on it, runtime error.
    if (!this->variables.count(name)) return false;
    
    // Different datatypes; cannot operate on it, runtime error.
    else if (prevIsNumber != isNumber) return false;
    
    switch (oper) {
    
    case optype::ADD:
        variables[name] = isNumber 
                ? value(*prevValue.getInt() + *val.getInt()) 
                : value(*prevValue.getString() + *val.getString());
        return true;
        
    case optype::SUBTRACT:
        // Can't subtract strings
        if (!isNumber) return false;
        variables[name] = *prevValue.getInt() - *val.getInt();
        return true;
        
    case optype::MULTIPLY:
        if (!isNumber) return false;
        variables[name] = *prevValue.getInt() * *val.getInt();
        return true;
    
    }

    // Should never reach this point
    cerr << "Severe error occurred." << endl;
    return false;
}

bool interpreter::print(string& variableName) {
    value val = variables[variableName];
    
    if (!this->variables.count(variableName)) 
        return false;
    
    const unsigned type = val.getType();

    cout << variableName << "=";

    if (type == VAL_INT)
        cout << *val.getInt();

    else cout << *val.getString();
    
    cout << endl;
    return true;
}

bool isNumeric(string str) {
    regex pattern("-?\\d+(\\.\\d+)?"); 

    return regex_match(str, pattern);  //match a number with optional '-' and decimal.
}

bool str_ends_with(const string& fullString, const string& ending) {
    if (fullString.length() >= ending.length())
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    
    return false;
}

bool str_starts_with(const string& fullString, const string& prefix) {
    return fullString.rfind(prefix) == 0;
}

void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}