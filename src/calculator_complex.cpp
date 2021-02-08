#include "calculator_complex.h"

void ComplexCalculator::bind(INode* root) {
    if(root->GetNodeType() == NodeType::kResult) {
        root_ = root;
    } else {
        exit(1);
    }
}

void ComplexCalculator::calculate(MDP *mdp) {
    // 节点栈
    std::stack<INode*> stack_node_ptr;
    // 结果栈
    std::stack<ComplexValueStruct> stack_val;
    // 最后遍历的节点地址，用于判断二叉树的子节点遍历状态，因为为后序遍历，右节点遍历完左节点必也遍历完。
    INode *last_node_ptr = NULL;
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
        } else if(top_node_ptr->GetNodeType() == NodeType::kConst || top_node_ptr->GetNodeType() == NodeType::kColumn) {
            ComplexValueStruct temp = GenValueStruct(top_node_ptr);
            stack_val.push(std::move(temp));
            last_node_ptr = top_node_ptr;
            stack_node_ptr.pop();
        }
    }
    // 取出结果，存到ResultNode
    ComplexValueStruct t = std::move(stack_val.top());
    switch (t.type) {
        case DataType::kInt: root->SetValue<int>(*(reinterpret_cast<int*>(t.data))); break;
        case DataType::kLong: root->SetValue<long>(*(reinterpret_cast<long*>(t.data))); break;
        case DataType::kDouble: root->SetValue<double>(*(reinterpret_cast<double*>(t.data))); break;
        default: break;
    }
    FreeValueMem(t.type, t.data);
}

ComplexValueStruct ComplexCalculator::GenValueStruct(INode* value_node) {
    ComplexValueStruct temp;
    temp.type = value_node->GetDataType();
    switch (temp.type) {
    case DataType::kInt: {
        temp.data = new int;
        int *t = reinterpret_cast<int*>(temp.data);
        *t = value_node->GetValue<int>();
        break;
    }
    case DataType::kLong: {
        temp.data = new long;
        long *t = reinterpret_cast<long*>(temp.data);
        *t = value_node->GetValue<long>();
        break;
    }
    case DataType::kDouble: {
        temp.data = new double;
        double *t = reinterpret_cast<double*>(temp.data);
        *t = value_node->GetValue<double>();
        break;
    }
    default: {
        temp.data = NULL;
        break;
    }
    }
    return std::move(temp);
}

void ComplexCalculator::RunAndPush(OptType opt, std::stack<ComplexValueStruct> &stack) {
    ComplexValueStruct right = std::move(stack.top());
    stack.pop();
    ComplexValueStruct left = std::move(stack.top());
    stack.pop();
    ComplexValueStruct middle;
    middle.type = std::max(right.type, left.type);
    // 类型转换
    if(right.type > left.type) {
        AltValueMem(left.type, right.type, left.data);
        left.type = right.type;
    } else if(right.type < left.type) {
        AltValueMem(right.type, left.type, right.data);
        right.type = left.type;
    }
    // 根据类型进行运算
    switch (middle.type) {
        case DataType::kInt: {
            middle.data = new int;
            int *t = reinterpret_cast<int*>(middle.data);
            *t = RunOperator(opt, *(reinterpret_cast<int*>(left.data)), *(reinterpret_cast<int*>(right.data)));
            break;
        }
        case DataType::kLong: {
            middle.data = new long;
            long *t = reinterpret_cast<long*>(middle.data);
            *t = RunOperator(opt, *(reinterpret_cast<long*>(left.data)), *(reinterpret_cast<long*>(right.data)));
            break;
        }
        case DataType::kDouble: {
            middle.data = new double;
            double *t = reinterpret_cast<double*>(middle.data);
            *t = RunOperator(opt, *(reinterpret_cast<double*>(left.data)), *(reinterpret_cast<double*>(right.data)));
            break;
        }
        default: { middle.data = NULL; break; }
    }
    FreeValueMem(right.type, right.data);
    FreeValueMem(left.type, left.data);
    stack.push(std::move(middle));
}

