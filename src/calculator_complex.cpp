#include "calculator_complex.h"

void ComplexCalculator::bind(INode* root) {
    if(root->GetNodeType() == NodeType::kResult) {
        root_ = root;
    } else {
        exit(1);    // 错误
    }
}

void ComplexCalculator::calculate(MDP *mdp) {
    // 节点栈
    std::stack<INode*> stack_node_ptr;
    // 结果栈
    std::stack<ComplexValueStruct> stack_val;
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
        } else if(top_node_ptr->GetNodeType() == NodeType::kConst || top_node_ptr->GetNodeType() == NodeType::kColumn) {
            ComplexValueStruct temp = GenValueStruct(top_node_ptr, mdp);
            stack_val.push(std::move(temp));
            last_node_ptr = top_node_ptr;
            stack_node_ptr.pop();
        }
    }
    // 取出结果，存到mdp中
    ComplexValueStruct t = std::move(stack_val.top());
    // 判断长度
    if(mdp->GetRowNum() != t.len) exit(-2);
    // int result_idx = mdp->GetColNum()-1;    // 索引mdp最后一列
    int result_idx = root->GetColIndex();
    // 释放mdp原数据
    mdp->FreeColMem(result_idx);
    mdp->SetColType(result_idx, t.type);
    mdp->SetColPtr(result_idx, t.data);
}

// 生成节点相应的结构体
ComplexValueStruct ComplexCalculator::GenValueStruct(INode* node, MDP *mdp) {
    ComplexValueStruct temp;
    int bytes;
    void* src = nullptr;
    if(node->GetNodeType() == NodeType::kConst) {
        ValueNode* ptr = reinterpret_cast<ValueNode*>(node);
        temp.len = 1;
        temp.type = ptr->GetDataType();
        bytes = AllocValueMem(temp.type, temp.data);
        src = ptr->GetValuePtr();
    } else if(node->GetNodeType() == NodeType::kColumn) {
        ColumnNode* ptr = reinterpret_cast<ColumnNode*>(node);
        temp.len = mdp->GetRowNum();
        temp.type = mdp->GetColType(ptr->GetColIdx());
        bytes = AllocValueMem(temp.type, temp.data, temp.len);
        src = mdp->GetColPtr(ptr->GetColIdx());
    }
    memcpy(temp.data, src, bytes);
    return std::move(temp);
}

// 运行计算并将结果入栈
void ComplexCalculator::RunAndPush(OptType opt, std::stack<ComplexValueStruct> &stack) {
    ComplexValueStruct right = std::move(stack.top());
    stack.pop();
    ComplexValueStruct left = std::move(stack.top());
    stack.pop();
    ComplexValueStruct middle;
    middle.data = RunOperator(opt, middle.type, left.data, right.data, left.type, right.type, left.len, right.len);
    middle.len = std::max(left.len, right.len);
    FreeValueMem(right.type, right.data);
    right.data = nullptr;
    FreeValueMem(left.type, left.data);
    left.data = nullptr;
    stack.push(std::move(middle));
}
