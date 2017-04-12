/* 
 * File:   SymbolTable.cpp
 * Author: Michael Frederick (n00725913)
 * 
 * Created on February 20, 2017, 5:50 PM
 */

#include "SymbolTable.h"

SymbolTable::SymbolTable(int size) {
    this->deadSymbol = new Symbol();
    this->size = this->getNextPrime(size);
    this->table = new Symbol*[this->size];
    for (int i = 0; i < this->size; i++) {
        this->table[i] = new Symbol();
    }
    this->assignedValues = 0;
}

Symbol* SymbolTable::addSymbol(Symbol* sym) {
    Symbol* resultSymbol;
    int index = this->HashFunction(sym->getIdentifier());
    int collisions = 0;
    bool searching = true;
    Symbol* currentSymbol;

    while (searching) {
        if (this->table[index]->isEqual(this->deadSymbol)) {
            searching = false;
            resultSymbol = sym;
            this->table[index] = sym;
            this->assignedValues++;
        }
        else if (this->table[index]->getIdentifier().compare(sym->getIdentifier()) == 0) {
            // Same identifier
            currentSymbol = this->table[index];
            if (currentSymbol->getDeclaredScope().compare(sym->getDeclaredScope()) != 0) {
                // Not the same scope
                while (searching) {
                    if (currentSymbol->getDeclaredScope().compare(sym->getDeclaredScope()) == 0) {
                        // Same Id and same scope
                        searching = false;
                        resultSymbol = currentSymbol;
                    }
                    else if (currentSymbol->getIsFunction()) {
                        // This identifier is a function
                        searching = false;
                        resultSymbol = currentSymbol;
                    }
                    else if (!currentSymbol->hasNextSymbol()) {
                        currentSymbol->linkNextSymbol(sym);
                        searching = false;
                        resultSymbol = sym;
                    }
                    else {
                        currentSymbol = currentSymbol->getNextSymbol();
                    }
                }
            }
            else {
                // Same identifier and scope. That's a no
                searching = false;
                resultSymbol = currentSymbol;
            }
        }
        else if (collisions > 10) {
            collisions++;
            index = (index + 1) % this->size;
        }
        else if (collisions == this->size) {
            // Something done broke
            this->throwFloatException();
        }
        else {
            collisions++;
            index = ((index * index) * collisions) % this->size;
        }
    }
        
    return resultSymbol;
}

Symbol* SymbolTable::getSymbol(std::string id, std::string scope) {
    Symbol* resultSymbol = new Symbol();
    Symbol* currentSymbol;
    int index = this->HashFunction(id);
    int collisions = 0;
    bool searching = true;
    
    while (searching) {
        currentSymbol = this->table[index];
        if (currentSymbol->getIdentifier().compare(id) == 0) {
            // Same identifier. Check if scope is find if scope is same then check if global stop when no more symbols exist in list
            while (searching) {
                if (currentSymbol->getDeclaredScope().compare(scope) == 0) {
                    searching = false;
                    resultSymbol = currentSymbol;
                }
                else if (currentSymbol->getDeclaredScope().compare("") == 0) {
                    resultSymbol = currentSymbol;
                    if (currentSymbol->hasNextSymbol()) {
                        currentSymbol = currentSymbol->getNextSymbol();
                    }
                    else {
                        searching = false;
                    }
                }
                else if (currentSymbol->hasNextSymbol()) {
                    currentSymbol = currentSymbol->getNextSymbol();
                }
                else {
                    searching = false;
                }
            }
        }
        else if (currentSymbol->getIdentifier().compare(this->deadSymbol->getIdentifier()) == 0) {
            // id does not exist in table
            searching = false;
        }
        else if (collisions == this->size) {
            // Something done broke
            searching = false;
        }
        else if (collisions > 10) {
            // Not the same identifier, move index and increment collisions
            collisions++;
            index = (index + 1) % this->size;
        }
        else {
            // Not the same identifier, move index and increment collisions
            collisions++;
            index = ((index * index) * collisions) % this->size;
        }
    }
    
    return resultSymbol;
}

Symbol SymbolTable::getDeadSymbol() {
    return *this->deadSymbol;
}

int SymbolTable::HashFunction(std::string str) {
    int value = 1;
    for (int i = 0; i < str.length(); i++) {
        value = (value * (26 + str[i])) % this->size;
    }
    return value;
}

int SymbolTable::getNextPrime(int num) {
    // 9973 is the highest prime number
    for (int i = num; i <= 9973; i++) {
        if (this->isPrime(i) && (i / 2) > num) {
            return i;
        }
    }
    return 9973;
}

bool SymbolTable::isPrime(int p) {
    for (int i = 2; (i * i) <= p; i++) {
        if ((p % i) == 0) {
            return false;
        }
    }
    return true;
}

void SymbolTable::throwFloatException() throw(float) {
    //throw (float)-1.0;
    return;
}

void SymbolTable::printTable() {
    Symbol sym;
    std::cout << " ---Symbol Table--- " << std::endl;
    std::cout << "Identifier | Scope | Location" << std::endl;
    std::cout << "RE-WORK THIS" << std::endl;
    //for (int i = 0; i < this->size; i++) {
    //    sym = this->table[i];
    //    if (!sym.isEqual(this->deadSymbol)) {
    //        std::cout << sym.getIdentifier() << " | " << sym.getDeclaredScope() << " | " << i << std::endl;
    //    }
    //}
    return;
}