// Создаем файл, в котором описан класс калькулятора
#ifndef Calculate_HPP
#define Calculate_HPP

// Подключаем файл с константами
#define _USE_MATH_DEFINES

// Подключаем необходимые библиотеки
#include <cmath>
#include <iterator>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>

using std::cin;
using std::cout;
using std::istringstream;
using std::vector;
using std::string;
using std::stringstream;
using std::to_string;
using std::multimap;

// Структура операции
struct operation { 
    int order;  // 2 + 3 - 5 * 4
    char operation;
};


// Класс калькулятора
class Calculator {
public:

    // Главная функция проверки, вычисления вводимого выражения и вывода на экран ответа
    void computer() {

        // Строка, в которую будет записываться выражение
        string expression;

        // вывод правил записи выражения
        Notation_Rules();

        getline(cin, expression, '\n');

        // Удаление пробелов из строчки выражения
        for (int i = 0; i < expression.size(); i++) {
            if (expression[i] == ' ') {
                expression.erase(i, 1);
                i--;
            }
        }

        // Проверка правильности выражения
        if (validation(expression)) {
            cout << "Ваш ответ:\n";

            cout << to_string(calculate(expression)) << "\n";
        }
        else {
            cout << "Error\n";
        }
    }

private:
    // Правила записи выражения
    void Notation_Rules() {
        cout << "\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: Введите выражение :::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: Выражение может содержать следующие операции и функции: :\n";
        cout << "\t\t:::: + - сложение; :::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: - - вычетание/унарный минус; ::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: * - умножение; ::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: / - деление; ::::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: () - взятие выражение в скобки; :::::::::::::::::::::::::\n";
        cout << "\t\t:::: sqrt(x) - извлечение квадратного корень из числа x; :::::\n";
        cout << "\t\t:::: ^n - возведение в n-ую степень; :::::::::::::::::::::::::\n";
        cout << "\t\t:::: Exp(n) - возведение числа e в n-ую степень; :::::::::::::\n";
        cout << "\t\t:::: sin(x) - взятие синуса от числа x; ::::::::::::::::::::::\n";
        cout << "\t\t:::: cos(x) - взятие косинуса от числа x; ::::::::::::::::::::\n";
        cout << "\t\t:::: tan(x) - взятие тангенса от числа x :::::::::::::::::::::\n";
        cout << "\t\t:::: cot(x) - взятие сотангенса от числа x; ::::::::::::::::::\n";
        cout << "\t\t:::: Обозначение констант: :::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: Pi - число Пи; ::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: e - число e; ::::::::::::::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: Обозначение переменных: :::::::::::::::::::::::::::::::::\n";
        cout << "\t\t:::: x; ::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    }

    // Проверка синтаксиса выражения
    bool validation(string expression) {

        // Строчка не должна быть пустой
        if (expression.size() == 0) {
            return false;
        }

        if (expression[0] != 's' and expression[0] != 'c' and expression[0] != 'E' and expression[0] != 't' and
            expression[0] != 'P' and expression[0] != 'e' and expression[0] != 'x' and expression[0] != '(' and
            expression[0] != '-' and (int(expression[0]) >= 58 or int(expression[0]) <= 47)) {
            return false;
        }

        int brackets = 0;

        // Цикл перебора символов и провеки их синтаксиса
        while (expression != "") {

            // Если символ цифра
            if (int(expression[0]) >= 48 and int(expression[0]) <= 57) {
                // Выделяем число из строчки
                istringstream iss(expression, istringstream::in); 
                double num;
                iss >> num; 
                
                // Преобразуем число в строчку
                stringstream ss;
                ss << num; 
                string str = ss.str(); 

                // Удаляем текст, который являлся числом
                expression.erase(0, str.length()); 

                if (expression[0] != '+' and expression[0] != '-' and expression[0] != '*' and expression[0] != '/' and expression[0] != '^' and expression[0] != ')' and expression.size() != 0) {
                    return false;
                }
            }

            else if (expression[0] == '+' or expression[0] == '-' or expression[0] == '*' or expression[0] == '/' or expression[0] == '^') {

                expression.erase(0, 1);

                if (expression[0] != 's' and expression[0] != 'c' and expression[0] != 'E' and expression[0] != 't' and
                    expression[0] != 'P' and expression[0] != 'e' and expression[0] != 'x' and expression[0] != '(' and
                    (int(expression[0]) >= 58 or int(expression[0]) <= 47)) {
                    return false;
                }

            }

            else if (expression[0] == '(') {

                expression.erase(0, 1);

                brackets++;

                if (expression[0] != 's' and expression[0] != 'c' and expression[0] != 'E' and expression[0] != 't' and
                    expression[0] != 'P' and expression[0] != 'e' and expression[0] != 'x' and expression[0] != '(' and
                    expression[0] != '-' and (int(expression[0]) >= 58 or int(expression[0]) <= 47)) {
                    return false;
                }
            }

            else if (expression[0] == ')') {
                expression.erase(0, 1);

                brackets--; 

                if (brackets < 0) {
                    return false;
                }

                if (expression[0] != '+' and expression[0] != '-' and expression[0] != '*' and expression[0] != '/' and expression[0] != '^' and expression[0] != ')' and expression.size() != 0) {
                    return false;
                }
            }

            else if (expression[0] == 'P' and expression[1] == 'i') {
                expression.erase(0, 2);

                if (expression[0] != '+' and expression[0] != '-' and expression[0] != '*' and expression[0] != '/' and expression[0] != '^' and expression[0] != ')' and expression.size() != 0) {
                    return false;
                }
            }

            else if (expression[0] == 'e') {
                expression.erase(0, 1);

                if (expression[0] != '+' and expression[0] != '-' and expression[0] != '*' and expression[0] != '/' and expression[0] != '^' and expression[0] != ')' and expression.size() != 0) {
                    return false;
                }
            }

            else if (expression[0] == 'x') {
                expression.erase(0, 1);

                if (expression[0] != '+' and expression[0] != '-' and expression[0] != '*' and expression[0] != '/' and expression[0] != '^' and expression[0] != ')' and expression.size() != 0) {
                    return false;
                }
            }

            else if (expression[0] == 's' and expression[1] == 'i' and expression[2] == 'n') {

                expression.erase(0, 3);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else if (expression[0] == 'c' and expression[1] == 'o' and expression[2] == 's') {
                expression.erase(0, 3);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else if (expression[0] == 't' and expression[1] == 'a' and expression[2] == 'n') {
                expression.erase(0, 3);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else if (expression[0] == 'c' and expression[1] == 'o' and expression[2] == 't') {
                expression.erase(0, 3);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else if (expression[0] == 's' and expression[1] == 'q' and expression[2] == 'r' and expression[3] == 't') {
                expression.erase(0, 4);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else if (expression[0] == 'E' and expression[1] == 'x' and expression[2] == 'p') {
                expression.erase(0, 3);

                if (expression[0] != '(') {
                    return false;
                }
            }

            else {
                return false;
            }
        }

        if (brackets != 0) {
            return false;
        }

        return true;
    }

    // Функция, ответственная за сдвиг операций в массиве
    void balance(multimap<int, operation>& operations, multimap<int, operation>::iterator iter) {
        int order = iter->second.order;
        for (; iter != operations.end(); iter++) {
            if (iter->second.order > order) {
                iter->second.order -= 1;
            }
        }
    }

    // Функция подсчета выражения
    double calculate(string& expression, bool request_x = false, double x = 0) {
        multimap<int, operation> operations;
        vector <double> numbers;

        int brackets = 0;
        int znak = 1;

        while (expression != "") {

            if (int(expression[0]) >= 48 and int(expression[0]) <= 57) {
                istringstream iss(expression, istringstream::in);
                double num;
                iss >> num;

                stringstream ss;
                ss << num;
                string str = ss.str();

                expression.erase(0, str.length());

                num = znak * num;
                znak = 1;
                numbers.push_back(num);

            }

            else if (expression[0] == '(') {
                brackets -= 100;
                expression.erase(0, 1);
            }

            else if (expression[0] == ')') {
                brackets += 100;
                expression.erase(0, 1);
            }

            else if (expression[0] == '+') {
                operation Plus;
                Plus.operation = '+';
                Plus.order = operations.size();
                operations.insert({ 3 + brackets, Plus });
                expression.erase(0, 1);
            }

            else if (expression[0] == '-') {
                if (operations.size() - numbers.size() == 0) {
                    znak = -1;
                    expression.erase(0, 1);
                }
                else {
                    operation Minus;
                    Minus.operation = '-';
                    Minus.order = operations.size();
                    operations.insert({ 3 + brackets, Minus });
                    expression.erase(0, 1);
                }
            }

            else if (expression[0] == '*') {
                operation Multiplication;
                Multiplication.operation = '*';
                Multiplication.order = operations.size();
                operations.insert({ 2 + brackets, Multiplication });
                expression.erase(0, 1);
            }

            else if (expression[0] == '/') {
                operation Division;
                Division.operation = '/';
                Division.order = operations.size();
                operations.insert({ 2 + brackets, Division });
                expression.erase(0, 1);
            }

            else if (expression[0] == '^') {
                operation Degree;
                Degree.operation = '^';
                Degree.order = operations.size();
                operations.insert({ 1 + brackets, Degree });
                expression.erase(0, 1);
            }

            else if (expression[0] == 'E' and expression[1] == 'x' and expression[2] == 'p') {
                double value;
                int size;
                int count = 1; 
                string new_expression = "";

                for (int i = 4; i < expression.size(); i++) {

                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;
                        if (count == 0) {
                            size = new_expression.size();
                            value = znak * pow(M_E, (calculate(new_expression, request_x, x)));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }
                numbers.push_back(value);
                expression.erase(0, size + 5);
            }

            else if (expression[0] == 's' and expression[1] == 'q' and expression[2] == 'r' and expression[3] == 't') {
                double value;
                int size;
                int count = 1;
                string new_expression = "";

                for (int i = 5; i < expression.size(); i++) {
                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;

                        if (count == 0) {
                            size = new_expression.size();
                            value = znak * sqrt(calculate(new_expression, request_x, x));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }

                numbers.push_back(value);
                expression.erase(0, size + 6);

            }

            else if (expression[0] == 's' and expression[1] == 'i' and expression[2] == 'n') {
                double value;
                int size;
                int count = 1;
                string new_expression = "";

                for (int i = 4; i < expression.size(); i++) {
                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;

                        if (count == 0) {
                            size = new_expression.size();
                            value = znak * sin(calculate(new_expression, request_x, x));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }
                numbers.push_back(value);
                expression.erase(0, size + 5);
            }

            else if (expression[0] == 'c' and expression[1] == 'o' and expression[2] == 's') {
                double value;
                int size;
                int count = 1;
                string new_expression = "";

                for (int i = 4; i < expression.size(); i++) {
                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;

                        if (count == 0) {
                            size = new_expression.size();
                            value = znak* cos(calculate(new_expression, request_x, x));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }
                numbers.push_back(value);
                expression.erase(0, size + 5);
            }

            else if (expression[0] == 't' and expression[1] == 'a' and expression[2] == 'n') {
                double value;
                int size;
                int count = 1;
                string new_expression = "";

                for (int i = 4; i < expression.size(); i++) {
                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;

                        if (count == 0) {
                            size = new_expression.size();
                            value = znak * tan(calculate(new_expression, request_x, x));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }
                numbers.push_back(value);
                expression.erase(0, size + 5);
            }

            else if (expression[0] == 'c' and expression[1] == 'o' and expression[2] == 't') {
                double value;
                int size;
                int count = 1;
                string new_expression = "";

                for (int i = 4; i < expression.size(); i++) {
                    if (expression[i] == '(') {
                        count++;
                    }

                    if (expression[i] == ')') {
                        count--;

                        if (count == 0) {
                            size = new_expression.size();
                            value = znak * 1 / tan(calculate(new_expression, request_x, x));
                            znak = 1;
                            break;
                        }
                    }

                    new_expression += expression[i];
                }
                numbers.push_back(value);
                expression.erase(0, size + 5);
            }

            else if (expression[0] == 'P' and expression[1] == 'i') {
                numbers.push_back(znak * M_PI);
                znak = 1;
                expression.erase(0, 2);
            }

            else if (expression[0] == 'e') {
                numbers.push_back(znak * M_E);
                znak = 1;
                expression.erase(0, 1);
            }

            else if (expression[0] == 'x') { 
                if (request_x == false) {
                    cout << "Введите значение переменной x\n";
                    cin >> x;
                    request_x = true;
                }

                numbers.push_back(znak * x);
                znak = 1;

                expression.erase(0, 1);
            }
        }


        for (auto iter = operations.begin(); iter != operations.end(); iter++) {
            if (iter->second.operation == '+') {
                numbers[iter->second.order] += numbers[iter->second.order + 1];
                numbers.erase(numbers.begin() + iter->second.order + 1);
            }
            else if (iter->second.operation == '-') {
                numbers[iter->second.order] -= numbers[iter->second.order + 1];
                numbers.erase(numbers.begin() + iter->second.order + 1);
            }
            else if (iter->second.operation == '*') {
                numbers[iter->second.order] *= numbers[iter->second.order + 1];
                numbers.erase(numbers.begin() + iter->second.order + 1);
            }
            else if (iter->second.operation == '/') {
                numbers[iter->second.order] /= numbers[iter->second.order + 1];
                numbers.erase(numbers.begin() + iter->second.order + 1);
            }
            else if (iter->second.operation == '^') {
                numbers[iter->second.order] = pow(numbers[iter->second.order], numbers[iter->second.order + 1]);
                numbers.erase(numbers.begin() + iter->second.order + 1);
            }

            balance(operations, iter);
        }
        return numbers[0];
    }

};

#endif
