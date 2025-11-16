int nestedLoopsAndConditions(int n) {
    int sum = 0;
    int i = 0;

    while (i < n) {
        int j = 0;
        while (j < i) {
            if ((i + j) % 2 == 0) {
                sum = sum - i * j;
            } else {
                sum = sum + i * j;

                if (sum < 0) {
                    sum = 0;
                    continue;
                }
            }

                if (sum > 1000) {
                    break;
            }

            j = j + 1;
        }

            if (sum > 1000) {
                break;
        }

        i = i + 1;
    }

    return sum;
}

int main() {
    return 0;
}
