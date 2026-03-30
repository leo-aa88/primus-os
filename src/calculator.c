#include "../include/calculator.h"
#include "../include/tty.h"
#include "../include/string.h"
#include "../include/math.h"

double calc_stack[MAX_STACK_SIZE];
int calc_top = -1;

void calc_push(double val) {
    if(calc_top >= MAX_STACK_SIZE - 1) {
        printk("Stack overflow\n");
        return;
    }
    calc_stack[++calc_top] = val;
}

double calc_pop() {
    if(calc_top < 0) {
        printk("Stack underflow\n");
        return 0.0;
    }
    return calc_stack[calc_top--];
}

void compute(char *buffer) {
    char *token;
    double a, b;
    char symbol;

    /* Tokenize the string buffer into an array of strings */
    token = strtok(buffer, " ");
    while(token != NULL) {
        if(isdigit(token[0])) {
            calc_push(atof(token));
        } else {
            symbol = token[0];
            b = calc_pop();
            a = calc_pop();
            switch(symbol) {
                case '+': calc_push(a + b); break;
                case '-': calc_push(a - b); break;
                case '*': calc_push(a * b); break;
                case '^': calc_push(pow(a,b)); break;
                case '/': 
                    if(b != 0) {
                        calc_push(a / b);
                    } else {
                        printk("Error: Division by zero\n");
                        return;
                    }
                    break;
                default:
                    printk("Error: Invalid operator '%c'\n", symbol);
                    return;
            }
        }
        token = strtok(NULL, " ");
    }

    printk("\nResult: %f\n", calc_pop());

    return;
}