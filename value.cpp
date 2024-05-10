#include <cstring>
#include <string>

#define VAL_STRING 1
#define VAL_INT 0
// Currently, VAL_NULL is just a placeholder since we need a no args
// constructor for map<string, value> to work. I just checked for existing
// keys in the map instead since I made this later on.
#define VAL_NULL 100

using namespace std;

class value {
private:
    string* str_address;
    int* int_address;
    unsigned type;

public:
    value() {
        this->type = VAL_NULL;
    }

    value(const string& value) {
        this->str_address = new string(value);
        this->type = VAL_STRING;
    }

    value(int value) {
        this->int_address = new int(value);
        this->type = VAL_INT;
    }

    value(const value& other) {
        copy(other);
    }

    value& operator=(const value& other) {
        copy(other);

        return *this;
    }

    ~value() {
        if (this->type == VAL_STRING)
            delete this->str_address;

        else
            delete this->int_address;
    }

    // Returns the string value, or nullptr if the value is not a string.
    string* getString() {
        if (this->type == VAL_STRING)
            return this->str_address;
        else
            return nullptr;
    }

    // Returns the int value, or nullptr if the value is not an int.
    int* getInt() {
        if (this->type == VAL_INT)
            return this->int_address;

        return nullptr;
    }

    unsigned getType() {
        return this->type;
    }

    void copy(const value& other) {
        if (other.type == VAL_STRING) {
            this->str_address = new string(*other.str_address);
            this->type = VAL_STRING;
        }

        else {
            this->int_address = new int(*other.int_address);
            this->type = VAL_INT;
        }
    }
};