/*
 * Symable.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of SymTable class. A SymTable stores inferences
 *          of symbols in a knowledge base
 */
#include "SymTable.h"

using namespace std;
/*
 * default constructor
 */
SymTable::SymTable() {
    capacity = INITIAL_CAPACITY;

    table =  new symbol[capacity];
    symbol s;
    s.symbol = "NONE";
    s.inferred = false;
    for (int i = 0; i < capacity; i++) {
        table[i] = s;
    }
    numSymbols = 0;
}
/*
 * destructor
 */
SymTable::~SymTable() {
    delete [] table;
    numSymbols = 0;
}
/*
 * add()
 * Purpose:     adds symbol to table
 * Parameters:  symbol to add
 * Returns:     none
 */
void SymTable::add(string key) {
    table[linearProbeIndex(key, hashIndex(key))].symbol = key;
}
/*
 * addClause()
 * Purpose:     add clause to list of clauses
 * Parameters:  string key and clause to add
 * Returns:     none
 */
void SymTable::addClause(string key, string clause) {
    table[linearProbeIndex(key, hashIndex(key))].clauses.push_back(clause);
}
/*
 * set()
 * Purpose:     sets symbol as true
 * Parameters:  symbol to set
 * Returns:     none
 */
void SymTable::set(string key) {
    table[linearProbeIndex(key, hashIndex(key))].inferred = true;
}
/*
 * getInferred()
 * Purpose:     get inferred value for specified symbol
 * Parameters:  string key
 * Returns:     inferred value
 */
bool SymTable::getInferred(string key) {
    return table[linearProbeIndex(key, hashIndex(key))].inferred;
}
/*
 * getNumClauses()
 * Purpose:     gets number of clauses symbol is a premise in
 * Parameters:  string key
 * Returns:     number of premised clauses
 */
int SymTable::getNumClauses(string key) {
    return table[linearProbeIndex(key, hashIndex(key))].clauses.size();
}
/*
 * getClause()
 * Purpose:     gets clause are particular index
 * Parameters:  string key and index of clause
 * Returns:     string clause
 */
string SymTable::getClause(string key, int index) {
    return table[linearProbeIndex(key, hashIndex(key))].clauses[index];
}
 /********************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/

/* expand()
 * Parameters:  none
 * Purpose:     expands the capacity and rehashes all entries
 * Returns:     n/a
 */
void SymTable::expand() {
    int        oldCapacity = capacity;
    symbol *oldTable        = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new symbol[capacity]; // create new table
    symbol s;
    s.symbol = "NONE";
    s.inferred = false;
    for (int i = 0; i < capacity; i++) { // initialized with empty symbol
        table[i] = s;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].symbol != "NONE") { // if entry empty: continue,
            string key = oldTable[i].symbol;
            int index = linearProbeIndex(key, hashIndex(key));
            // copy over contents in table
            table[index].symbol = oldTable[i].symbol;
            table[index].inferred = oldTable[i].inferred;
            for (size_t j = 0 ; j < oldTable[i].clauses.size(); j++) {
                table[index].clauses.push_back(oldTable[i].clauses[j]);
            }
        }
    }
    delete [] oldTable;
}
/* loadFactorCheck()
 * Parameters:  none
 * Purpose:     checks the load factor and rehashes if necessary
 * Returns:     n/a
 */
void SymTable::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = numSymbols * 1.0;
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
 * purpose:     to find the correct index for a key using linear probing
 * returns:     an integer representing the key's index
 */
int SymTable::linearProbeIndex(string key, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].symbol == "NONE") { // if index is empty
            return index;
        } else if (table[index].symbol == key) { // if clause matches
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
int SymTable::hashIndex(string key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<string>{}(key) % capacity);
}