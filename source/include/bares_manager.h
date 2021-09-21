#ifndef _BARESMANAGER_H_
#define _BARESMANAGER_H_

#include "parser.h"

class BaresManager {
    public:
        /**
         * @brief Send to the standard output the proper error messages.
         * @param result what happened in the operation.
         * @param str the expression that was analyzed.
         */
        void print_error_msg( const Parser::ResultType & result, const std::string &str );

        /**
         * @brief Parse a line and compute a expression.
         * @param expr the expression that will be calculated.
         */
        void parse_and_compute(std::string expr);

        /**
         * @brief Function to analyze the precedence of operators.
         * @param c the operator that will be analyzed.
         * @return int a number that represents its magnitude among the other operators.
         */
        int prec(std::string c);

        /**
         * @brief Convert infix expression to postfix expression.
         * @see The implementation was inspired by the website:
         * https://www.geeksforgeeks.org/stack-set-2-infix-to-postfix/
         */
        void infix_to_postfix(void);

        /**
         * @brief Calculates the postfix expression.
         */
        void calculate(void);
        
    private:
        Parser::ResultType status; //!< The status of the program, if has an error or no.
        sc::vector<Token> tokens;   //!< The tokens used during the program.
        Parser::required_int_type final_value; //!< The final value of the expression that was calculated.
};

#endif
