#include <iostream>
#include <cstring>
#include <ctime>

#define ull unsigned long long
#define MIN_SIZE 10

using namespace std;

long long currentMemory = 0, maxMemory = 0;

class BigInt {
    bool sign = 0;
    int size = 0;
    char* arr = nullptr;

    void resize(int new_size) {
        char* new_arr = new char[new_size] {0};
        currentMemory += new_size * sizeof(char); //+
        int mn_size = (size < new_size ? size : new_size);
        for (int i = 0; i < mn_size; i++)
            new_arr[i] = arr[i];
        delete[] arr;
        currentMemory -= size * sizeof(char); //-
        size = new_size;
        arr = new_arr;
        if (currentMemory > maxMemory)
            maxMemory = currentMemory;
    }

    void remove_zeros() {
        while (size > 1 && arr[size - 1] == 0)
            size--;
        resize(size);
    }

    void reverse() {
        for (int i = 0; i < size / 2; i++)
            swap(arr[i], arr[size - i - 1]);
        remove_zeros();
    }

    void check_zero() {
        if (size == 1 && arr[0] == 0)
            sign = 0;
    } 
    bool isZero() const {
        return size == 1 && arr[0] == 0;
    }


public:
    BigInt() {
        resize(1);
    }

    BigInt(int n) {
        resize(1);
        if (n < 0) {
            sign = 1;
            n = -n;
        }
        int i = 0;
        do {
            if (i == size)
                resize(size + 1);
            arr[i++] = n % 10;
            n /= 10;
        } while (n);
        remove_zeros();
    }

    BigInt(const BigInt& n) {
        sign = n.sign;
        resize(n.size);
        for (int i = 0; i < size; i++)
            arr[i] = n.arr[i];
    }

    BigInt(char* str) {
        if (str == nullptr)
            return;
        if (str[0] == '-') {
            sign = 1;
            str++;
        }
        int len = strlen(str);
        resize(len);
        for (int i = 0; i < len; i++) {
            arr[len - i - 1] = str[i] - '0';
        }
        check_zero();
    }

    ~BigInt() {
        currentMemory -= size * sizeof(char); //-
        delete[] arr;
    }

    BigInt& operator=(const BigInt& n) {
        if (&n != this) {
            sign = n.sign;
            resize(n.size);
            for (int i = 0; i < size; i++)
                arr[i] = n.arr[i];
        }
        return *this;
    }
    // Оператор сравнения BigInt с int
    bool operator==(int n) const {
        // Если n равно 0, проверим, равен ли BigInt нулю
        if (n == 0) {
            if (size != 1) return false; // BigInt с нулем должен содержать ровно 1 цифру
            return arr[0] == 0 && !sign; // Проверяем, что число равно 0 и нет знака минус
        }

        // Если n не равно 0, создаём объект BigInt для сравнения
        BigInt bInt(n);
        return *this == bInt;
    }



    BigInt operator/(const BigInt& n) {
        if (n.isZero()) { // вместо n == 0
            throw runtime_error("Division by zero!");
        }

        // Определяем знак результата
        BigInt divisor = n.abs();
        BigInt dividend = this->abs();
        BigInt quotient = 0;
        BigInt remainder = 0;

        // Для оптимизации работы, будем использовать сдвиги
        int dividend_size = dividend.size;
        int divisor_size = divisor.size;

        // Позиция деления
        for (int i = dividend_size - 1; i >= 0; i--) {
            // Сдвиг остатков
            remainder = remainder * 10 + dividend.arr[i];

            // Проверяем, сколько раз делитель помещается в остатке
            int count = 0;
            while (remainder >= divisor) {
                remainder = remainder - divisor;
                count++;
            }

            // Записываем цифру в результат
            quotient = quotient * 10 + count;
        }

        // Возвращаем результат с нужным знаком
        quotient.sign = this->sign ^ n.sign;
        quotient.remove_zeros();
        return quotient;
    }

    BigInt operator%(const BigInt& n) {
        if (n == 0) {
            throw runtime_error("Modulo by zero!");
        }

        BigInt divisor = n.abs();
        BigInt dividend = this->abs();
        BigInt remainder = 0;

        // Для оптимизации работы, будем использовать сдвиги
        int dividend_size = dividend.size;

        // Позиция деления
        for (int i = dividend_size - 1; i >= 0; i--) {
            // Сдвиг остатков
            remainder = remainder * 10 + dividend.arr[i];

            // Вычитаем, пока остаток больше или равен делителю
            while (remainder >= divisor) {
                remainder = remainder - divisor;
            }
        }

        // Возвращаем остаток с нужным знаком
        remainder.sign = this->sign;
        remainder.remove_zeros();
        return remainder;
    }


    bool operator==(const BigInt& other) const {
        // Сравнение знаков
        if (sign != other.sign) return false;

        // Сравнение размера
        if (size != other.size) return false;

        // Посимвольное сравнение
        for (size_t i = 0; i < size; ++i) {
            if (arr[i] != other.arr[i]) return false;
        }

        return true;
    }


    bool operator!=(const BigInt& n) {
        return !(*this == n);
    }

    bool operator>(const BigInt& n) {
        if (sign != n.sign)
            return sign == 0;
        if (size != n.size) {
            if (sign == 0)
                return size > n.size;
            else
                return size < n.size;
        }
        for (int i = size - 1; i >= 0; i--)
            if (arr[i] > n.arr[i])
                return 1;
            else if (arr[i] < n.arr[i])
                return 0;
        return 0;
    }

    bool operator<(const BigInt& n) {
        return !(*this > n) && !(*this == n);
    }

    bool operator>=(const BigInt& n) {
        return !(*this < n);
    }

    bool operator<=(const BigInt& n) {
        return !(*this > n);
    }

    BigInt operator-() const {
        BigInt res = *this;
        res.sign = !res.sign;
        res.check_zero();
        return res;
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sign = 0;
        return res;
    }

    BigInt operator+(const BigInt& n) {
        if (sign == n.sign) {
            int mx_size = max(size, n.size);
            BigInt res = *this;
            res.resize(mx_size + 1);
            int carry = 0;
            for (int i = 0; i < n.size || carry; i++) {
                int sum = res.arr[i] + (i < n.size ? n.arr[i] : 0) + carry;
                res.arr[i] = sum % 10;
                carry = sum / 10;
            }
            res.remove_zeros();
            return res;
        }
        else if (sign == 0)
            return *this - (-n);
        else
            return -(-(*this) - n);
    }

    BigInt& operator+=(const BigInt& n) {
        *this = *this + n;
        return *this;
    }

    BigInt operator-(const BigInt& n) {
        if (sign == n.sign) {
            if (this->abs() < n.abs())
                return -((BigInt)n - *this);
            BigInt res = *this;
            for (int i = 0; i < n.size; i++) {
                int ri = res.arr[i] - n.arr[i];
                if (ri < 0) {
                    ri += 10;
                    res.arr[i + 1]--;
                }
                res.arr[i] = ri;
            }
            res.remove_zeros();
            res.check_zero();
            return res;
        }
        else
            return *this + (-n);
    }

    BigInt& operator-=(const BigInt& n) {
        *this = *this - n;
        return *this;
    }

    BigInt operator*(const BigInt& n) {
        BigInt res = 0;
        res.sign = sign ^ n.sign;
        res.resize(size + n.size);
        for (int i = 0; i < n.size; i++) {
            int carry = 0;
            for (int j = 0; j < size; j++) {
                int prod = arr[j] * n.arr[i] + carry + res.arr[i + j];
                res.arr[i + j] = prod % 10;
                carry = prod / 10;
            }
            if (carry)
                res.arr[i + size] += carry;
        }
        res.remove_zeros();
        res.check_zero();
        return res;
    }

    BigInt& operator*=(const BigInt& n) {
        *this = *this * n;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const BigInt& n) {
        if (n.sign)
            os << '-';
        if (n.arr)
            for (int i = n.size - 1; i >= 0; i--)
                os << (int)n.arr[i];
        return os;
    }

    friend istream& operator>>(istream& is, BigInt& n) {
        n.sign = 0;
        n.resize(1);
        int i = 0;
        char ch = is.get();
        if (ch == '-')
            n.sign = 1;
        else
            n.arr[i++] = ch - '0';
        while ((ch = is.get()) && ('0' <= ch && ch <= '9')) {
            if (i >= n.size)
                n.resize(n.size * 2);
            n.arr[i++] = ch - '0';
        }
        n.resize(i);
        n.reverse();
        return is;
    }

    char* toString(char* str, int mx) {
        if (str == nullptr)
            return nullptr;
        if (size + sign + 1 > mx)
            return nullptr;
        int i = 0;
        if (sign)
            str[i++] = '-';
        for (int j = size - 1; j >= 0; j--)
            str[i++] = '0' + arr[j];
        str[i] = 0;
        return str;
    }
    BigInt randint(int element_size)
    {
        BigInt res = 1 + rand() % 9;
        for (int i = 0; i < element_size - 1; i++)
        {
            res = (res << 1) + rand() % 10;
            return res;

        }
    }
    BigInt operator<<(int n) {
        if (n <= 0 || *this == 0)
            return *this;
        BigInt res = 0;
        res.resize(size + n);
        for (int i = 0; i < size; i++)
            res.arr[n + i] = arr[i];
        return res;
    }
    
};

int main() {
    srand(time(NULL));
    int size;
    
}
