/* 
 * File:   main.cpp
 * Author: Michael Frederick (n00725913)
 *
 * Created on Febuary 20, 2017, 2:41 PM
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "LexicalAnalyzer.h"
#include "SymbolTable.h"
#include "Parser.h"

/*
 * Main Starting point of the program
 */
int main(int argc, char** argv) {
    if (argc > 1) {
        bool continueCompile = false;
        std::string filename = argv[1];
        std::fstream inputFile(argv[1]);
        if (inputFile.is_open()) {
            SymbolTable* symTab = new SymbolTable(100);
            LexicalAnalyzer* lexy = new LexicalAnalyzer(inputFile, filename);
            if (lexy->isReady()) {
                continueCompile = lexy->scanFile();
                if (continueCompile) {
                    // Transition from input to MY temp file
                    filename.append(".temp"); // .append() adds to the same string in memory and returns a point to that string
                    std::fstream tempFilestream(filename.c_str());
                    inputFile.flush();
                    inputFile.close();
                    Parser* percy = new Parser(tempFilestream);
                    continueCompile = percy->parseFile(symTab);
                    delete percy;
                    tempFilestream.flush();
                    tempFilestream.seekg(0, std::ios::beg);
                    
                    if (continueCompile) {
                        std::cout << "ACCEPT" << std::endl;
                    }
                    else {
                        std::cout << "REJECT" << std::endl;
                    }
                    
                    tempFilestream.close();
                }
                else {
                    std::cout << "REJECT" << std::endl;
                }
                //symTab->printTable();
            }
            else {
                std::cout << "Unable to allocate memory for compiler";
            }
            delete lexy;
            delete symTab;
            remove(filename.c_str());
        }
        else {
            std::cout << "Bad Input file" << std::endl;
        }
    }
    else {
        std::cout << "No Input file given" << std::endl;
    }
    return 0;
}

