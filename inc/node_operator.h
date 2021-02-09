#ifndef CPP_11_2_1_NODE_OPERATOR_H_
#define CPP_11_2_1_NODE_OPERATOR_H_

#include "node_value.h"
#include "node_column.h"
#include <stack>
#include "utils.h"

// 操作符类型
enum class OptType {
    kNULL,
    kPlus,
    kSub,
    kMult,
    kDiv
};

// 运算符节点类
class OperatorNode: public INode {
public:
    // OperatorNode();
    OperatorNode(OptType opt) : INode(NodeType::kOperator, DataType::kNULL), operator_(opt), left_(nullptr), right_(nullptr) { };
    OperatorNode(OptType opt, INode* left, INode* right): INode(NodeType::kOperator, DataType::kNULL), operator_(opt), left_(left), right_(right) { };
    ~OperatorNode() { delete left_; delete right_; };

public:
    OptType GetOperator() const;
    char GetOperatorChar() const;
    INode* GetLeft() const;
    INode* GetRight() const;
    int GetOperatorLevel() const;
    void SetOperator(OptType opt);
    void SetLeft(INode* left);
    void SetRight(INode* right);
    std::string Show() const;

private:
    OptType operator_;
    INode* left_;
    INode* right_;
};

#endif