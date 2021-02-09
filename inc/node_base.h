#ifndef CPP_11_2_1_NODE_BASE_H_
#define CPP_11_2_1_NODE_BASE_H_

#include <string>

// 节点类型
enum class NodeType {
    kNULL,
    kConst,
    kColumn,
    kOperator,
    kResult
};

// 值类型
enum class DataType {
    kNULL,
    kInt,
    kLong,
    kDouble
};

// 节点接口类
class INode {
public:
    INode(NodeType nodeType, DataType dataType): node_type_(nodeType), data_type_(dataType) { }

public:
    NodeType GetNodeType() const { return node_type_; }
    DataType GetDataType() const { return data_type_; }
    void SetDataType(DataType type) { data_type_ = type; }
    // 将表达式树转换为表达式字符串返回
    virtual std::string Show() const = 0;

protected:
    NodeType node_type_;
    DataType data_type_;
};

#endif