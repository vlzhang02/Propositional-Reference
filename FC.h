/*
 * FC.h
 * by: Valerie Zhang
 *
 * Purpose: The FC class obtains a query from the client and performs propositional inference
 *          to inform if the knowledge base entails the query.
 *
 */
#ifndef _FC_H_
#define _FC_H_

#include "SymTable.h"
#include "KB.h"
#include <iostream>
#include <string>
#include <queue>

using namespace std;

class FC {
public:
    FC();
    FC(string filename);
    ~FC();

    void run(); 
private:
    KB knowledgeBase;
    SymTable inferred;
    queue<string> symbols;

    bool entails(string input);
};
#endif