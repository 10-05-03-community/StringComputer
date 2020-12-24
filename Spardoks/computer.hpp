// Copyright 2020 Spardoks

#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>


class Computer {
public:
    Computer();
    std::string get_result(const std::string& expression, int cmd_mode = 0, std::map<std::string, long double> var_values = std::map<std::string, long double>());
private:
    bool erflag;

    void remove_spaces(std::string& expr);
    void check_plus_minus_balance(std::string& expr);
    void check_waste_dots(std::string& expr);
    void check_braces(std::string& expr);
    void check_lack_of_operands(std::string& expr);
    void check_vars(std::string& expr, int cmd_mode, std::map<std::string, long double>& var_values);
    void replace_vars(std::string& expr, std::map<std::string, long double>& var_values);
    void get_vars_from_console(std::map<std::string, long double>& var_values);

    void compute(std::string& expr);
    void do_operation(std::string& expr, std::string::size_type pos_operation);
    void do_function(std::string& expr, std::string::size_type pos_end_name_of_function);
};


#endif // COMPUTER_HPP
