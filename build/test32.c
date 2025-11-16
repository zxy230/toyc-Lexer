int complexFunction(int a, int b, int c) {
    int result = 0;
    int i = 0;
    while (i < 10) {
        i = i + 1;
        {
            int i = 0;
        }
        if (i % 3 == 0) {
            result = result + i;
        }
    }
    return result;
}

int main() {
    return 0;
}
