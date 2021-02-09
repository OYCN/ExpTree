#include "node_result.h"

template <typename T>
T ResultNode::GetValue() const {
    if(value_ != nullptr) {
        return *(reinterpret_cast<T*>(value_));
    } else {
        return T();
    }
}

INode* ResultNode::GetRoot() const {
    return root_;
}

int ResultNode::GetColIndex() {
    return col_index_;
}

void ResultNode::SetRoot(INode* root) {
    root_ = root;
}

void ResultNode::SetColIndex(int idx) {
    col_index_ = idx;
}

std::string ResultNode::Show() const {
    std::string str;
    if(GetDataType()==DataType::kInt) {
        str = std::to_string(GetValue<int>());
    } else if(GetDataType()==DataType::kLong) {
        str = std::to_string(GetValue<long>());
    } else if(GetDataType()==DataType::kDouble) {
        str = std::to_string(GetValue<double>());
    } else {
        str = "[Result]";
    }
    return root_->Show() + " = " + str;
}