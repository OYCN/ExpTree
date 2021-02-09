#include "calculator_simple.h"

void SimpleCalculator::bind(INode* root) {
    if(root->GetNodeType() == NodeType::kResult) {
        root_ = root;
    } else {
        exit(1);
    }
}

void SimpleCalculator::calculate(MDP *mdp) {
    // 节点栈
    std::stack<INode*> stack_node_ptr;
    // 结果栈
    std::stack<SimpleValueStruct> stack_val;
    // 最后遍历的节点地址
    INode *last_node_ptr = nullptr;
    ResultNode *root = reinterpret_cast<ResultNode*>(root_);
    stack_node_ptr.push(root->GetRoot());
    while(!stack_node_ptr.empty()) {
        INode *&top_node_ptr = stack_node_ptr.top();
        // 若为运算节点
        if(top_node_ptr->GetNodeType() == NodeType::kOperator) {
            OperatorNode *opt_node_ptr = reinterpret_cast<OperatorNode*>(top_node_ptr);
            // 未遍历完
            if(opt_node_ptr->GetRight() != last_node_ptr) {
                stack_node_ptr.push(opt_node_ptr->GetRight());
                if(opt_node_ptr->GetLeft() != last_node_ptr) {
                    stack_node_ptr.push(opt_node_ptr->GetLeft());
                }
            // 遍历完成，进行运算
            } else {
                RunAndPush(opt_node_ptr->GetOperator(), stack_val);
                last_node_ptr = top_node_ptr;
                stack_node_ptr.pop();
            }
        // 若为值节点，值入栈
        } else if(top_node_ptr->GetNodeType() == NodeType::kConst) {
            ValueNode *value_node_ptr = reinterpret_cast<ValueNode*>(top_node_ptr);
            SimpleValueStruct temp = GenValueStruct(value_node_ptr);
            stack_val.push(std::move(temp));
            last_node_ptr = top_node_ptr;
            stack_node_ptr.pop();
        }
    }
    // 取出结果，存到ResultNode
    SimpleValueStruct t = std::move(stack_val.top());
    switch (t.type) {
        case DataType::kInt: root->SetValue<int>(*(reinterpret_cast<int*>(t.data))); break;
        case DataType::kLong: root->SetValue<long>(*(reinterpret_cast<long*>(t.data))); break;
        case DataType::kDouble: root->SetValue<double>(*(reinterpret_cast<double*>(t.data))); break;
        default: break;
    }
    FreeValueMem(t.type, t.data);
    t.data = nullptr;
}

// 根据值节点，生成相应的值栈所用的结构体
SimpleValueStruct SimpleCalculator::GenValueStruct(ValueNode *value_name) {
    SimpleValueStruct temp;
    temp.type = value_name->GetDataType();
    switch (temp.type) {
    case DataType::kInt: {
        temp.data = new int;
        int *t = reinterpret_cast<int*>(temp.data);
        *t = value_name->GetValue<int>();
        break;
    }
    case DataType::kLong: {
        temp.data = new long;
        long *t = reinterpret_cast<long*>(temp.data);
        *t = value_name->GetValue<long>();
        break;
    }
    case DataType::kDouble: {
        temp.data = new double;
        double *t = reinterpret_cast<double*>(temp.data);
        *t = value_name->GetValue<double>();
        break;
    }
    default: {
        temp.data = nullptr;
        break;
    }
    }
    return std::move(temp);
}

void SimpleCalculator::RunAndPush(OptType opt, std::stack<SimpleValueStruct> &stack) {
    // 右、左 出栈
    SimpleValueStruct right = std::move(stack.top());
    stack.pop();
    SimpleValueStruct left = std::move(stack.top());
    stack.pop();
    // 计算结果
    SimpleValueStruct middle;
    middle.data = RunOperator(opt, middle.type, left.data, right.data, left.type, right.type);
    // 释放右、左值的空间
    FreeValueMem(right.type, right.data);
    right.data = nullptr;
    FreeValueMem(left.type, left.data);
    left.data = nullptr;
    stack.push(std::move(middle));
}


