# include <stdio.h>
# include <string.h>
# include <stdlib.h>

// int printf(char ptr, ...);
// int scanf(char ptr, ...);

//a calculator for expressions(which may have +, -, *, /, (, )), using stack
int calculate(char* exp) {

}

int main() {
    char exp[100];
    scanf("%s", exp);

    int result = calculate(exp);
    printf("%d", result);
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义操作符的优先级
#define ADD_SUB_PRIORITY 1
#define MUL_DIV_PRIORITY 2

// 定义栈的最大容量
#define MAX_STACK_SIZE 100

// 定义操作符栈的结构体
typedef struct {
    char data[MAX_STACK_SIZE];
    int top;
} OperatorStack;

// 定义操作数栈的结构体
typedef struct {
    int data[MAX_STACK_SIZE];
    int top;
} OperandStack;

// 初始化操作符栈
void initOperatorStack(OperatorStack* stack) {
    stack->top = -1;
}

// 初始化操作数栈
void initOperandStack(OperandStack* stack) {
    stack->top = -1;
}

// 将操作符入栈
void pushOperator(OperatorStack* stack, char op) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Error: Operator stack is full.\n");
        exit(1);
    }
    stack->data[++stack->top] = op;
}

// 将操作数入栈
void pushOperand(OperandStack* stack, int num) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Error: Operand stack is full.\n");
        exit(1);
    }
    stack->data[++stack->top] = num;
}

// 从操作符栈弹出一个操作符
char popOperator(OperatorStack* stack) {
    if (stack->top == -1) {
        printf("Error: Operator stack is empty.\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

// 从操作数栈弹出一个操作数
int popOperand(OperandStack* stack) {
    if (stack->top == -1) {
        printf("Error: Operand stack is empty.\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

// 判断一个字符是否为操作符
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// 获取操作符的优先级
int getPriority(char op) {
    if (op == '*' || op == '/') {
        return MUL_DIV_PRIORITY;
    } else if (op == '+' || op == '-') {
        return ADD_SUB_PRIORITY;
    } else {
        return 0;
    }
}

// 执行操作符栈顶的操作符
void evaluateTopOperator(OperatorStack* operatorStack, OperandStack* operandStack) {
    char op = popOperator(operatorStack);
    int operand2 = popOperand(operandStack);
    int operand1 = popOperand(operandStack);
    int result;
    switch (op) {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            result = operand1 / operand2;
            break;
        default:
            printf("Error: Invalid operator.\n");
            exit(1);
    }