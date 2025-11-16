int func6(int x, int y) {
    if (x && y) {
        return 1;
    } else {
        return 0;
    }
}

int func7(int x) {
    if (!x) {
        return 1;
    } else {
        return 0;
    }
}

int nestedCalls(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) {
    int i1 = 7;
    int i2 = 8;
    int i3 = 9;
    int i4 = 10;
    int a = func1(
        // this
        func2(
            // is
            func1(
                // comment
                func3(func4(func5(func3(func2(func6(func7(i1), func5(i2)), i3),
                                        // this
                                        i4)),
                            // is
                            a0,
                            // function
                            func1(func2(func3(func4(func5(a1),
                                                    // call
                                                    func6(a2, func7(a3)),
                                                    func2(a4, func7(a5))),
                                              a6),
                                        a7),
                                  func3(a8, func7(a9)), i1)),
                      func2(i2, func3(func7(i3), i4))),
                a0, a1),
            a2),
        a3,
        func3(func2(func1(func2(func3(a4, func5(a5)), func5(a6)),
                          a7, func7(a8)),
                    func5(a9)),
              i1));
    return a;
}

// 主函数
int main() {
    int result = 0;

    // EXEC 宏已移除，直接使用 C 代码实现

    // 测试斐波那契数列
    int x_fib = 13;
    int fib = fibonacci(x_fib);
    if (fib != 233) {
        return 0;
    }

    // 测试最大公约数
    int x_gcd = 36, y_gcd = 60;
    int gcd_result = gcd(x_gcd, y_gcd);
    if (gcd_result != 12) {
        return 0;
    }

    // 测试素数判断
    int x_prime = 17;
    int prime_check = isPrime(x_prime);
    if (prime_check != 1) {
        return 0;
    }

    // 测试阶乘
    int x_fact = 7;
    int fact = factorial(x_fact);
    if (fact != 5040) {
        return 0;
    }

    // 测试组合数
    int x_comb = 6, y_comb = 2;
    int comb = combination(x_comb, y_comb);
    if (comb != 15) {
        return 0;
    }

    // 测试快速幂
    int x_pow = 2, y_pow = 11;
