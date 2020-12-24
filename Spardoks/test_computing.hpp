#ifndef TEST_COMPUTING_HPP
#define TEST_COMPUTING_HPP

#include "computer.hpp"
#include <iostream>
#include <map>
#include <string>
#include <utility>

static size_t passed_ = 0;
static size_t lost_ = 0;

void test_calc(Computer& ct, const std::string& expr, const std::string& answer,
               const std::map<std::string, long double>& var_values = std::map<std::string, long double>()) {
    std::cout << expr << "  =  " << answer << " ----> ";
    std::string res = ct.get_result(expr, 1, var_values);

    if ((res.compare("ERROR") == 0)) {
        if ((answer.compare("ERROR") != 0)) {
            std::cout << "FALSE :(, it is " << res;
            ++lost_;
        } else {
            std::cout << "OK :)";
            ++passed_;
        }
    } else if (((std::abs(std::stold(res) - std::stold(answer)) <= 0.001)
                || (res.compare(answer) == 0))) { // последнее сравнение для inf и nan
         std::cout << "OK :)";
         ++passed_;
    }
    else {
        std::cout << "FALSE :(, it is " << res;
        ++lost_;
    }
    std::cout << std::endl;
}

void test() {
    Computer ct;
    std::cout << "\\----------TESTING----------/" << std::endl;
    test_calc(ct, "1", "1");
    test_calc(ct, "-1", "-1");
    test_calc(ct, "+1", "1");
    test_calc(ct, "+-1", "-1");
    test_calc(ct, "--1", "1");
    test_calc(ct, "---1", "-1");
    test_calc(ct, "+++1", "1");
    test_calc(ct, "++1", "1");
    test_calc(ct, "+-+1", "-1");
    test_calc(ct, "+0", "0");
    test_calc(ct, "-0", "0");
    test_calc(ct, "1 + 1", "2");
    test_calc(ct, "0 + 10", "10");
    test_calc(ct, "10 + 0", "10");
    test_calc(ct, "9 + 0", "9");
    test_calc(ct, "0+9", "9");
    test_calc(ct, "1            +          1", "2");
    test_calc(ct, "-6+6", "0");
    test_calc(ct, "-6+0", "-6");
    test_calc(ct, "1 - 1", "0");
    test_calc(ct, "         1 -          1              ", "0");
    test_calc(ct, "2 - 3", "-1");
    test_calc(ct, "-2 - 3", "-5");
    test_calc(ct, "-2 - -3", "1");
    test_calc(ct, "1-0", "1");
    test_calc(ct, "0-1", "-1");
    test_calc(ct, "1 * 2", "2");
    test_calc(ct, "2 * 9", "18");
    test_calc(ct, "0 * 11", "0");
    test_calc(ct, "11 * 0", "0");
    test_calc(ct, "-6 * 6", "-36");
    test_calc(ct, "-6 * -6", "36");
    test_calc(ct, "-5 / 2", "-2.5");
    test_calc(ct, "-5 * 2 + 3", "-7");
    test_calc(ct, "-5 * 2 + 3 / 2 + 3 * 8 - 2^2^2", "-0.5");
    test_calc(ct, "-5 * ((2 + 3) / (2 + 3)) * 8 - 2^2^2", "-56");
    test_calc(ct, "((2 + 3) / (2 + 3))", "1");
    test_calc(ct, "sqrt((2 + 3) / (2 + 3))", "1");
    test_calc(ct, "sqrt(((2 + 3) / (2 + 3) + tg(0) - cos(0) + sin(0) + exp(0)*sqrt(((120 * 120))) + 1))", "11");
    test_calc(ct, "(2 + 2) + (2 * 2)", "8");
    test_calc(ct, "(2 + 2*-2) + (2 * 2)", "2");
    test_calc(ct, "(2*-2 + 2) + (2 * 2)", "2");
    test_calc(ct, "2+-2*-2", "6");
    test_calc(ct, "2+-2*2", "-2");
    test_calc(ct, "2--2*2", "6");
    test_calc(ct, "2--2*-2", "-2");
    test_calc(ct, "2-(-2)*(-2)", "-2");
    test_calc(ct, "1 / 0", "inf");
    test_calc(ct, "-1 / 0", "-inf");
    test_calc(ct, "-1 / -0", "inf");
    test_calc(ct, "0 / 0", "-nan");
    test_calc(ct, "1/0 - -1/0", "inf");
    test_calc(ct, "9999999999 + 1", "10000000000");
    test_calc(ct, "PI", "3.141593");
    test_calc(ct, "E + 1", "3.718282");
    test_calc(ct, "sin(PI/2)", "1");
    test_calc(ct, "sqrt(-1)", "-nan");
    test_calc(ct, "tg(PI)", "0");
    test_calc(ct, "a * (a - a)", "0", {{"a", 1}});
    test_calc(ct, "x + y + z", "6", {{"x", 1}, {"y", 2}, {"z", 3}});
    // SuperTEST
    long double x1 = 1;
    long double x2 = 2;
    long double x3 = 3;
    test_calc(ct, "2*x1 + sqrt(x2) + exp(x1) + (sin(x3)^2 + E * PI * cos(x1) + x2/x3 - tg(x1)) * ctg(x3) * 4^2",
              std::to_string((long double)(2*x1 + sqrt(x2) + exp(x1) + (pow(sin(x3),2) + M_E * M_PI * cos(x1) + x2/x3 - tan(x1)) * (1./tan(x3)) * pow(4,2))),
              {{"x1", x1}, {"x2", x2}, {"x3", x3}});

    // testing bad input
    test_calc(ct, "", "ERROR");
    test_calc(ct, "!", "ERROR");
    test_calc(ct, "12asdasd", "ERROR");
    test_calc(ct, "12 asdasd", "ERROR");
    test_calc(ct, "asdasd 121", "ERROR");
    test_calc(ct, "1+", "ERROR");

    test_calc(ct, "-2^3", "-8");
    test_calc(ct, "1-2^3", "-7");
    test_calc(ct, "-2^2", "-4");
    test_calc(ct, "1-2^2", "-3");

    test_calc(ct, "(-2)^3", "-8");
    test_calc(ct, "1+(-2)^3", "-7");
    test_calc(ct, "(-2)^2", "4");
    test_calc(ct, "1+(-2)^2", "5");
    test_calc(ct,"sin(1)tg(2)", "ERROR");
    test_calc(ct, "2^2^2", "16");
    test_calc(ct, "2^2^3", "64");

    std::cout << std::endl;
    if (!lost_) {
        std::cout << ":)_OK_ALL_TESTS_PASSED_OK_:)" << std::endl << std::endl;
    } else {
        std::cout << ":(_NOT_ALL_TESTS_PASSED_:(" << std::endl << std::endl;
    }
    std::cout << "PASSED: " << passed_ << " LOST: " << lost_ << " ALL: " << passed_ + lost_ << std::endl;
    std::cout << "\\--------TESTING_END--------/" << std::endl << std::endl;
}

#endif // TEST_COMPUTING_HPP
