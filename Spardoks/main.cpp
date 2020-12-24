#include "computer.hpp"
#include <iostream>

// uncomment this to compile for testing
//#define TEST

#ifdef TEST
#include "test_computing.hpp"
#endif


int main() {

#ifdef TEST

    test();

#else // !TEST

    ///// cmd_mode in future

    ///// interactive_mode
    Computer c1;
    // get expression
    std::string expression;
    std::cout << "Input your expression: " ;
    std::getline(std::cin, expression);
    // compute
    std::string result = c1.get_result(expression);
    // output
    std::cout << "Answer is: " << result << std::endl;

#endif // TEST

    return 0;
}
