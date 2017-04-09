/* 
 * File:   Parser.h
 * Author: Michael Frederick (n00725913)
 *
 * Created on February 24, 2017, 6:15 PM
 */

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <iostream>
#include <new>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include "SymbolTable.h"
#include "Symbol.h"

#define INT_TO_STRING( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class Parser {
public:
    Parser(std::fstream& inputFile);
    bool parseFile(SymbolTable* symTable);
private:
    // Class Variables
    std::string currentToken;
    std::fstream& filestream;
    
    // Added for Project Three
    Symbol* lastSymbol;
    Symbol* functionSymbol;
    SymbolTable* symTab;
    std::string currentScope;
    std::string functionId;
    std::string lastId;
    std::string lastType;
    int numberOfParamSeen;
    bool seenReturnStmt;
    
    // Added for Project Four
    int currentOutputLine;
    int tempVariableCount;
    int lastNumberConstant;
    std::string** output;
    
    // Class Methods
    bool getNextToken();
    bool acceptToken(std::string token, bool addSymbol);
    void throwBadAcceptToken(std::string badToken, std::string expectedToken) throw(std::string);
    void throwException() throw (int);
    void throwFloatException() throw(float);
    bool searchArray(int arraySize, std::string *array, std::string key);
    
    // Added for project 4
    void printOutput();
    
    // Grammar Methods
    void program();
    void declarationList();
    void declarationListPrime();
    void declaration();
    void callDeclaration();
    void idSpecifier();
    void typeSpecifier();
    void params();
    void paramList();
    void paramListPrime();
    void param();
    void array();
    void compountStmt();
    void localDeclarations();
    void localDeclarationsPrime();
    void statementList();
    void statementListPrime();
    void statement();
    void expressionStmt();
    void selectionStmt();
    void danglingElse();
    void iterationStmt();
    void returnStmt();
    void returnStmtEnd();
    std::string expression();
    std::string variable(std::string leftType);
    std::string variableFactor(std::string leftType);
    std::string varArray(std::string leftType);
    std::string relopExpression(std::string leftType);
    std::string relop();
    std::string additiveExpression(std::string leftType);
    std::string additiveExpressionPrime(std::string leftType);
    std::string addop();
    std::string term();
    std::string termPrime(std::string leftType);
    std::string factor();
    std::string mulop();
    std::string varCall(std::string leftType);
    std::string args();
    std::string argList();
    std::string argListPrime();
};

#endif /* PARSER_H */

