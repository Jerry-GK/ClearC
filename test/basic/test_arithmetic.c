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

// ��������������ȼ�
#define ADD_SUB_PRIORITY 1
#define MUL_DIV_PRIORITY 2

// ����ջ���������
#define MAX_STACK_SIZE 100

// ���������ջ�Ľṹ��
typedef struct {
    char data[MAX_STACK_SIZE];
    int top;
} OperatorStack;

// ���������ջ�Ľṹ��
typedef struct {
    int data[MAX_STACK_SIZE];
    int top;
} OperandStack;

// ��ʼ��������ջ
void initOperatorStack(OperatorStack* stack) {
    stack->top = -1;
}

// ��ʼ��������ջ
void initOperandStack(OperandStack* stack) {
    stack->top = -1;
}

// ����������ջ
void pushOperator(OperatorStack* stack, char op) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Error: Operator stack is full.\n");
        exit(1);
    }
    stack->data[++stack->top] = op;
}

// ����������ջ
void pushOperand(OperandStack* stack, int num) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Error: Operand stack is full.\n");
        exit(1);
    }
    stack->data[++stack->top] = num;
}

// �Ӳ�����ջ����һ��������
char popOperator(OperatorStack* stack) {
    if (stack->top == -1) {
        printf("Error: Operator stack is empty.\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

// �Ӳ�����ջ����һ��������
int popOperand(OperandStack* stack) {
    if (stack->top == -1) {
        printf("Error: Operand stack is empty.\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

// �ж�һ���ַ��Ƿ�Ϊ������
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// ��ȡ�����������ȼ�
int getPriority(char op) {
    if (op == '*' || op == '/') {
        return MUL_DIV_PRIORITY;
    } else if (op == '+' || op == '-') {
        return ADD_SUB_PRIORITY;
    } else {
        return 0;
    }
}

// ִ�в�����ջ���Ĳ�����
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