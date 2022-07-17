/*
 * FC.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of FC class. The FC class performs propositional
 *          inference
 */

#include "FC.h"

using namespace std;
/*
 * default constructor
 */
FC::FC() {}
/*
 * secondary constructor
 * Purpose:     initializes instance through a provided file
 */
FC::FC(string filename) {
    knowledgeBase.openAndParseFile(filename, inferred);
    knowledgeBase.setQueue(symbols);
}
/*
 * destrcutor
 */
FC::~FC() {}
/*
 * run()
 * Purpose:     gets user query and loops program
 * Parameters:  none
 * Returns:     none
 */
void FC::run() {
    knowledgeBase.print();
    string input = "";
    while(input != "end") {
        cout << "Query symbol (or end): ";
        cin >> input;
        if (input == "end") {
            break;
        }
        if (entails(input)) {
            cout << "Yes! " << input << " is entailed by our knowledge-base.\n\n";
        } else {
            cout << "No. " << input << " is not entailed by our knowledge-base.\n\n";
        }
        knowledgeBase.setQueue(symbols);
    }

}
/*
 * entails()
 * Purpose:     determine if the KB entails a query through forward chaining
 * Parameters:  string query
 * Returns:     true if query is entailed, false if not
 */
bool FC::entails(string input) {
    while (!symbols.empty()) {
        string s = symbols.front(); // get front element
        symbols.pop(); // remove from queue
        if (input == s) {return true;} // return true if matches query
        if (inferred.getInferred(s) == false) {
            inferred.set(s); // set inference to true
            for (int i = 0 ; i < inferred.getNumClauses(s); i++) {
                string clause = inferred.getClause(s, i);
                knowledgeBase.decrement(clause); // decrement count in clause
                if (knowledgeBase.getCount(clause) == 0) { // if all conditions met
                    string symbol = knowledgeBase.getHead(clause);
                    symbols.push(symbol); // add to queue
                    knowledgeBase.addSymbol(symbol); // add to knowledge base
                }
            }
        }
    }
    return false;
}