#ifndef CPP_11_2_1_CALCULATOR_COMPLEX_H_
#define CPP_11_2_1_CALCULATOR_COMPLEX_H_

#include "node_result.h"
#include "utils.h"
#include "mdp.h"

// 后序遍历中值栈的结构体
struct ComplexValueStruct{
    void *data;
    DataType type;
    int len;
};

// 用于计算表达式树
class ComplexCalculator {
public:
    ComplexCalculator() : root_(NULL) { };

public:
    void bind(INode*);
    void calculate(MDP *mdp);

private:
    ComplexValueStruct GenSimpleValueStruct(ValueNode* value_name);
    ComplexValueStruct GenComplexValueStruct(ColumnNode* value_name);
    void RunAndPush(OptType opt, std::stack<ComplexValueStruct> &stack);
    template<typename T>
    T RunOperator(OptType opt,T left,T right);
    INode *root_;
};

template<typename T>
T ComplexCalculator::RunOperator(OptType opt,T left,T right) {
    switch (opt) {
        case OptType::kPlus: return left+right;
        case OptType::kSub: return left-right;
        case OptType::kMult: return left*right;
        case OptType::kDiv: return left/right;
        default: return 0;
    }
}

#endif