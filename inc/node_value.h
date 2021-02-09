#ifndef CPP_11_2_1_NODE_VALUE_H_
#define CPP_11_2_1_NODE_VALUE_H_

#include "node_base.h"
#include <stack>
#include "utils.h"

// 值节点类
class ValueNode: public INode {
public:
    ValueNode(int value) : INode(NodeType::kConst, DataType::kInt), value_(nullptr) { SetValue<int>(value); }
    ValueNode(long value) : INode(NodeType::kConst, DataType::kLong), value_(nullptr) { SetValue<long>(value); }
    ValueNode(double value) : INode(NodeType::kConst, DataType::kDouble), value_(nullptr) { SetValue<double>(value); }
    ~ValueNode() {
        FreeValueMem(GetDataType(), value_);
        value_ = nullptr;
    }

public:
    template <typename T>
    T GetValue() const;
    void* GetValuePtr() const;
    template <typename T>
    void SetValue(T value);
    std::string Show() const;

private:
    void *value_;
};

template <typename T>
T ValueNode::GetValue() const {
    if(value_ != nullptr) {
        return *(reinterpret_cast<T*>(value_));
    } else {
        return T();
    }
}

template <typename T>
void ValueNode::SetValue(T value) {
    DataType type = Type2DataType<T>();
    if(value_ == nullptr) {
        AllocValueMem(type, value_);
        SetDataType(type);
    }
    switch (type) {
        case DataType::kInt: {
            int *t = reinterpret_cast<int*>(value_);
            *t = static_cast<int>(value);
            break;
        }
        case DataType::kLong: {
            long *t = reinterpret_cast<long*>(value_);
            *t = static_cast<long>(value);
            break;
        }
        case DataType::kDouble: {
            double *t = reinterpret_cast<double*>(value_);
            *t = static_cast<double>(value);
            break;
        }
        default: break;
    }
}

#endif