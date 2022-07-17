/*
 * main.cpp
 * by: Valerie Zhang
 *
 * Purpose: Run program for propositional inference
 *
 */
#include "SymTable.h"
#include "KB.h"
#include "FC.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./solver gridFile wordFile \n";
        exit(EXIT_FAILURE);
    }
    FC f(argv[1]);
    f.run();

    return 0;
}