#include <iostream>
#include "optype.h"
using namespace std;

int main() {
    string s { "hello world" };
    
    cout << s.substr(1, 7) << endl;
}