#include <iostream>
#include <iomanip>

#include "../lib/vector.h"
#include "../include/bares_manager.h"

/// List of expressions to evaluate and tokenize.
sc::vector<std::string> expressions = {
    "4 + 03",
    "10",
    "    12    +    4   8",
    "32767 - 32768 + 3",
    "5 + -32766",
    "5 + -32769",
    "12 + 3",
    "-3+-5+-6",
    "12 + 3     -3 + -34 ",
    "+12",
    "1.3 * 4",
    "a + 4",
    "       ",
    "  123 *  548",
    "4 + ",
    "32a23",
    "43 + 54 -   "
};

/// Send to the standard output the proper error messages.
void BaresManager::print_error_msg( const Parser::ResultType & result, const std::string &str ) {
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type ) {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << "Unexpected end of input at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::ILL_FORMED_INTEGER:
            std::cout << "Ill formed integer at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::MISSING_TERM:
            std::cout << "Missing <term> at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::EXTRANEOUS_SYMBOL:
            std::cout << "Extraneous symbol after valid expression found at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << "Integer constant out of range beginning at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::MISSING_CLOSING:
            std::cout << "Missing closing \")\" at column (" << result.at_col+1 << ")!\n";
            break;
        case Parser::ResultType::DIVISION_BY_ZERO:
            std::cout << "Division by zero!\n";
            break;
        case Parser::ResultType::OVERFLOW_ERROR:
            std::cout << "Numeric overflow error!\n";
            break;
        default:
            std::cout << "Unhandled error found!\n";
            break;
    }
    //? Indicate the column of error.
    // std::cout << "\"" << str << "\"\n";
    // std::cout << " " << error_indicator << std::endl;
}

/// Function to return precedence of operators
int BaresManager::prec(std::string c) {
    if (c == "^")
        return 3;
    else if (c == "/" || c == "*" || c == "%")
        return 2;
    else if (c == "+" || c == "-")
        return 1;
    else
        return -1;
}

/// The main function to convert infix expression
/// to postfix expression
void BaresManager::infix_to_postfix(void) {
    sta::stack<Token> st; // For stack operations
    sc::vector<Token> pf_tk_list;

    for (size_t i{0}; i < tokens.size(); i++) {
        Token c = tokens[i];

        // If the scanned character is
        // an operand, add it to output string.
        if (c.type == Token::token_t::OPERAND)
            pf_tk_list.push_back(c);

        // If the scanned character is an
        // ‘(‘, push it to the stack.
        else if (c.type == Token::token_t::OPEN_PARENTHESES)
            st.push(c);

        // If the scanned character is an ‘)’,
        // pop and to output string from the stack
        // until an ‘(‘ is encountered.
        else if (c.type == Token::token_t::CLOSE_PARENTHESES) {
            while (st.top().type != Token::token_t::OPEN_PARENTHESES)
            {
                pf_tk_list.push_back(st.top());
                st.pop();
            }
            st.pop();
        }

        //If an operator is scanned
        else {
            while (not st.empty() and prec(tokens[i].value) <= prec(st.top().value)) {
                pf_tk_list.push_back(st.top());
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all the remaining elements from the stack
    while (not st.empty()) {
        pf_tk_list.push_back(st.top());
        st.pop();
    }

    tokens = pf_tk_list;
}

/// Function that calculates the postfix expression
void BaresManager::calculate(void) {
    sta::stack<Parser::input_int_type> st; // The stack to store the operands.
    Parser::input_int_type result{0}; // The result of expression;

    // Travels the tokens to calculate the expression.
    for (size_t i{0}; i < tokens.size(); i++) {
        Token c = tokens[i];
        // transform std::string to char[]
        // to use atoll and switch
        size_t sz = c.value.length();
        // declaring character array
        char c_value[sz + 1];
        // copying the contents of the
        // string to char array
        std::strcpy(c_value, c.value.c_str());

        // If it is an operand, transform in int and push on the stack.
        if (c.type == Token::token_t::OPERAND) {
            // transform in int and push on the stack.
            Parser::input_int_type value = std::atoll(c_value);
            st.push(value);
        }
        // If it is an operator, pop twice on stack and calculate the expression.
        else {
            // Take from stack the two values that will be calculated.
            Parser::input_int_type second_operand = st.top();
            st.pop();
            Parser::input_int_type first_operand = st.top();
            st.pop();
            // To avoid special cases of operations with 0.
            if ( second_operand == 0 and (c_value[0] == '/' or c_value[0] == '%') ) {
                status = Parser::ResultType{ Parser::ResultType::DIVISION_BY_ZERO };
            }
            else {
                // Decide the operation that will be made.
                switch (c_value[0]) {
                    case '+':  result = first_operand + second_operand; break;
                    case '-':  result = first_operand - second_operand; break;
                    case '*':  result = first_operand * second_operand; break;
                    case '/':  result = first_operand / second_operand; break;
                    case '%':  result = first_operand % second_operand; break;
                    case '^':
                        // Calculate the exception of x^0 = 1
                        if (second_operand == 0)
                            result = 1;
                        else if (second_operand < 0)
                            result = 0;
                        else {
                            Parser::input_int_type expo = first_operand;
                            while (second_operand != 1) {
                                expo *= first_operand;
                                second_operand--;
                            }
                            result = expo;
                        }
                }
                // We did a calculation, did it generate an overflow?
                if ( result < std::numeric_limits< Parser::required_int_type >::min() or
                    result > std::numeric_limits< Parser::required_int_type >::max() ) {
                    // Overflow occurred, stop the loop.
                    break;
                }
            }
            // Insert the result on the top of stack.
            st.push(result);
        }
    }
    // Case of one operand is passed.
    if (st.size() == 1) {
        result = st.top();
        st.pop();
    }
    
    // We calculate the result, just know if it is within the range (overflow occurred).
    if ( result < std::numeric_limits< Parser::required_int_type >::min() or
         result > std::numeric_limits< Parser::required_int_type >::max() ) {
        // Overflow occurred, report error.
        status = Parser::ResultType{ Parser::ResultType::OVERFLOW_ERROR };
    }
    else {
        final_value = result;
    }
}

/// Reads a line and compute a expression.
void BaresManager::parse_and_compute(std::string expr) {
    Parser parser; // Instancia um parser.
    final_value = 0;

    //======================================================================
    //== Códigos para ajudar na depuração
    //----------------------------------------------------------------------
    // Tentar analisar cada expressão da lista.
    /*for( const auto & expr : expressions ) {
        // [I] Fazer o parsing desta expressão.
        auto result = parser.parse_and_tokenize( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ResultType::OK )
            print_error_msg( result, expr );
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

        // [II] Recuperar a lista de tokens.
        auto lista = parser.get_tokens();
        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
                std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";
        std::cout << std::endl;
    }*/
    
    //* [I] Fazer o parsing desta expressão.
    status = parser.parse_and_tokenize(expr);
    //? Preparar cabeçalho da saida.
    // std::cout << std::setfill('=') << std::setw(80) << "\n";
    // std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";

    // Se deu pau, imprimir a mensagem adequada.
    if ( status.type != Parser::ResultType::OK )
        print_error_msg( status, expr );
    else {
        // std::cout << ">>> Expression SUCCESSFULLY parsed!\n"; //? Deu certo.
        //* [II.1] Recuperar a lista de tokens no formato infixo.
        tokens = parser.get_tokens();
        // std::cout << ">>> Tokens: { ";
        // std::copy( tokens.begin(), tokens.end(),
        //         std::ostream_iterator< Token >(std::cout, " ") );
        // std::cout << "}\n";
        // std::cout << std::endl;

        //* [II.2] Transformar de infixo para posfixo.
        infix_to_postfix();

        //? [II.3] Recuperar a lista de tokens no formato posfixo.
        // std::cout << ">>> Tokens: { ";
        // std::copy( tokens.begin(), tokens.end(),
        //         std::ostream_iterator< Token >(std::cout, " ") );
        // std::cout << "}\n";
        // std::cout << std::endl;

        //* [III] Calcular a expressão pos fixa.
        calculate();
        if ( status.type != Parser::ResultType::OK )
            print_error_msg( status, expr );
        else
            std::cout << final_value << std::endl;
    }
    // std::cout << "\n>>> Normal exiting...\n";
}