/* 
 * File:   Symbol.h
 * Author: Michael Frederick (n00725913)
 *
 * Created on February 20, 2017, 5:49 PM
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol {
public:
    //Constructor
    Symbol();
    Symbol(std::string identifier, std::string scope);
    //Variables
    
    //Functions
    std::string getIdentifier();
    std::string getDeclaredScope();
    std::string getType();
    std::string getValue();
    bool getIsFunction();
    bool getIsArray();
    void setType(std::string type);
    void setValue(std::string value);
    void changeIsFunction();
    void changeIsArray();
    void setNumberOfParams(int number);
    int getNumberOfParams();
    void linkNextSymbol(Symbol* nextSym);
    bool hasNextSymbol();
    Symbol* getNextSymbol();
    bool isEqual(Symbol* sym);
private:
    //Variables
    std::string identifier;
    std::string declaredScope;
    std::string type;
    std::string value;
    bool isFunction;
    bool isArray;
    int numberOfParams;
    Symbol* nextSymbol;
    
    //Functions
    void throwFloatException() throw(float);
};

#endif /* SYMBOL_H */
