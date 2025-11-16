// 斐波那契数列计算（递归实现）
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

// 最大公约数（辗转相除法）
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// 判断素数
int isPrime(int num) {
    if (num <= 1) {
        return 0;
    }

    if (num <= 3) {
        return 1;
    }

    if (num % 2 == 0 || num % 3 == 0) {
        return 0;
    }

    int i = 5;
    while (i * i <= num) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return 0;
        }
        i = i + 6;
    }

    return 1;
}

// 阶乘计算
int factorial(int n) {
    int result = 1;
    while (n > 0) {
        result = result * n;
        n = n - 1;
    }
    return result;
}

// 计算组合数 C(n,k)
int combination(int n, int k) {
    if (k > n) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }

    // 使用公式 C(n,k) = n!/(k!*(n-k)!)
    return factorial(n) / (factorial(k) * factorial(n - k));
}

// 快速幂算法
int power(int base, int exponent) {
    int result = 1;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exponent = exponent / 2;
    }

    return result;
}

// 复杂的嵌套控制流和表达式计算
int complexFunction(int a, int b, int c) {
    int result = 0;

    if (!(a > b && b > c)) {
        result = a * b - -(c + -1);
    } else if (!(a < c) || c < b) {
        result = a * (c - +b - -2);
    } else if (b >= a && a >= c || b <= c) {
        result = b * a - -(c + -3);
    } else if (b > c || c > a && a <= b) {
        result = b * (c - +a - -4);
    } else if (!(c > a || b != a) && a == b) {
        result = c * a - +(b + -5);
    } else {
        result = c * (b - -a - -6);
    }

    int i = 0;
    while (i < 10) {
        i = i + 1;
        {
            int i = 0;
        }

        if (i % 3 == 0) {
            result = result + i;
        } else if (i % 3 == 1) {
            result = result - i;
        } else {
            result = result * 2;

            if (result < 50) {
                continue;
            }

            result = result + 1;

            if (result > 100) {
                break;
            }
        }
    }

    return result;
}

// 测试短路逻辑
int shortCircuit(int a, int b) {
    int result = 0;

    // 测试逻辑与的短路
        if (a > 0 && b / a > 2) {
            result = result + 42;
    }

    // 测试逻辑或的短路
        if (a < 0 || b / (a - a + 1) < 0) {
            result = result + 17;
    }

int main() { return 0; }
