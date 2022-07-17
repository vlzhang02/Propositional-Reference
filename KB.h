/*
 * KB.h (Knowledge Base)
 * by: Valerie Zhang
 *
 * Purpose: Stores conditional clauses and propositional symbols
 *
 */
#ifndef _KB_H_
#define _KB_H_

#include "SymTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <string>
#include <vector>
#include <queue>
using namespace std;

static const int initialCapacity = 100;

class KB {
public: 
    KB();
    KB(string filename, SymTable& inferred);
    ~KB();

    int numClauses();
    int numSymbols();
    int getCount(string key);
    string getHead(string key);

    void openAndParseFile(string filename, SymTable& inferred);
    void setQueue(queue<string>& q);
    void decrement(string key);
    void addSymbol(string symbol);

    void print();

private:
    struct clause {
        string clause;
        string head;
        vector<string> tail;
        int count;
    };
    
    int capacity;
    int numClause;
    int numSym;
    clause* table;

    queue<string> info;
    ifstream infile;

    void parseFile(SymTable& inferred);
    void parseLine(string input, int index, SymTable& inferred);

    /* maintaining the table */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(string key);
    int linearProbeIndex(string key, int index);
};
#endif