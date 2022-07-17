/*
 * KB.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of the KB class. The KB class stores clauses and
 *          propositional symbols
 *
 */
#include "KB.h"

using namespace std;

/*
 * default constructor
 */
KB::KB() {
    capacity = INITIAL_CAPACITY;

    table =  new clause[capacity];
    clause c;
    c.head = "NONE";
    c.clause = "NONE";

    for (int i = 0; i < capacity; i++) {
        table[i] = c;
    }
    numClause = numSym = 0;
}
/*
 * secondary constructor
 * Purpose:     initializes knowledge base with file
 * Parameters:  filename
 * Returns:     none lol
 */
KB::KB(string filename, SymTable& inferred) {
    capacity = INITIAL_CAPACITY;
    table =  new clause[capacity];
    clause c;
    c.head = "NONE";
    c.clause = "NONE";
    c.count = 0;
    for (int i = 0; i < capacity; i++) {
        table[i] = c;
    }
    numClause = numSym = 0;
    openAndParseFile(filename, inferred);
}
/*
 * destructor
 */
KB::~KB() {
    delete [] table;
    numClause = numSym = 0;
}
/*
 * numClauses()
 * Purpose:     get total number of clauses in knowledge base
 * Parameters:  none
 * Returns:     number of clauses in knowlege base
 */
int KB::numClauses() {
    return numClause;
}
/*
 * numSymbols()
 * Purpose:     get total number of symbols in knowledge base
 * Parameters:  none
 * Returns:     number of symbols in knowlege base
 */
int KB::numSymbols() {
    return numSym;
}
/*
 * getCount()
 * Purpose:     get count of untrue symbols(according to KB) in clause
 * Parameters:  string key
 * Returns:     number of false symbols in clause
 */
int KB::getCount(string key) {
    return table[linearProbeIndex(key, hashIndex(key))].count;
}
/*
 * getHeat()
 * Purpose:     get head of clause
 * Parameters:  string key
 * Returns:     string head of clause
 */
string KB::getHead(string key) {
    return table[linearProbeIndex(key, hashIndex(key))].head;
}
/* 
 * openAndParseFile()
 * Purpose: opens and parses file
 * Parameters: filename
 * Returns: none
 */
void KB::openAndParseFile(string filename, SymTable& inferred) {
    infile.open(filename);
    if (!infile.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
    }
    parseFile(inferred);
    infile.close();
}
/*
 * parseFile()
 * Purpose:     parse file and add clauses and symbols to knowledge base
 * Parameters:  none
 * Returns:     none
 */
void KB::parseFile(SymTable& inferred) {
    string input = "";
    while (getline(infile, input)) {
        info.push(input); // add to printing queue
        int index = linearProbeIndex(input, hashIndex(input)); 
        table[index].clause = input; // add clause to KB
        parseLine(input, index, inferred); // parse line
    }
}
/*
 * parseLine()
 * Purpose:     parses line for symbols and adds to knowledge base entry and 
 *              inference table 
 * Parameters:  string input
 * Returns:     none
 */
void KB::parseLine(string input, int index, SymTable& inferred) {
    stringstream ss(input);
    string word;
    bool head = false;
    while (ss >> word) { // parse through word by word
        if (!head) {
            if (word[0] == 'p') {
                table[index].tail.push_back(word); // add to premises
                inferred.add(word); // add to inference table
                if (input.size() >= 8) {
                    inferred.addClause(word, input); // add clause to inference table
                }
            } else if (word == "THEN") { // determine when head is found
                head = true;
            }
        } else {
            table[index].head = word;
            inferred.add(word);
        }
    }
    table[index].count = table[index].tail.size();
    if (head) { // increment corresponding count
        numClause++;
    } else {
        numSym++;
    } 
}
/*
 * setQueue()
 * Purpose:     initialize queue with all known symbols that are true in 
 *              knowledge base
 * Parameters:  address of queue
 * Returns:     none
 */
void KB::setQueue(queue<string>& q) {
    for (int i = 0; i < capacity; i++) {
        if (table[i].clause != "NONE") {
            if (table[i].head == "NONE") {
                q.push(table[i].clause);
            }
        }
    }
}
/*
 * decrement()
 * Purpose:     decrement conditionals count
 * Parameters:  string key(clause)
 * Returns:     none 
 */
void KB::decrement(string key) {
    table[linearProbeIndex(key, hashIndex(key))].count--;
}
/*
 * addSymbol()
 * Purpose:     add entailed symbol to KB
 * Parameters:  symbol to add
 * Return:      none
 */
void KB::addSymbol(string symbol) {
    int index = linearProbeIndex(symbol, hashIndex(symbol));
    table[index].clause = symbol;
}
/*
 * print()
 * Purpose:     prints all info in KB
 * Parameters:  none
 * Returns:     none
 */
void KB::print() {
    cout << "KB has " << numClause << " conditonal clauses and " << numSym 
         << " propositional symbols.\n\n";
    cout << "\tClauses: ";
    if (numClause == 0) {
        cout << "NONE\n";
    } else {
        for (int i = 0; i < numClause; i++) {
            if (i != 0) {
                cout << "\t\t ";
            }
            cout << info.front() << "\n";
            info.pop();
        }
    }
    cout << "\tSymbol: ";
    if (numSym == 0) {
        cout << "NONE\n\n";
    } else {
        for (int j = 0; j < numSym; j++) {
            cout << info.front();
            info.pop();
            if (j != numSym-1) {
                cout << ", ";
            }
        }
        cout << "\n\n";
    }
}
/********************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/

/* expand()
 * Parameters: none
 * Purpose: expands the capacity and rehashes all entries
 * Returns: n/a
 */
void KB::expand() {
    int        oldCapacity = capacity;
    clause *oldTable        = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new clause[capacity]; // create new table
    clause c;
    c.head = "NONE";
    for (int i = 0; i < capacity; i++) {
        table[i] = c;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].head != "NONE") { // if entry empty: continue,
            string key = oldTable[i].clause;
            int index = linearProbeIndex(key, hashIndex(key));
            // copy over contents in table
            table[index].clause = oldTable[i].clause;
            table[index].head = oldTable[i].head;
            table[index].count = oldTable[i].count;
            for (size_t j = 0; j < oldTable[i].tail.size(); j++) { 
                table[index].tail.push_back(oldTable[i].tail[j]);
            }
        }
    }
    delete [] oldTable;
}
/* loadFactorCheck()
 * Parameters: none
 * Purpose: checks the load factor and rehashes if necessary
 * Returns: n/a
 */
void KB::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = (numClause + numSym) * 1.0;
    if (nc / c > .7) {
        expand();
    }
}
/********************************************************************\
*                           hash functions                           *
\********************************************************************/

/* linearProbeIndex()
 * parameters:
 *  -   int index represents the original hash index for a key
 *  -   string &key is a key to find the index for
 * purpose: to find the correct index for a key using linear probing
 * returns: an integer representing the key's index
 */
int KB::linearProbeIndex(string key, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].clause == "NONE") { // if index is empty
            return index;
        } else if (table[index].clause == key) { // if clause matches
            return index;
        }
        attempt++;
    }
    return 0;
}
/* hashIndex()
 * parameters:
 *  -   string &key is a key to find an index for
 * purpose: calls the hash function on the key, then mods it to get an
 *          index
 * returns: the index corresponding to the key
 */
int KB::hashIndex(string key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<string>{}(key) % capacity);
}