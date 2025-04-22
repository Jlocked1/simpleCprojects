#include <stdio.h>

int main() {
    int num1, num2;
    char operation;

    puts("type a number: ");
    scanf("%d", &num1);
    puts("type another number: ");
    scanf("%d", &num2);
    puts("type a operation: +, -, /, %%");
    scanf(" %c", &operation);

    if (operation == '+') {
        return printf("A soma é: %d + %d = %d\n", num1, num2, num1 + num2);
    } else if (operation == '-') {
        return printf("A subtração é: %d - %d = %d\n", num1, num2, num1 - num2);
    } else if (operation == '*') {
        return printf("A multiplicação é: %d * %d = %d\n", num1, num2, num1 * num2);
    } else if (operation == '/') {
        return printf("A divisão é: %d / %d = %.2f\n", num1, num2, (float) num1 / num2);
    } else if (operation == '%') {
        return printf("O resto da divisão é: %d %% %d = %d\n", num1, num2, num1 % num2);
    }

    return 0;
};