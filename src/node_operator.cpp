#include "node_operator.h"

OptType OperatorNode::GetOperator() const {
    return operator_;
}

char OperatorNode::GetOperatorChar() const {
    switch (operator_) {
        case OptType::kPlus: return '+';
        case OptType::kSub: return '-';
        case OptType::kMult: return '*';
        case OptType::kDiv: return '/';
        default: return '\0';
    }
}

INode* OperatorNode::GetLeft() const {
    return left_;
}

INode* OperatorNode::GetRight() const {
    return right_;
}

// 获取运算符优先级
int OperatorNode::GetOperatorLevel() const {
    switch (operator_) {
        case OptType::kPlus: return 0;
        case OptType::kSub: return 0;
        case OptType::kMult: return 1;
        case OptType::kDiv: return 1;
        default: return -1;
    }
}

void OperatorNode::SetOperator(OptType opt) {
    operator_ = opt;
}

void OperatorNode::SetLeft(INode* left) {
    left_ = left;
}

void OperatorNode::SetRight(INode* right) {
    right_ = right;
}

std::string OperatorNode::Show() const {
    // 存放需要遍历的节点
    std::stack<INode*> stack_node_ptr;
    // 存放已经遍历的节点的字符串
    std::stack<std::string> stack_str;
    // 存放最后遍历的节点的地址，用于判断二叉树的子节点遍历状态，因为为后序遍历，右节点遍历完左节点必也遍历完。
    INode *last_node_ptr = nullptr;
    stack_node_ptr.push(const_cast<OperatorNode*>(this));
    while(!stack_node_ptr.empty()) {
        INode *&top_node_ptr = stack_node_ptr.top();
        // 若为运算节点
        if(top_node_ptr->GetNodeType() == NodeType::kOperator) {
            OperatorNode *opt_node_ptr = reinterpret_cast<OperatorNode*>(top_node_ptr);
            // 当前节点未遍历完
            if(opt_node_ptr->GetRight() != last_node_ptr) {
                stack_node_ptr.push(opt_node_ptr->GetRight());
                if(opt_node_ptr->GetLeft() != last_node_ptr) {
                    stack_node_ptr.push(opt_node_ptr->GetLeft());
                }
            // 当前节点遍历完
            } else {
                std::string right = std::move(stack_str.top());
                stack_str.pop();
                // 判断右侧是否加括号
                if (opt_node_ptr->GetRight()->GetNodeType() == NodeType::kOperator) {
                    if (reinterpret_cast<OperatorNode*>(opt_node_ptr->GetRight())->GetOperatorLevel() <
                        opt_node_ptr->GetOperatorLevel()) {
                        right = "(" + right + ")";
                    }
                }
                std::string left = std::move(stack_str.top());
                stack_str.pop();
                // 判断左侧是否加括号
                if (opt_node_ptr->GetLeft()->GetNodeType() == NodeType::kOperator) {
                    if (reinterpret_cast<OperatorNode*>(opt_node_ptr->GetLeft())->GetOperatorLevel() <
                        opt_node_ptr->GetOperatorLevel()) {
                        left = "(" + left + ")";
                    }
                }
                stack_str.push(left + opt_node_ptr->GetOperatorChar() + right);
                last_node_ptr = top_node_ptr;
                stack_node_ptr.pop();
            }
        // 若为值节点，字符串入栈
        } else if(top_node_ptr->GetNodeType() == NodeType::kConst || top_node_ptr->GetNodeType() == NodeType::kColumn) {
            ValueNode *value_node_ptr = reinterpret_cast<ValueNode*>(top_node_ptr);
            stack_str.push(value_node_ptr->Show());
            last_node_ptr = top_node_ptr;
            stack_node_ptr.pop();
        }
    }
    // 返回最终的字符串
    return stack_str.top();
}
