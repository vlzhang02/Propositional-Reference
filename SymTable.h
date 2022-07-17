/*
 * SymTable.h
 * by: Valerie Zhang
 *
 * Purpose: stores inferences about symbols in a knowledge base
 */
#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <vector>
#include <iostream>
#include <string>
#include <functional>

using namespace std;

static const int INITIAL_CAPACITY = 100;

class SymTable {
public:
    SymTable();
    ~SymTable();

    void add(string key);
    void addClause(string key, string clause);
    void set(string key);

    bool getInferred(string key);
    int getNumClauses(string key);
    string getClause(string key, int index);
    void print();

private:
    struct symbol {
        string symbol;
        bool inferred;
        vector<string> clauses;
    };
    
    int capacity;
    int numSymbols;
    symbol* table;

    /* maintaining the table */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(string key);
    int linearProbeIndex(string key, int index);
};
#endif