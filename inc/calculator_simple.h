#ifndef CPP_11_2_1_CALCULATOR_SIMPLE_H_
#define CPP_11_2_1_CALCULATOR_SIMPLE_H_

#include "node_result.h"
#include "operator.h"
#include "utils.h"

struct SimpleValueStruct{
    void *data;
    DataType type;
};

// 用于计算表达式树
class SimpleCalculator {
public:
    SimpleCalculator() : root_(nullptr) { };

public:
    void bind(INode*);
    void calculate();

private:
    SimpleValueStruct GenValueStruct(ValueNode *value_name);
    void RunAndPush(OptType opt, std::stack<SimpleValueStruct> &stack);
    INode *root_;
};

#endif // CPP_11_2_1_CALCULATOR_H_