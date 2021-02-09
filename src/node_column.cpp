#include "node_column.h"

int ColumnNode::GetColIdx() {
    return col_index_;
}

std::string ColumnNode::Show() const {
    return name_;
}
