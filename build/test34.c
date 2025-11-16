int main() {
    int i = 0;
    while (i < 10) {
        i = i + 1;
        {
            int j = 0;
        }
    }
    return i;
}
