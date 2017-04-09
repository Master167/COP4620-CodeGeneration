/* 
 * File:   SymbolTable.h
 * Author: Michael Frederick (n00725913)
 *
 * Created on February 20, 2017, 5:50 PM
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include "Symbol.h"


class SymbolTable {
public:
    SymbolTable(int size);
    // Variables
    Symbol* deadSymbol;
    
    // Functions
    void printTable();
    Symbol* addSymbol(Symbol* sym);
    Symbol* getSymbol(std::string id, std::string scope);
    Symbol getDeadSymbol();
//    removeSymbol(); // To add later
//    updateSymbol();
private:
    // Variables
    Symbol** table;
    int size;
    int assignedValues;

    //Functions
    int HashFunction(std::string str);
    int getNextPrime(int num);
    bool isPrime(int p);
    void throwFloatException() throw(float);
};

#endif /* SYMBOLTABLE_H */
