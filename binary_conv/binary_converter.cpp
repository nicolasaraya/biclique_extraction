#include "binary_converter.hpp"

using namespace std;

void text2binary(const string path){
    //cout << path << endl; 
    string binaryfile = path; 
    do {
        binaryfile.pop_back();
    } while(binaryfile.back() != '.');

    ofstream binFile;
    binFile.open(binaryfile + "bin", std::ios::out | std::ios::binary | std::ios::trunc);
    ifstream textFile; 
    textFile.open(path);

    assert(binFile.is_open());
    assert(textFile.is_open());

    string line; 
    getline(textFile, line); //skip first line
    while(not textFile.eof()){
        getline(textFile, line);
        auto elements = splitString(line, " ");
        
        if (elements.size() <= 0) {
            continue;
        }
        var id = atoi(elements[0].c_str()) * -1; 

        binFile.write((char*)&id, sizeof(var));
        //binFile << atoi(elements[0].c_str()) * -1;
        for (size_t i = 1; i < elements.size(); i++) {
            id = atoi(elements[i].c_str());
            binFile.write((char*)&id, sizeof(var));
        }
        //if(temp++ == 100) break;
    }

    binFile.close();
    textFile.close();

    cout << "Escribiendo: " << binaryfile+"bin" << endl;
}

void binary2text(const string path){
    string textFilePath = path; 
    do {
        textFilePath.pop_back();
    } while(textFilePath.back() != '.');

    ifstream binFile; 
    binFile.open(path, ios::in | ios::binary);
    ofstream textFile;
    textFile.open(textFilePath + "txt_test", std::ios::out | std::ios::trunc);


    assert(binFile.is_open());
    assert(textFile.is_open());
    
    unsigned long long nodes = 0; 
    unsigned long long edges = 0;
    textFile << "                                  ";
    var* buffer = new var(0); 
    while(not binFile.eof()) {
        binFile.read( (char*)buffer, sizeof(var));
        //cout << *buffer << endl;
        if(*buffer < 0) {
            textFile << endl <<*buffer * -1 << ":";
            nodes++;
        }
        else {
            textFile << " " << *buffer;
            edges++; 
        }
    }
    textFile.seekp(0);
    textFile << nodes << " " << edges;
    binFile.close();
    textFile.close();
}

std::vector<std::string> splitString(std::string line, std::string delims)
{
    std::string::size_type bi, ei;
    std::vector<std::string> words;
    bi = line.find_first_not_of(delims);
    while (bi != std::string::npos)
    {
        ei = line.find_first_of(delims, bi);
        if (ei == std::string::npos)
            ei = line.length();
        std::string aux = line.substr(bi, ei - bi);
        words.push_back(aux.c_str());
        bi = line.find_first_not_of(delims, ei);
    }
    return words;
}