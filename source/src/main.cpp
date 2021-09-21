/**
 * @file main.cpp
 * @author João Guilherme Lopes Alves da Costa
 * @author Thiago de Oliveira Cordeiro
 * @brief This programming project implements a simple parser for an EBNF
 * (Extended Backus-Naur Form) grammar.
 * @version 1.0
 * @date 2021-08-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "../include/bares_manager.h"

int main( void ) {
    BaresManager bm; // an instance of class BaresManager

    std::string expr;
    // evaluate an expression while has lines to read.
    while (std::getline(std::cin, expr))
    {
        bm.parse_and_compute(expr);
    }

    return EXIT_SUCCESS;
}
