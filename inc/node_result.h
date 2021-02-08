#ifndef CPP_11_2_1_NODE_RESULT_H_
#define CPP_11_2_1_NODE_RESULT_H_

#include "node_operator.h"
#include <stack>
#include "utils.h"

// 结果节点
class ResultNode: public INode {
public:
    ResultNode() : INode(NodeType::kResult, DataType::kNULL), value_(NULL), root_(NULL) { };
    ResultNode(INode* root) : INode(NodeType::kResult, DataType::kNULL), value_(NULL), root_(root) { };
    ~ResultNode() { delete root_; };

public:
    template <typename T>
    T GetValue() const;
    INode* GetRoot() const;
    void SetRoot(INode*);
    template <typename T>
    void SetValue(T value);
    std::string Show() const;

private:
    void* value_;
    INode* root_;
};

template <typename T>
void ResultNode::SetValue(T value) {
    DataType target = Type2DataType<T>();
    if(target != GetDataType() || value_ == NULL) {
        FreeValueMem(GetDataType(), value_);
        SetDataType(Type2DataType<T>());
        AllocValueMem(GetDataType(), value_);
    }
    T *t = reinterpret_cast<T*>(value_);
    *t = value;
}

#endif