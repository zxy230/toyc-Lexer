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
int main() { return 0; }
