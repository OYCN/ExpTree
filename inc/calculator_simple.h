#ifndef CPP_11_2_1_CALCULATOR_SIMPLE_H_
#define CPP_11_2_1_CALCULATOR_SIMPLE_H_

#include "node_result.h"
#include "utils.h"

struct SimpleValueStruct{
    void *data;
    DataType type;
};

// 用于计算表达式树
class SimpleCalculator {
public:
    SimpleCalculator() : root_(NULL) { };

public:
    void bind(INode*);
    void calculate();

private:
    SimpleValueStruct GenValueStruct(ValueNode *value_name);
    void RunAndPush(OptType opt, std::stack<SimpleValueStruct> &stack);
    template<typename T>
    T RunOperator(OptType opt,T left,T right);
    INode *root_;
};

// 根据运算符进行计算
template<typename T>
T SimpleCalculator::RunOperator(OptType opt,T left,T right) {
    switch (opt) {
        case OptType::kPlus: return left+right;
        case OptType::kSub: return left-right;
        case OptType::kMult: return left*right;
        case OptType::kDiv: return left/right;
        default: return 0;
    }
}

#endif // CPP_11_2_1_CALCULATOR_H_