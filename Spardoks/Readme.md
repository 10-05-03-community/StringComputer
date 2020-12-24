# Договорённости

## Состав expression
- +, -, *, /
- унарный -
- sin, cos, tg, ctg
- exp, sqrt, ^
- pi, e
- vars
- ()
- считаем, что все значения только в R
- считаем, что углы в тригонометрические фукнции подаются в радианах
- считаем, что все операнды в expression типа long double, то есть значения выражения в диапазонах этого типа
- считаем, что функция обязательно принимает аргумент в круглых скобках и не может быть пустой
- считаем, что дробные числа отделяются точкой
- считаем, что PI, E - const
- считаем, что переменные могут состоять из символов A..Z, a..z, 0..9
- считаем, что имена переменных могут начинаться только с буквы

## План обработки expression:
0. Удаление пробелов
1. Уничтожение лишних знаков +- (и после каждой операции)
2. Проверка на лишние точки
3. Проверка скобок, поиск пустых
4. Проверка на нехватку операндов
5. Проверка на переменные и получение для них значений, подстановка
6. Расчёт выражения пока не отсанется операций

   a) операции во внутренних скобках
   
   b) вычисление функций тригонометрических, экспоненты, корня, степеней
   
   c) выполнение *, / в порядке следования
   
   d) выполнение +, - в порядке следования
7. Уничтожить у ответа лишние нули и плюсы