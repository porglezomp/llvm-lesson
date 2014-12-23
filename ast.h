#ifndef _AST_H
#define _AST_H

#include <string>
#include <vector>

/// Base class for all expression nodes
class ExprAST {
public:
    virtual ~ExprAST() {}
};

/// Expression class for numeric literals
class NumberExprAST : public ExprAST {
    double value;
public:
    NumberExprAST(double val) : value(val) {}
};

/// Expression class for referencing a variable
class VariableExprAST : public ExprAST {
    std::string name;
public:
    VariableExprAST(const std::string &name) : name(name) {}
};

/// Expression class for binary operator
class BinaryExprAST : public ExprAST {
    char op;
    ExprAST *lhs, *rhs;
public:
    BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) :
        op(op), lhs(lhs), rhs(rhs) {}
};

/// Expression class for function calls
class CallExprAST : public ExprAST {
    std::string callee;
    std::vector<ExprAST*> args;
public:
    CallExprAST(const std::string &callee, std::vector<ExprAST*> &args) :
        callee(callee), args(args) {}
};

/// This class represents the prototype of a function,
/// it captures the function name and its argument names
class PrototypeAST {
    std::string name;
    std::vector<std::string> args;
public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args) :
        name(name), args(args) {}
};

class FunctionAST {
    PrototypeAST *prototype;
    ExprAST *body;
public:
    FunctionAST(PrototypeAST *proto, ExprAST *body) :
        prototype(proto), body(body) {}
};

#endif