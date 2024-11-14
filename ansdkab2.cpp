#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

class Number {
private:
    char* value; // number
    long long size; // size of number

public:

    Number() : value(nullptr), size(0) {} //default constructor 

    ~Number() 
    {
        delete[] value;
        value = nullptr;
    }

    Number(const char* num) //constructor with string
    {
        size = strlen(num); 
        value = new char[size+1];
        for (int i = 0; i != size; ++i)
        {
            if (num[i] >= '0' || num[i] <= '9')
                value[i] = num[i];
        }
    } 

    Number(char* values, long long sz) : value(values), size(sz) {}

    Number(const Number& other) : size(other.size) {
        value = new char[size];
        memcpy(value, other.value, size * sizeof(char));
    }
    Number(Number&& other) noexcept : value(other.value), size(other.size)
    {
        other.value = nullptr; 
        other.size = 0;
    }
    Number& operator=(Number&& other) noexcept {
        if (this != &other) {
            delete[] value;

            value = other.value;
            size = other.size;

            other.value = nullptr;
            other.size = 0;
        }
        return *this;
    }
    void shift(char* num, long long polySize) const {
        for (long long i = 0; i < polySize - 1; ++i) {
            if ((num[i] - '0') >= 10) {         
                num[i + 1] += (num[i] - '0') / 10; 
                num[i] = ((num[i] - '0') % 10) + '0'; 
            }
        }
    } 

    Number operator*(const Number& other) const
    {
        long long resultsize = size + other.size; 
        char* resultNum = new char[resultsize];
        memset(resultNum, 0, resultsize*sizeof(char)); 
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < other.size; ++j)
            {
                resultNum[i + j] += (value[i] - '0') * (value[j] - '0');
            }
        }
        shift(resultNum, resultsize); 
        return Number(resultNum, resultsize);

    }

    Number operator*(const long long num) const
    {
        long long resultsize = size + 1;
        char* resultNum = new char[resultsize];
        memset(resultNum, 0, resultsize * sizeof(char));
        for (int i = 0; i < size; ++i)
        {
            resultNum[i] = value[i] * num;
        }

        shift(resultNum, resultsize);
        return Number(resultNum, resultsize);

    }


    Number powerOfTwo(int exponent) const {
        Number result("1");
        for (int i = 0; i < exponent; i++) {
            result = result * 2;  // Используем перегруженный оператор умножения
        }
        return result;
    }

    // Операция деления
    //Number divide(const Number& divisor) {
    //     Проверка на деление на ноль
    //    if (strcmp(divisor.value, "0") == 0) {
    //        throw std::runtime_error("Ошибка: деление на ноль");
    //    }

    //     Если делимое меньше делителя, частное равно 0
    //    if (*this < divisor) {
    //        return Number("0");
    //    }

    //     Определяем максимальное значение c, при котором 2^c * divisor <= *this
    //    int c = 0;

    //    while (!(Number(*this) >= (powerOfTwo(c) * divisor) && (Number(*this) < powerOfTwo(c + 1) * divisor))) {
    //        c++;
    //    }

    //     Рекурсивный вызов с вычитанием 2^c * divisor
    //    Number quotient = (Number(*this) - powerOfTwo(c) * divisor).divide(divisor) + powerOfTwo(c);

    //    return quotient;
    //}

    // Генерация случайного числа
    //void randomNum(int n) {
    //    srand(time(NULL));
    //    int i = 0;
    //    Number maximum = powerOfTwo(n);
    //    Number minimum = powerOfTwo(n - 1) + Number("1");

    //     Выравнивание строк по длине
    //    minimum.value = std::string(maximum.value.length() - minimum.value.length(), '0') + minimum.value;
    //    bool bigger = true, smaller = true;

    //     Формирование случайного числа в диапазоне от минимального до максимального
    //    for (int i = 0; i < maximum.value.length(); i++) {
    //        if (i == 0) {
    //            value += to_string(rand() % (maximum.value[0] - '0' - (minimum.value[0] - '0') + 1) + (minimum.value[0] - '0'));
    //            continue;
    //        }
    //        if (bigger && value[i - 1] == maximum.value[i - 1]) {
    //            value += to_string(rand() % (maximum.value[i] - '0'));
    //            smaller = false;
    //            continue;
    //        }
    //        if (smaller && value[i - 1] == minimum.value[i - 1]) {
    //            bigger = false;
    //            value += to_string(rand() % (9 - (minimum.value[i] - '0') + 1) + (minimum.value[i] - '0'));
    //            continue;
    //        }
    //        value += to_string(rand() % 10);
    //        bigger = false;
    //        smaller = false;
    //    }

    //    leadZero();  // исправленный вызов функции leadZero без аргумента
    //}


    void write() {
        for (int i = 0; i < size; i++) {
            cout << value[i] - '0';
        }
        cout << "\n";
    }

    // Удаление ведущих нулей
    //void leadZero() {
    //    int count = 0;
    //     Удаление ведущих нулей
    //    while (count < value.length() && value[count] == '0') {
    //        count++;
    //    }
    //     Обновляем значение объекта
    //    value = value.substr(count);
    //     Если строка пустая, устанавливаем значение "0"
    //    if (value.empty()) {
    //        value = "0";
    //    }
    //}

    // Переворот строки
    //string reverseStr(string str) {
    //    char ch;
    //    for (int index = 0, len = str.length(); index < len / 2; index++) {
    //        ch = str[index];
    //        str[index] = str[len - 1 - index];
    //        str[len - 1 - index] = ch;
    //    }
    //    return str;
    //}

    // Возведение в степень 2 (2^c)
    //Number powerOfTwo(int c) {
    //    if (c < 31) {
    //        return Number(to_string(1 << c));
    //    }
    //    string number = "1" + std::string(c, '0');
    //    return Number(number);
    //}

    // Операции с числами
    //Number operator + (Number number2) {
    //    string result = "";
    //    int borrow = 0;
    //    int i = value.length() - 1;
    //    int j = number2.value.length() - 1;
    //    while (i >= 0 || j >= 0) {
    //        int x = (i >= 0) ? (value[i--] - '0') : 0;
    //        int y = (j >= 0) ? (number2.value[j--] - '0') : 0;
    //        int sum = (x + borrow) + y;
    //        borrow = (sum >= 10) ? 1 : 0;
    //        if (sum >= 10) sum -= 10;
    //        result += sum + '0';
    //    }
    //    return leadZero(), Number(reverseStr(result));
    //}

    //Number operator - (Number number2) {
    //    int borrow = 0;
    //    int i = value.length() - 1;
    //    int j = number2.value.length() - 1;
    //    string result = "";
    //    while (i >= 0 || j >= 0) {
    //        int x = (i >= 0) ? (value[i--] - '0') : 0;
    //        int y = (j >= 0) ? (number2.value[j--] - '0') : 0;
    //        int diff = (x - borrow) - y;
    //        borrow = (diff < 0) ? 1 : 0;
    //        if (diff < 0) diff += 10;
    //        result += diff + '0';
    //    }
    //    result = reverseStr(result);
    //    leadZero();
    //    return *this;
    //}

    //Number operator * (Number number2) {
    //    int i = value.length() - 1;
    //    int j = number2.value.length() - 1;
    //    string result = std::string(value.length() + number2.value.length(), '0');
    //    for (int i = 0; i <= value.length(); i++) {
    //        int carry = 0;
    //        for (int j = 0; j < number2.value.length(); j++) {
    //            int x = value[i] - '0';
    //            int y = number2.value[j] - '0';
    //            int prod = x * y + carry;
    //            carry = prod / 10;
    //            result[i + j] = (prod % 10) + '0';
    //        }
    //    }
    //    return Number(result);
    //}

    //bool operator < (Number number2) {
    //    int len1 = value.length();
    //    int len2 = number2.value.length();
    //    if (len1 != len2) return len1 < len2;
    //    int i = 0;
    //    while (i < len1 && value[i] == number2.value[i]) {
    //        i++;
    //    }
    //    return i < len1 && (value[i] < number2.value[i]);
    //}

    //bool operator >= (Number number2) {
    //    int len1 = value.length();
    //    int len2 = number2.value.length();
    //    if (len1 != len2) return len1 > len2;
    //    int i = 0;
    //    while (i < len1 && value[i] == number2.value[i]) {
    //        i++;
    //    }
    //    return i <= len1 && (value[i] >= number2.value[i]);
    //}
};

int main() {

    /*Number res;
    Number ost; */  

    char* num1 = new char[1000]; 
    /*char* num2 = new char[1000];*/

    cin >> num1; 

    Number Num1(num1);
    /*Number Num2(num2);*/
    Number res = Num1 * 3;
    res.write();



    /*res = num1.divide(num2);
    cout << "Num1: ";
    num1.write();
    cout << "Num2: ";
    num2.write();
    cout << "Div: ";
    res.write();
    cout << "Mod: ";
    ost = num1 - res * num2;
    ost.write();*/

    return 0;
}
