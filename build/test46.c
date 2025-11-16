int shortCircuit(int a, int b) {
    int result = 0;

    if (a > 0 && b / a > 2) {
        result = result + 42;
    }

    if (a < 0 || b / (a - a + 1) < 0) {
        result = result + 17;
    }

    return result;
}

int main() {
    return 0;
}
