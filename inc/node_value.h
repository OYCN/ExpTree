#ifndef CPP_11_2_1_NODE_VALUE_H_
#define CPP_11_2_1_NODE_VALUE_H_

#include "node_base.h"
#include <stack>
#include "utils.h"

// 值节点类
class ValueNode: public INode {
public:
    ValueNode(int value) : INode(NodeType::kConst, DataType::kInt), value_(NULL) { SetValue<int>(value); };
    ValueNode(long value) : INode(NodeType::kConst, DataType::kLong), value_(NULL) { SetValue<long>(value); };
    ValueNode(double value) : INode(NodeType::kConst, DataType::kDouble), value_(NULL) { SetValue<double>(value); };
    ~ValueNode() { FreeValueMem(GetDataType(), value_); };

public:
    template <typename T>
    T GetValue() const;
    template <typename T>
    void SetValue(T value);
    std::string Show() const;

private:
    void *value_;
};

template <typename T>
T ValueNode::GetValue() const {
    if(value_ != NULL) {
        return *(reinterpret_cast<T*>(value_));
    } else {
        return T();
    }
}

template <typename T>
void ValueNode::SetValue(T value) {
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