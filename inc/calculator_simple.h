#ifndef CPP_11_2_1_CALCULATOR_SIMPLE_H_
#define CPP_11_2_1_CALCULATOR_SIMPLE_H_

#include "calculator_base.h"
#include "node_result.h"
#include "utils.h"

struct SimpleValueStruct{
    void *data;
    DataType type;
};

// 用于计算表达式树
class SimpleCalculator : public ICalculator {
public:
    SimpleCalculator() : ICalculator() { }

public:
    void bind(INode*);
    void calculate(MDP *mdp = nullptr);

private:
    SimpleValueStruct GenValueStruct(ValueNode *value_name);
    void RunAndPush(OptType opt, std::stack<SimpleValueStruct> &stack);
};

#endif // CPP_11_2_1_CALCULATOR_H_