#include <stdio.h>
#include <stdarg.h>
#include <math.h>



typedef struct {
    double x;
    double y;
} Cord;

int isConvex(int num, ...) {
    va_list args;
    va_start(args, num);

    Cord prev = va_arg(args, Cord);
    Cord curr = va_arg(args, Cord);
    Cord next = va_arg(args, Cord);

    int sign = 0;
    for (int i = 0; i < num; i++) {
        double crossProduct = (curr.x - prev.x) * (next.y - curr.y) - (curr.y - prev.y) * (next.x - curr.x);
        if (crossProduct != 0) {
            if (sign == 0) {
                sign = crossProduct > 0 ? 1 : -1;
            } else if ((crossProduct > 0 && sign == -1) || (crossProduct < 0 && sign == 1)) {
                return 0;
            }
        }

        prev = curr;
        curr = next;
        next = va_arg(args, Cord);
    }

    va_end(args);

    double crossProduct = (curr.x - prev.x) * (next.y - curr.y) - (curr.y - prev.y) * (next.x - curr.x);
    if (crossProduct != 0) {
        if ((crossProduct > 0 && sign == -1) || (crossProduct < 0 && sign == 1)) {
            return 0;
        }
    }

    return 1;
}

double evaluatePolynomial(double x, int degree, ...) {
    va_list args;
    va_start(args, degree);

    double result = 0;
    double coefficient;

    for (int i = degree; i >= 1; i--) {
        coefficient = va_arg(args, double);
        result += coefficient * pow(x, i);
    }

    va_end(args);

    return result;
}

int main() {
    Cord p1 = {0, 0};
    Cord p2 = {1, 0};
    Cord p3 = {1, 1};
    Cord p4 = {-1, 1};
    Cord p5 = {-1, 0};

    int isConvexPolygon = isConvex(5, p1, p2, p3, p4, p5);
    printf("Is convex polygon: %d\n", isConvexPolygon);
    double x = 2.0;
    double coefficient0 = 1.0;
    double coefficient1 = 2.0;
    double coefficient2 = 3.0;

    double result = evaluatePolynomial(x, 2, coefficient2, coefficient1, coefficient0);
    printf("Result: %f\n", result);
    return 0;
}