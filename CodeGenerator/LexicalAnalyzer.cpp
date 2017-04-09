/* 
 * File:   LexicalAnalyzer.cpp
 * Author: Michael Frederick (n00725913)
 * 
 * Created on February 20, 2017, 5:48 PM
 */

#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::fstream& inputFile, std::string filename) :
    currentFile(inputFile) {
    // Initialize special characters and keywords
    this->specialCharacters = new (std::nothrow) char[16];
    this->keywords = new (std::nothrow) std::string[7];
    this->currentLineIndex = -1;
    this->scope = 0;
    if (this->isReady()) {
        this->outputFilename = filename.append(".temp");
        // Manually loading the keywords and specialCharacters
        this->keywords[0] = "if";
        this->keywords[1] = "int";
        this->keywords[2] = "else";
        this->keywords[3] = "void";
        this->keywords[4] = "float";
        this->keywords[5] = "while";
        this->keywords[6] = "return";
        this->specialCharacters[0] = '!';
        this->specialCharacters[1] = '(';
        this->specialCharacters[2] = ')';
        this->specialCharacters[3] = '*';
        this->specialCharacters[4] = '+';
        this->specialCharacters[5] = ',';
        this->specialCharacters[6] = '-';
        this->specialCharacters[7] = '/';
        this->specialCharacters[8] = ';';
        this->specialCharacters[9] = '<';
        this->specialCharacters[10] = '=';
        this->specialCharacters[11] = '>';
        this->specialCharacters[12] = '[';
        this->specialCharacters[13] = ']';
        this->specialCharacters[14] = '{';
        this->specialCharacters[15] = '}';
    }
}

bool LexicalAnalyzer::isReady() {
    return (this->keywords != NULL) && (this->specialCharacters != NULL);
}

bool LexicalAnalyzer::scanFile() {
    bool results = true;
    bool continueLoop = true;
    if (this->isReady()) {
        while (this->moveToNextCharacter()) {
            //Check what the character is and decide
            if (std::isalpha(this->currentCharacter)) {
                this->analyzeLetter();
            }
            else if (std::isdigit(this->currentCharacter)) {
                this->analyzeDigit();
            }
            else if (std::ispunct(this->currentCharacter)){
                this->analyzeSpecialCharacter();
            }
        }
    }
    else {
        results = false;
    }
    return results;
}

// Private functions
bool LexicalAnalyzer::analyzeDigit() {
    bool result = true;
    bool foundTerm = false;
    bool foundError = false;
    bool isFloat = false;
    std::string number = "";
    number.append(1, this->currentCharacter);
    char peekCharacter;
    // Get Numbers
    while (!foundTerm) {
        peekCharacter = this->peekAtNextCharacter();
        if (peekCharacter != '$' && peekCharacter != '.' && peekCharacter != ' '  && peekCharacter != 'E') {
            if (std::isdigit(peekCharacter)) {
                this->moveToNextCharacter();
                number.append(1, this->currentCharacter);
            }
            else if (this->searchSpecialCharacter(peekCharacter)) {
                foundTerm = true;
            }
            else {
                number.append(1, peekCharacter);
                foundError = true;
                foundTerm = true;
            }
        }
        else {
            // Reached end of line
            foundTerm = true;
        }
    }

    // Check for floating point
    if (peekCharacter == '.' && !foundError) {
        if (this->currentLine.length() > this->currentLineIndex + 2) {
            isFloat = true;
            // String is long enough
            peekCharacter = this->currentLine[this->currentLineIndex + 2];
            if (std::isdigit(peekCharacter)) {
                this->moveToNextCharacter();
                number.append(1, this->currentCharacter);
                this->moveToNextCharacter();
                number.append(1, this->currentCharacter);
                foundTerm = false;
                while (!foundTerm) {
                    peekCharacter = this->peekAtNextCharacter();
                    if (std::isdigit(peekCharacter)) {
                        this->moveToNextCharacter();
                        number.append(1, this->currentCharacter);
                    }
                    else {
                        foundTerm = true;
                    }
                }// end while
            }
            else {
                foundError = true;
            }
        }
        else {
            foundError = true;
        }
    }

    // Check for scientific notation
    if (peekCharacter == 'E' && !foundError) {
        if (this->currentLine.length() > this->currentLineIndex + 2) {
            isFloat = true;
            // String is long enough
            peekCharacter = this->currentLine[this->currentLineIndex + 2];
            if (std::isdigit(peekCharacter)) {
                //Move Twice
                this->moveToNextCharacter();
                number.append(1, this->currentCharacter);
                this->moveToNextCharacter();
                number.append(1, this->currentCharacter);
                foundTerm = false;
                while (!foundTerm) {
                    peekCharacter = this->peekAtNextCharacter();
                    if (std::isdigit(peekCharacter)) {
                        this->moveToNextCharacter();
                        number.append(1, this->currentCharacter);
                    }
                    else {
                        foundTerm = true;
                    }
                }// end while
            }
            else if ((peekCharacter == '-' || peekCharacter == '+') && this->currentLine.length() > this->currentLineIndex + 3) {
                peekCharacter = this->currentLine[this->currentLineIndex + 3];
                if (std::isdigit(peekCharacter)) {
                    // Move three times
                    this->moveToNextCharacter();
                    number.append(1, this->currentCharacter);
                    this->moveToNextCharacter();
                    number.append(1, this->currentCharacter);
                    this->moveToNextCharacter();
                    number.append(1, this->currentCharacter);
                    foundTerm = false;
                    while (!foundTerm) {
                        peekCharacter = this->peekAtNextCharacter();
                        if (std::isdigit(peekCharacter)) {
                            this->moveToNextCharacter();
                            number.append(1, this->currentCharacter);
                        }
                        else {
                            foundTerm = true;
                        }
                    }// end while
                }
                else {
                    foundError = true;
                }
            }
            else {
                foundError = true;
            }
        }
        else {
            foundError = true;
        }
    }
    
    if (foundError) {
        if (std::isdigit(this->currentCharacter)) {
            this->moveToNextCharacter();
        }
        result = this->errorLine(number);
    }
    else if (number.length() > 0) {
        if (isFloat) {
            //std::cout << "FLOAT: " << number << std::endl;
            this->writeToFile("FLOAT: " + number + "\n");
        }
        else {
            //std::cout << "INT: " << number << std::endl;
            this->writeToFile("INT: " + number + "\n");
        }
    }

    return result;
}

bool LexicalAnalyzer::analyzeLetter() {
    bool result = false;
    bool foundTerm = false;
    std::string identifier = "";
    identifier.append(1, this->currentCharacter);
    char peekCharacter;
    while (!foundTerm) {
        peekCharacter = this->peekAtNextCharacter();
        if (peekCharacter != '$') {
            if (std::isalpha(peekCharacter)) {
                this->moveToNextCharacter();
                identifier.append(1, this->currentCharacter);
            }
            else {
                foundTerm = true;
            }
        }
        else {
            // Reached end of line
            // I need to analyze what I have
            foundTerm = true;
        }
    }
    // peekCharacter could be a delimiter, digit, or special character
    if ((std::isdigit(peekCharacter) | std::ispunct(peekCharacter)) & !std::isspace(peekCharacter)) {
        result = this->errorLine(identifier);
    }
    else if (this->searchKeyword(identifier)) {
        //std::cout << "KEYWORD: " << identifier << std::endl;
        this->writeToFile("KEYWORD: " + identifier + "\n");
        result = true;
    }
    //else if (identifier.compare("mod") == 0) {
    //    std::cout << identifier << std::endl;
    //    this->writeToFile(identifier + "\n");
    //}
    else {
        //std::cout << "ID: " << identifier << " Scope: " << this->scope <<  std::endl;
        this->writeToFile("ID: " + identifier + "\n");
        result = true;
    }
    return result;
}

bool LexicalAnalyzer::analyzeSpecialCharacter() {
    bool result = false;
    int commentCounter = 0;
    char peekCharacter;
    std::string character = "";
    character.append(1, this->currentCharacter);
    if (this->searchSpecialCharacter(this->currentCharacter)) {
        if (this->currentCharacter == '/') {
            peekCharacter = this->peekAtNextCharacter();
            if (peekCharacter == '*') {
                commentCounter++;
                this->moveToNextCharacter();
                while (commentCounter > 0) {
                    if (!this->moveToNextCharacter()) {
                        // We have reached end of file
                        break;
                    }
                    if (this->currentCharacter == '*') {
                        peekCharacter = this->peekAtNextCharacter();
                        if (peekCharacter == '/') {
                            commentCounter--;
                            this->moveToNextCharacter();
                        }
                    }
                    else if (this->currentCharacter == '/') {
                        peekCharacter = this->peekAtNextCharacter();
                        if (peekCharacter == '*') {
                            commentCounter++;
                            this->moveToNextCharacter();
                        }
                    }
                }
            }
            else if (peekCharacter == '/') {
                this->moveToNextline();
                result == true;
            }
            else if (character.length() > 0) {
                //std::cout << character << std::endl;
                this->writeToFile(character + "\n");
            }
        }
        else if (this->currentCharacter == '<' | this->currentCharacter == '>' | this->currentCharacter == '=' | this->currentCharacter == '!') {
            peekCharacter = this->peekAtNextCharacter();
            if (peekCharacter == '=') {
                //std::cout << character;
                this->writeToFile(character + "");
                if (this->moveToNextCharacter()) {
                    //std::cout << this->currentCharacter << std::endl;
                    // So, this code gets the deadSymbol string for some reason
                    //this->writeToFile(this->currentCharacter + "\n");
                    this->writeToFile("=\n");
                }
            }
            else if (this->currentCharacter == '!') {
                result = this->errorLine(character);
            }
            else if (character.length() > 0) {
                //std::cout << character << std::endl;
                this->writeToFile(character + "\n");
            }
        }
        else if (character.length() > 0) {
            //std::cout << character << std::endl;
            this->writeToFile(character + "\n");
        }
        // For Scope Calculations
        if (this->currentCharacter == '{') {
            this->scope++;
        }
        else if (this->currentCharacter == '}') {
            this->scope--;
        }
    }
    else {
        result = this->errorLine(character);
    }
    return result;
}

bool LexicalAnalyzer::writeToFile(std::string outputLine) {
    std::fstream outputFile;
    outputFile.open(this->outputFilename.c_str(), std::ios::out | std::ios::app);
    outputFile << outputLine;
    outputFile.flush();
    outputFile.close();
    return true;
}

bool LexicalAnalyzer::moveToNextCharacter() {
    bool result = false;
    this->currentLineIndex++;
    if (this->currentLineIndex == this->currentLine.length()) {
        // Move to next line
        if (this->moveToNextline()) {
            result = true;
        }
    }
    else if (this->currentLineIndex < this->currentLine.length()) {
        result = true;
    }

    if (result) {
        this->currentCharacter = this->currentLine[this->currentLineIndex];
    }
    return result;
}

bool LexicalAnalyzer::moveToNextline() {
    bool result = false;
    std::string temp;
    while (temp.length() <= 0) {
        if (!std::getline(this->currentFile,temp)) {
            // getLine failed, we have reached the end of the file
            break;
        }
    }
    if (temp.length() > 0) {
        result = true;
        this->currentLine = temp;
        this->currentLineIndex = 0;
        //std::cout << "INPUT: " << temp << std::endl;
        //this->writeToFile("INPUT: " + temp + "\n");
    }
    else {
        this->currentLine = "";
        this->currentLineIndex = -1;
    }
    return result;
}

char LexicalAnalyzer::peekAtNextCharacter() {
    char peekCharacter = '$';
    if (this->currentLineIndex + 1 < this->currentLine.length()) {
        peekCharacter = this->currentLine[this->currentLineIndex + 1];
    }
    return peekCharacter;
}

bool LexicalAnalyzer::searchKeyword(std::string search) {
    bool found = false;
    for (int i = 0; i < 7; i++) {
        if (search.compare(this->keywords[i]) == 0) {
            found = true;
        }
    }
    return found;
}

bool LexicalAnalyzer::searchSpecialCharacter(char search) {
    bool found = false;
    for (int i = 0; i < 16; i++) {
        if (search == this->specialCharacters[i]) {
            found = true;
        }
    }
    return found;
}

bool LexicalAnalyzer::errorLine(std::string str) {
    bool endProgram = false;
    bool continueLoop = true;
    std::string remainingLine = str;
    char peekCharacter;
    //Going to find the end of this current token
    while (continueLoop) {
        peekCharacter = this->peekAtNextCharacter();
        if (peekCharacter == '$') {
            continueLoop = false;
        }
        else if (this->searchSpecialCharacter(peekCharacter)) {
            continueLoop = false;
        }
        else if (peekCharacter == ' ') {
            continueLoop = false;
        }
        else {
            remainingLine.append(1, peekCharacter);
        }
        if (continueLoop) {
            endProgram = this->moveToNextCharacter();
        }
    }
    //std::cout << "ERROR: " << remainingLine << std::endl;
    this->writeToFile("ERROR: " + remainingLine + "\n");
    return endProgram;
}