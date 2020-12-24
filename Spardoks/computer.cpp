// Copyright 2020 Spardoks

#include "computer.hpp"

Computer::Computer() {
    erflag = false;
}

std::string Computer::get_result(const std::string& expression, int cmd_mode, std::map<std::string, long double> var_values) {

    // для упрощения анализа и вычислений
    std::string expr = '(' + expression + ')';

    /// VALIDATION ///
    // Удаление пробелов
    remove_spaces(expr);

    // Уничтожение лишних знаков +- // +++--- <--> -
    check_plus_minus_balance(expr);

    // Проверка на лишние точки
    erflag = false;
    check_waste_dots(expr);
    if (erflag) {
        expr = "ERROR";
        return expr;
    }

    // Проверка скобок
    erflag = false;
    check_braces(expr);
    if (erflag) {
        expr = "ERROR";
        return expr;
    }

    // Проверка на нехватку операндов
    erflag = false;
    check_lack_of_operands(expr);
    if (erflag) {
        expr = "ERROR";
        return expr;
    }

    // Проверка на переменные и получение для них значений, подстановка переменных и констант, исключение лишних символов в выражении
    erflag = false;
    check_vars(expr, cmd_mode, var_values);
    if (erflag) {
        expr = "ERROR";
        return expr;
    }
    /// VALIDATION_END //

    /// COMPUTING ///
    // Подсчёт
    compute(expr);
    /// COMPUTING_END ///

    erflag = false;
    return expr;
}

/// VALIDATION ///

void Computer::remove_spaces(std::string& expr) {

    std::string::size_type pos_of_substr = expr.find(' ');
    while(pos_of_substr != std::string::npos) {
        expr.erase(pos_of_substr, 1);
        pos_of_substr = expr.find(' ');
    }
}

void Computer::check_plus_minus_balance(std::string& expr) {

    // удалить лишние плюсы и -

    // после - и +
    while ((expr.find("--") != std::string::npos) || ((expr.find("-+") != std::string::npos))
             || (expr.find("+-") != std::string::npos) || (expr.find("++") != std::string::npos)) {
        std::string::size_type pos_of_substr = expr.find("--");
        while(pos_of_substr != std::string::npos) {
            expr.replace(pos_of_substr, 2, "+");
            pos_of_substr = expr.find("--");
        }

        pos_of_substr = expr.find("++");
        while(pos_of_substr != std::string::npos) {
            expr.replace(pos_of_substr, 2, "+");
            pos_of_substr = expr.find("++");
        }

        pos_of_substr = expr.find("+-");
        while(pos_of_substr != std::string::npos) {
            expr.replace(pos_of_substr, 2, "-");
            pos_of_substr = expr.find("+-");
        }

        pos_of_substr = expr.find("-+");
        while(pos_of_substr != std::string::npos) {
            expr.replace(pos_of_substr, 2, "-");
            pos_of_substr = expr.find("-+");
        }
    }

    // в начале скобок
    std::string::size_type pos_of_substr = expr.find("(+");
    while (pos_of_substr != std::string::npos) {
        expr.replace(pos_of_substr, 2, "(");
        pos_of_substr = expr.find("(+");
    }

    // после *
    pos_of_substr = expr.find("*+");
    while (pos_of_substr != std::string::npos) {
        expr.replace(pos_of_substr, 2, "*");
        pos_of_substr = expr.find("*+");
    }

    // после /
    pos_of_substr = expr.find("/+");
    while (pos_of_substr != std::string::npos) {
        expr.replace(pos_of_substr, 2, "/");
        pos_of_substr = expr.find("/+");
    }

    // после ^
    pos_of_substr = expr.find("^+");
    while (pos_of_substr != std::string::npos) {
        expr.replace(pos_of_substr, 2, "^");
        pos_of_substr = expr.find("^+");
    }

}

void Computer::check_waste_dots(std::string& expr) {

    // слева и справа от точки хотя бы по 1 цифре, между 2 точками есть хотя бы 1 действие

    // поиск перой точки
    std::string::size_type pos_dot = expr.find('.', 0);

    // пока есть точки, проверяем их
    while(pos_dot != std::string::npos) {
        // слева должна быть цифра
        bool was_digit_left = (pos_dot > 0) && ((expr[pos_dot - 1] >= '0') && (expr[pos_dot - 1] <= '9'));
        // справа должна быть цифра
        bool was_digit_right = (pos_dot < expr.size()) && ((expr[pos_dot + 1] >= '0') && (expr[pos_dot + 1] <= '9'));
        if (!was_digit_left || !was_digit_right) {
            // точка без цифры справа или слева
            erflag = true;
            return;
        }
        // ищем новую точку
        std::string::size_type pos_dot_new = expr.find('.', pos_dot + 1);
        if (pos_dot_new == std::string::npos) {
            return;
        }
        // между ними должен быть хотя бы 1 оператор
        bool was_operator = false;
        for (std::string::size_type i = pos_dot + 1; i < pos_dot_new; ++i) {
            if ((expr[i] == '^') || (expr[i] == '*') || (expr[i] == '+') || (expr[i] == '-')) {
                was_operator = true;
                break;
            }
        }
        if (!was_operator) {
            // две точки подряд
            erflag = true;
            return;
        }
        pos_dot = pos_dot_new;
    }

}

void Computer::check_braces(std::string& expr) {

    // проверка на наличие пустых
    if (expr.find("()") != std::string::npos) {
        // ошибка, есть пустые скобки
        erflag = true;
        return;
    }
    // проверка на нехватку закрывающихся
    int balance = 0;
    for (size_t i = 0; i < expr.size(); ++i) {
        if(expr[i] == '(')
            ++balance;
        else if(expr[i] == ')')
            --balance;
        if (balance < 0) {
            // лишняя закрывающая скобка
            erflag = true;
            return;
        }
    }
    if (balance != 0) {
        // лишняя скобка
        erflag = true;
        return;
    }
}

void Computer::check_lack_of_operands(std::string& expr) {

    // проверяем для основных
    const char operations[4] = {'^', '*', '/', '+'};
    // слева и справа от операторов должно быть по операнду
    for (size_t i = 0; i < 4; ++i) {
        std::string::size_type pos_operator = expr.find(operations[i], 0);
        while (pos_operator != std::string::npos) {
            // проверка слева
            if ( !((expr[pos_operator - 1] >= '0') && (expr[pos_operator - 1] <= '9'))) {
                if (!((expr[pos_operator - 1] >= 'A') && (expr[pos_operator - 1] <= 'Z'))) {
                    if (!((expr[pos_operator - 1] >= 'a') && (expr[pos_operator - 1] <= 'z'))) {
                        if (!(expr[pos_operator - 1] == ')')) {
                            // нет операнда слева
                            erflag = true;
                            return;
                        }
                    }
                }
            }

            // проверка справа
            if ( !((expr[pos_operator + 1] >= '0') && (expr[pos_operator + 1] <= '9'))) {
                if (!((expr[pos_operator + 1] >= 'A') && (expr[pos_operator + 1] <= 'Z'))) {
                    if (!((expr[pos_operator + 1] >= 'a') && (expr[pos_operator + 1] <= 'z'))) {
                        if (!(expr[pos_operator + 1] == '(')) {
                            if (!(expr[pos_operator + 1] == '-'))
                            // нет операнда справа
                            erflag = true;
                            return;
                        }
                    }
                }
            }

            // переход к следующему операнду
            pos_operator = expr.find(operations[i], pos_operator + 1);
        }
    }

    // отдельно проверим для -
    if ((expr.find("-)") != std::string::npos) || (expr.find("-^") != std::string::npos)) {
        // висячий минус
        erflag = true;
        return;
    }

    // проверка на )"letter"
    std::string::size_type pos_brace = expr.find(')',0);
    while(pos_brace != (expr.size() - 1)) {
        if ((expr[pos_brace + 1] >= 'a' && expr[pos_brace + 1] <= 'z')
             || (expr[pos_brace + 1] >= 'A' && expr[pos_brace + 1] <= 'Z') ) {
            erflag = true;
            return;
        }
        pos_brace = expr.find(')', pos_brace + 1);
    }
}

void Computer::check_vars(std::string& expr, int cmd_mode, std::map<std::string, long double>& var_values) {

    // поиск переменных
    // PI, E - const
    // считаем, что переменные могут состоять из символов A..Z, a..z, 0..9
    // считаем, что имена могут начинаться только с буквы
    std::string name_of_var;
    bool was_name = false;
    for (size_t i = 0; i < expr.size(); ++i) {

        if (((expr[i] >= 'a') && (expr[i] <= 'z')) || ((expr[i] >= 'A') && (expr[i] <= 'Z'))) {
            name_of_var += expr[i];
            was_name = true;
        } else if (was_name &&(expr[i] >= '0') && (expr[i] <= '9') ) {
            name_of_var += expr[i];
        } else if (was_name &&
                   (  (expr[i] == '^') || (expr[i] == '*') || (expr[i] == '/')
                   || (expr[i] == '+') || (expr[i] == '-')
                   || (expr[i] == '(') || (expr[i] == ')')) ) {

            // если ещё нет такой переменной, то добавляем
            if (var_values.find(name_of_var) == var_values.end()) {

                // пропускаем имена известных функций
                if ((name_of_var.compare("sin") == 0) || (name_of_var.compare("cos") == 0)
                    || (name_of_var.compare("tg") == 0) || (name_of_var.compare("ctg") == 0)
                    || (name_of_var.compare("exp") == 0) || (name_of_var.compare("sqrt") == 0)) {

                    name_of_var = "";
                    was_name = false;
                    continue;
                }

                // проверяем, не пытаются ли вызвать неизвестную функцию
                if (expr[i] == '(') {
                    // нет такой функции
                    erflag = true;
                    return;
                }

                // если константы, то инициализируем их стандартными значениями, иначе пока 0
                if (name_of_var.compare("PI") == 0) {
                    var_values.insert(std::make_pair("PI", M_PI));
                } else if ((name_of_var.compare("E") == 0)) {
                    var_values.insert(std::make_pair("E", M_E));
                } else {
                    if (cmd_mode) {
                        // нет значения для переменной в режиме cmd
                        erflag = true;
                        return;
                    }
                    // добавляем переменную в map
                    var_values.insert(std::make_pair(name_of_var, 0));
                }
            }
            name_of_var = "";
            was_name = false;
        } else {
            if (was_name) {
                // нераспознанный символ в выражении
                erflag = true;
                return;
            } else {
                std::string symbols = "0123456789()*/+-^";
                if (symbols.find(expr[i]) == std::string::npos) {
                    // нераспознанный символ в выражении
                    erflag = true;
                    return;
                }
            }
        }
    }

    // если есть переменные, то подставляем вместо них значения
    if (!var_values.empty()) {
        if (!cmd_mode) {
            // ввод значений с клавиатуры
            get_vars_from_console(var_values);
        }
        // подстановка из map
        replace_vars(expr, var_values);
    }
}

void Computer::replace_vars(std::string& expr, std::map<std::string, long double>& var_values) {

    // константы PI и E заменялись на прошлом шаге при парсе переменных

    // перед заменой проведём проверку, что имя переменной не совпадает с названием части какой-либо функции
    // для корректной замены

    for (auto it = var_values.begin(); it != var_values.end(); ++it) {
        std::string::size_type pos_var = expr.find(it->first);
        while(pos_var != std::string::npos) {

            // проверим, что находится за именем переменной справа и слева
            // не должно быть букв и цифр

            // слева
            if ( ((expr[pos_var - 1] >= 'a') && (expr[pos_var - 1] <= 'z'))
                 || ((expr[pos_var - 1] >= 'A') && (expr[pos_var - 1] <= 'Z'))
                 || ((expr[pos_var - 1] >= '0') && (expr[pos_var - 1] <= '9')) ) {

                pos_var = expr.find(it->first, pos_var + 1);
                continue;
            }

            // справа
            if ( ((expr[pos_var + it->first.size()] >= 'a') && (expr[pos_var + it->first.size()] <= 'z'))
                 || ((expr[pos_var + it->first.size()] >= 'A') && (expr[pos_var + it->first.size()] <= 'Z'))
                 || ((expr[pos_var + it->first.size()] >= '0') && (expr[pos_var + it->first.size()] <= '9')) ) {

                pos_var = expr.find(it->first, pos_var + it->first.size());
                continue;
            }
            // вставляем в скобках, чтоб избежать проблем с вычислением степени
            expr.replace(pos_var, it->first.size(), '(' + std::to_string(it->second) + ')');
            pos_var = expr.find(it->first);
        }
    }
}

void Computer::get_vars_from_console(std::map<std::string, long double>& var_values) {

    for (auto it = var_values.begin(); it != var_values.end(); ++it) {
        if (it->second == 0) {
            bool good_input = false;
            while (!good_input) {
                std::cout << std::endl;
                std::cout << "Input value for " << it->first << ": ";
                double value = 0;
                std::cin >> value;
                if (std::cin.fail() || (std::cin.peek() != '\n')) {
                    std::cin.clear();
                    good_input = false;
                    std::cout << "Bad_value. Try again!" << std::endl;
                } else {
                    good_input = true;
                    it->second = value;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        }
    }
}

/// COMPUTING ///

void Computer::compute(std::string& expr) {

    std::string::size_type pos_deep_open_brace = 0;
    std::string::size_type pos_deep_close_brace = 0;
    std::string::size_type pos_operation = 0;

    // пока остались операции в скобках
    while (expr.find(')') != std::string::npos) {

        // поиск самых правых внутренних открывающихся скобок
        pos_deep_open_brace = expr.find('(', pos_deep_open_brace + 1);
        while (expr.find('(', pos_deep_open_brace + 1) != std::string::npos) {
            pos_deep_open_brace = expr.find('(', pos_deep_open_brace + 1);
        }
        pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);

        // операция ^
        pos_operation = expr.find('^', pos_deep_open_brace + 2);
        while ((pos_operation != std::string::npos) && (pos_operation < pos_deep_close_brace)) {

            // выполнение операции и подстановка значения обратно в строку
            do_operation(expr, pos_operation);

            // корректировка
            pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);
            pos_operation = expr.find('^', pos_deep_open_brace + 2);
        }

        // операции * и /
        pos_operation = expr.find('*', pos_deep_open_brace + 2);
        while ((pos_operation != std::string::npos) && (pos_operation < pos_deep_close_brace)) {

            // если / раньше, чем *
            std::string::size_type pos_op2 = expr.find('*', pos_deep_open_brace + 2);
            if(((pos_op2 != std::string::npos) && (pos_op2 < pos_operation))) {
                pos_operation = pos_op2;
            }

            // выполнение операции и подстановка значения обратно в строку
            do_operation(expr, pos_operation);

            // корректировка
            pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);
            pos_operation = expr.find('*', pos_deep_open_brace + 2);
        }

        // если осталось /
        pos_operation = expr.find('/', pos_deep_open_brace + 2);
        while ((pos_operation != std::string::npos) && (pos_operation < pos_deep_close_brace)) {

            // выполнение операции и подстановка значения обратно в строку
            do_operation(expr, pos_operation);

            // корректировка
            pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);
            pos_operation = expr.find('/', pos_deep_open_brace + 2);
        }

        // операции + и -
        pos_operation = expr.find('+', pos_deep_open_brace + 2);
        while ((pos_operation != std::string::npos) && (pos_operation < pos_deep_close_brace)) {

            // если - раньше, чем +
            std::string::size_type pos_op2 = expr.find('-', pos_deep_open_brace + 2);
            if(((pos_op2 != std::string::npos) && (pos_op2 < pos_operation))) {
                pos_operation = pos_op2;
            }

            // выполнение операции и подстановка значения обратно в строку
            do_operation(expr, pos_operation);

            // корректировка
            pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);
            pos_operation = expr.find('+', pos_deep_open_brace + 2);
        }

        // если остался -
        pos_operation = expr.find('-', pos_deep_open_brace + 2);
        while ((pos_operation != std::string::npos) && (pos_operation < pos_deep_close_brace)) {

            // выполнение операции и подстановка значения обратно в строку
            do_operation(expr, pos_operation);

            // корректировка
            pos_deep_close_brace = expr.find(')', pos_deep_open_brace + 2);
            pos_operation = expr.find('-', pos_deep_open_brace + 2);
        }

        // замена скобок и внутреннего числа
        if (pos_deep_open_brace != 0) {
            // проверить на функцию слева за скобками и заменить на значение функции
            // также проверить, не возводится ли отрицательное число в степень
            // или просто раскрыть скобки
            do_function(expr, pos_deep_open_brace - 1);
        } else {
            // конечная замена
            expr = expr.substr(1, pos_deep_close_brace - 1);
            // удаление лишних нулей в дроби
            if (expr.find('.') != std::string::npos) {
                std::string::size_type pos0 = expr.size() - 1;
                while((pos0 != 0) && (expr[pos0] == '0')) {
                    --pos0;
                }
                if (pos0 != (expr.size() - 1)) {
                    if (expr[pos0] == '.')
                        --pos0;
                    expr = expr.substr(0, pos0 + 1);
                }
            }
            // -0 = 0
            if(expr.compare("-0") == 0) {
                expr = "0";
            }
        }
    }
}

void Computer::do_operation(std::string& expr, std::string::size_type pos_operation) {

    // поиск начала левого операнда
    std::string::size_type pos_begin_left_operand = pos_operation - 1;
    while (((expr[pos_begin_left_operand] >= '0') && ((expr[pos_begin_left_operand] <= '9')))
              || expr[pos_begin_left_operand] == '.') {
        --pos_begin_left_operand;
    }

    // для работы с nan и inf слева
    if (((expr[pos_begin_left_operand] == 'n') && (expr[pos_begin_left_operand - 1] == 'a') && (expr[pos_begin_left_operand - 2] == 'n'))
        || ((expr[pos_begin_left_operand] == 'f') && (expr[pos_begin_left_operand - 1] == 'n') && (expr[pos_begin_left_operand - 2] == 'i'))) {
        pos_begin_left_operand -= 3;
    }

    // считаем, что отрицательным левый операнд после преобразований может быть только в начале скобок и не относится к операции ^
    if ((expr[pos_begin_left_operand] == '-') && (expr[pos_begin_left_operand - 1] == '(') && (expr[pos_operation] != '^'))
        --pos_begin_left_operand;

    // поиск конца правого операнда
    std::string::size_type pos_end_right_operand = pos_operation + 1;
    while (((expr[pos_end_right_operand] >= '0') && ((expr[pos_end_right_operand] <= '9')))
              || expr[pos_end_right_operand] == '.') {
        ++pos_end_right_operand;
    }
    // если наткнулись началом правого операнда на -
    if ((expr[pos_end_right_operand] == '-') && (pos_end_right_operand == (pos_operation + 1))){
        ++pos_end_right_operand;
        while (((expr[pos_end_right_operand] >= '0') && ((expr[pos_end_right_operand] <= '9')))
                  || expr[pos_end_right_operand] == '.') {
            ++pos_end_right_operand;
        }
    }

    // для работы с nan и inf справа
    if (((expr[pos_end_right_operand] == 'n') && (expr[pos_end_right_operand + 1] == 'a') && (expr[pos_end_right_operand + 2] == 'n'))
        || ((expr[pos_end_right_operand] == 'i') && (expr[pos_end_right_operand + 1] == 'n') && (expr[pos_end_right_operand + 2] == 'f'))) {
        pos_end_right_operand += 3;
    }

    // вычленение
    // pos_begin_left_operand указывает на символ перед нашим левым операндом
    // pos_end_right_operand указывает на символ после нашего правого операнда
    long double left_op = std::stold(expr.substr(pos_begin_left_operand + 1, pos_operation - pos_begin_left_operand - 1));
    long double right_op = std::stold(expr.substr(pos_operation + 1, pos_end_right_operand - pos_operation - 1));

    // подсчёт
    long double res = 0;
    switch (expr[pos_operation]) {
    case '^': res = pow(left_op, right_op); break;
    case '*': res = left_op * right_op; break;
    case '/': res = left_op / right_op; break;
    case '+': res = left_op + right_op; break;
    case '-': res = left_op - right_op; break;
    default: break;
    }

    // замена подстроки значением
    std::string res_str = std::to_string(res);
    expr.replace(pos_begin_left_operand + 1, pos_end_right_operand - pos_begin_left_operand - 1, res_str);

    // могут появиться лишние знаки, убираем
    check_plus_minus_balance(expr);
}

void Computer::do_function(std::string &expr, std::string::size_type pos_end_name_of_function) {

    // получаем операнд
    size_t len_of_operand = expr.find(')', pos_end_name_of_function + 2) - (pos_end_name_of_function + 2);
    long double operand = std::stold(expr.substr(pos_end_name_of_function + 2, len_of_operand));

    // находим левую границу названия функции, правую знаем
    std::string::size_type pos_begin_name_of_function = pos_end_name_of_function;
    while ((expr[pos_begin_name_of_function] >= 'a') && (expr[pos_begin_name_of_function] <= 'z')) {
        --pos_begin_name_of_function;
    }

    long double res = 0;

    // получаем имя функции
    // pos_begin_name_of_function указывает на символ перед началом имени функции
    std::string name_of_function = expr.substr(pos_begin_name_of_function + 1, pos_end_name_of_function - pos_begin_name_of_function);

    // выполняем функцию
    if (name_of_function.compare("sin") == 0) {
        res = sin(operand);
    } else if ((name_of_function.compare("cos") == 0)) {
        res = cos(operand);
    } else if ((name_of_function.compare("tg") == 0)) {
        res = tan(operand);
    } else if ((name_of_function.compare("ctg") == 0)) {
        res = 1./tan(operand);
    } else if ((name_of_function.compare("exp") == 0)) {
        res = exp(operand);
    } else if ((name_of_function.compare("sqrt") == 0)) {
        res = sqrt(operand);
    } else {
        // просто скобки раскрываем, но проверяем, не возводится ли в степень
        if (expr[expr.find(')', pos_end_name_of_function + 2) + 1] == '^') {
            // поиск конца правого операнда
            std::string::size_type pos_operation = expr.find(')', pos_end_name_of_function + 2) + 1;
            std::string::size_type pos_end_right_operand = pos_operation + 1;
            while (((expr[pos_end_right_operand] >= '0') && ((expr[pos_end_right_operand] <= '9')))
                      || expr[pos_end_right_operand] == '.') {
                ++pos_end_right_operand;
            }
            // если наткнулись началом правого операнда на -
            if ((expr[pos_end_right_operand] == '-') && (pos_end_right_operand == (pos_operation + 1))){
                ++pos_end_right_operand;
                while (((expr[pos_end_right_operand] >= '0') && ((expr[pos_end_right_operand] <= '9')))
                          || expr[pos_end_right_operand] == '.') {
                    ++pos_end_right_operand;
                }
            }

            // для работы с nan и inf справа
            if (((expr[pos_end_right_operand] == 'n') && (expr[pos_end_right_operand + 1] == 'a') && (expr[pos_end_right_operand + 2] == 'n'))
                || ((expr[pos_end_right_operand] == 'i') && (expr[pos_end_right_operand + 1] == 'n') && (expr[pos_end_right_operand + 2] == 'f'))) {
                pos_end_right_operand += 3;
            }

            long double right_op = std::stold(expr.substr(pos_operation + 1, pos_end_right_operand - pos_operation - 1));

            res = pow(operand, right_op);

            // замена подстроки значением
            std::string res_str = '(' + std::to_string(res) + ')';
            expr.replace(pos_begin_name_of_function + 1, pos_end_right_operand - pos_begin_name_of_function  - 1, res_str);

            // могут появиться лишние знаки, убираем
            check_plus_minus_balance(expr);
            return;

        }
        res = operand;
        // просто скобки раскрываем
        std::string res_string = std::to_string(res);
        expr.replace(pos_begin_name_of_function + 1, len_of_operand + 2 + pos_end_name_of_function - pos_begin_name_of_function, res_string);

        // могут появиться лишние знаки, убираем
        check_plus_minus_balance(expr);
        return;

    }

    // возвращаем в скобках, чтоб правильно возвести в степень, если она будет
    std::string res_string = '(' + std::to_string(res) + ')';
    expr.replace(pos_begin_name_of_function + 1, len_of_operand + 2 + pos_end_name_of_function - pos_begin_name_of_function, res_string);

    // могут появиться лишние знаки, убираем
    check_plus_minus_balance(expr);
}
