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

int main() {
    return 0;
}
