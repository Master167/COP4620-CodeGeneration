/* 
 * File:   LexicalAnalyzer.h
 * Author: Michael Frederick (n00725913)
 *
 * Created on February 20, 2017, 5:48 PM
 */

#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <string>
#include <iostream>
#include <new>
#include <cctype>
#include <sstream>
#include "SymbolTable.h"
#include "Symbol.h"

#define INT_TO_STRING( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class LexicalAnalyzer {
public:
    LexicalAnalyzer(std::fstream& inputFile, std::string filename);
    bool isReady();
    bool scanFile();    
    std::string outputFilename;
private:
    // Class Variables
    char currentCharacter;
    int currentLineIndex;
    int scope;
    std::fstream& currentFile;
    std::string currentLine;
    char* specialCharacters;
    std::string* keywords;

    // Class functions
    bool analyzeDigit();
    bool analyzeLetter();
    bool analyzeSpecialCharacter();
    bool writeToFile(std::string outputLine);
    
    bool moveToNextCharacter();
    bool moveToNextline();
    char peekAtNextCharacter();
    bool searchKeyword(std::string search);
    bool searchSpecialCharacter(char search);

    bool errorLine(std::string str);
};

#endif /* LEXICALANALYZER_H */
