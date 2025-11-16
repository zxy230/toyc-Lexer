int complexFunction(int a, int b, int c) {
    int result = 0;

    if (!(a > b && b > c)) {
        result = a * b - -(c + -1);
    } else if (!(a < c) || c < b) {
        result = a * (c - +b - -2);
    }
    
    return result;
}

int main() {
    return 0;
}
