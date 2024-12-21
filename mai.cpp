#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>

using namespace std;

char Look; // Lookahead Character

void GetChar() {
    cin.get(Look);
}

void Error(string s) {
    cout << endl;
    cout << (char)7 << "Error: " << s << ".";
}

void Abort(string s) {
    Error(s);
    exit(1);
}

void Expected(string s) {
    Abort(s + " Expected");
}

void Match(char x) {
    if (Look == x)
        GetChar();
    else
        Expected("'" + string(1, x) + "'");
}

bool IsAlpha(char c) {
    return isalpha(c);
}

bool IsDigit(char c) {
    return isdigit(c);
}

char GetName() {
    if (!IsAlpha(Look))
        Expected("Name");
    char name = toupper(Look);
    GetChar();
    return name;
}

char GetNum() {
    if (!IsDigit(Look))
        Expected("Integer");
    char num = Look;
    GetChar();
    return num;
}

void Emit(string s) {
    cout << "\t" << s;
}

void EmitLn(string s) {
    Emit(s);
    cout << endl;
}

void Init() {
    GetChar();
}

void Expression();

void Factor() {
    if (Look == '(') {
        Match('(');
        Expression();
        Match(')');
    } else {
        EmitLn("MOVE #" + string(1, GetNum()) + ",D0");
    }
}

void Multiply() {
    Match('*');
    Factor();
    EmitLn("MULS (SP)+,D0");
}

void Divide() {
    Match('/');
    Factor();
    EmitLn("MOVE (SP)+,D1");
    EmitLn("DIVS D1,D0");
}

void Term() {
    Factor();
    while (Look == '*' || Look == '/') {
        EmitLn("MOVE D0,-(SP)");
        switch (Look) {
        case '*':
            Multiply();
            break;
        case '/':
            Divide();
            break;
        default:
            Expected("Mulop");
            break;
        }
    }
}

void Add() {
    Match('+');
    Term();
    EmitLn("ADD (SP)+,D0");
}

void Subtract() {
    Match('-');
    Term();
    EmitLn("SUB (SP)+,D0");
    EmitLn("NEG D0");
}

void Expression() {
    if (IsAddop(Look))
        EmitLn("CLR D0");
    else
        Term();

    while (IsAddop(Look)) {
        EmitLn("MOVE D0,-(SP)");
        switch (Look) {
        case '+':
            Add();
            break;
        case '-':
            Subtract();
            break;
        default:
            Expected("Addop");
            break;
        }
    }
}

bool IsAddop(char c) {
    return c == '+' || c == '-';
}

int main() {
    Init();
    Expression();
    return 0;
}
