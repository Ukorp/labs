#include <stdio.h>
#include <stdarg.h>

int has_finite_representation(int base, int count, ...) {
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        double decimal_value = va_arg(args, double);
        double fractional_part = decimal_value;
        
        fractional_part *= base;
        int seen[100] = {0};
        while (fractional_part != 0 && seen[(int)fractional_part] == 0) {
            seen[(int)fractional_part] = 1;
            fractional_part -= (int)fractional_part;
            fractional_part *= base;
            
        }
        if (fractional_part == 0) {
            printf("Decimal %f has finite representation in base %d\n", decimal_value, base);
        } else {
            printf("Decimal %f does not have finite representation in base %d\n", decimal_value, base);
        }
    }
    va_end(args);
}

int main() {
    has_finite_representation(16, 0, 0.7, 0.25, 0.9);
    return 0;
}
