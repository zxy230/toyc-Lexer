int func1(int x, int y, int z) { return x; }
int func2(int x, int y) { return x; }
int func3(int x, int y) { return x; }
int func4(int x, int y, int z) { return x; }
int func5(int x) { return x; }
int func6(int x, int y) { return x; }
int func7(int x) { return x; }

int nestedCalls(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) {
    int i1 = 7;
    int i2 = 8;
    int i3 = 9;
    int i4 = 10;
    int a = func1(
        func2(
            func1(
                func3(func4(func5(func3(func2(func6(func7(i1), func5(i2)), i3),
                                        i4)),
                            a0,
                            func1(func2(func3(func4(func5(a1),
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

int main() {
    return 0;
}
