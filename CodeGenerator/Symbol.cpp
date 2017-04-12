/* 
 * File:   Symbol.cpp
 * Author: Michael Frederick (n00725913)
 * 
 * Created on February 20, 2017, 5:49 PM
 */

#include "Symbol.h"

Symbol::Symbol() {
    this->identifier = "$DEAD$";
    this->declaredScope = "";
    this->type = "";
    this->value = "";
    this->isFunction = false;
    this->isArray = false;
    this->numberOfParams = -1;
    this->nextSymbol = NULL;
}

Symbol::Symbol(std::string identifier, std::string scope) {
    this->identifier = identifier;
    this->declaredScope = scope;
    this->type = "";
    this->value = "";
    this->isFunction = false;
    this->isArray = false;
    this->numberOfParams = 0;
    this->nextSymbol = NULL;
}

std::string Symbol::getIdentifier() {
    return this->identifier;
}

std::string Symbol::getDeclaredScope() {
    return this->declaredScope;
}

std::string Symbol::getType() {
    return this->type;
}

std::string Symbol::getValue() {
    return this->value;
}

bool Symbol::getIsFunction() {
    return this->isFunction;
}

bool Symbol::getIsArray() {
    return this->isArray;
}

void Symbol::setType(std::string type) {
    if (this->type.compare("") == 0) {
        // Hasn't been set yet
        this->type = type;
    }
    else {
        this->throwFloatException();
    }
    return;
}

void Symbol::setValue(std::string value) {
    if (this->identifier.compare("$DEAD$") != 0) {
        this->value = value;
    }
    else {
        this->throwFloatException();
    }
    return;
}

void Symbol::changeIsFunction() {
    if (!this->isFunction && this->identifier.compare("$DEAD$") != 0) {
        this->isFunction = true;
    }
    else {
        this->throwFloatException();
    }
    return;
}

void Symbol::changeIsArray() {
    if (!this->isArray && this->identifier.compare("$DEAD$") != 0) {
        this->isArray = true;
    }
    else {
        this->throwFloatException();
    }
    return;
}

void Symbol::setNumberOfParams(int number) {
    if (this->isFunction) {
        this->numberOfParams = number;
    }
    else {
        this->throwFloatException();
    }
    return;
}

int Symbol::getNumberOfParams() {
    if (this->isFunction) {
        return this->numberOfParams;
    }
    else {
        this->throwFloatException();
    }
    return 0;
}

void Symbol::linkNextSymbol(Symbol* nextSym) {
    if (!this->hasNextSymbol()) {
        this->nextSymbol = nextSym;
    }
    else {
        this->throwFloatException();
    }
    return;
}

bool Symbol::hasNextSymbol() {
    return (this->nextSymbol != NULL);
}

Symbol* Symbol::getNextSymbol() {
    if (this->hasNextSymbol()) {
        return this->nextSymbol;
    }
    else {
        this->throwFloatException();
        return new Symbol();
    }
}

bool Symbol::isEqual(Symbol* sym) {
    bool equal = false;

    if ((sym->getIdentifier().compare(this->getIdentifier()) == 0) && (sym->getDeclaredScope().compare(this->getDeclaredScope()) == 0)) {
        equal = true;
    }
    return equal;
}

void Symbol::throwFloatException() throw(float) {
    //throw (float)-1.0;
    return;
}
