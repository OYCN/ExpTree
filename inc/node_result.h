#ifndef CPP_11_2_1_NODE_RESULT_H_
#define CPP_11_2_1_NODE_RESULT_H_

#include "node_operator.h"
#include <stack>
#include "utils.h"

// 结果节点
class ResultNode: public INode {
public:
    ResultNode() : INode(NodeType::kResult, DataType::kNULL), value_(nullptr), root_(nullptr), col_index_(0) { }
    ResultNode(INode* root) : INode(NodeType::kResult, DataType::kNULL), value_(nullptr), root_(root), col_index_(0) { }
    ResultNode(int index) : INode(NodeType::kResult, DataType::kNULL), value_(nullptr), root_(nullptr), col_index_(index) { }
    ResultNode(INode* root, int index) : INode(NodeType::kResult, DataType::kNULL), value_(nullptr), root_(root), col_index_(index) { }
    ~ResultNode() {
        if(root_ != nullptr) {
            delete root_;
        }
    }

public:
    template <typename T>
    T GetValue() const;
    INode* GetRoot() const;
    int GetColIndex();
    void SetRoot(INode*);
    template <typename T>
    void SetValue(T value);
    void SetColIndex(int idx);
    std::string Show() const;

private:
    void* value_;
    INode* root_;
    // mdp结果列索引
    int col_index_;
};

template <typename T>
void ResultNode::SetValue(T value) {
    DataType target = Type2DataType<T>();
    if(target != GetDataType() || value_ == nullptr) {
        FreeValueMem(GetDataType(), value_);
        value_ = nullptr;
        SetDataType(Type2DataType<T>());
        AllocValueMem(GetDataType(), value_);
    }
    T *t = reinterpret_cast<T*>(value_);
    *t = value;
}

#endif