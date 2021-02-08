#ifndef CPP_11_2_1_NODE_COLUMN_H_
#define CPP_11_2_1_NODE_COLUMN_H_

#include "node_base.h"

// 列节点类
class ColumnNode: public INode {
public:
    ColumnNode(int idx, std::string name) : INode(NodeType::kColumn, DataType::kInt), col_index_(idx), name_(name) { };
    ~ColumnNode() { };

public:
    std::string Show() const;

private:
    std::string name_;
    // 内部标识列索引
    int col_index_;
};

#endif