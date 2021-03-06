#ifndef CPP_11_2_1_CALCULATOR_COMPLEX_H_
#define CPP_11_2_1_CALCULATOR_COMPLEX_H_

#include "calculator_base.h"
#include "node_result.h"
#include "utils.h"
#include "mdp.h"
#include <string.h>

// 后序遍历中值栈的结构体
struct ComplexValueStruct{
    void *data;
    DataType type;
    int len;
};

// 用于计算表达式树
class ComplexCalculator : public ICalculator {
public:
    ComplexCalculator() : ICalculator() { }

public:
    void bind(INode*);
    void calculate(MDP *mdp);

private:
    ComplexValueStruct GenValueStruct(INode* node, MDP *mdp);
    void RunAndPush(OptType opt, std::stack<ComplexValueStruct> &stack);
};

#endif