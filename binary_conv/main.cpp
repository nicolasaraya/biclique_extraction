#include <iostream>

#include "binary_converter.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        cout << "Uso: ./exec flag file" << endl;     
        return 0; 
    }

    if (string(argv[1]) == "1") {
        text2binary(argv[2]);
    } else if (string(argv[1]) == "2"){
        binary2text(argv[2]);
    } else {
        cout << "1: text2binary" << endl << "2: binary2text" << endl; 
    }

    return 0;
}
