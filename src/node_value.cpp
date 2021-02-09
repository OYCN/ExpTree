#include "node_value.h"

void* ValueNode::GetValuePtr() const {
    return value_;
}

std::string ValueNode::Show() const {
    std::string str;
    switch (GetDataType()) {
        case DataType::kInt: {
            str = std::to_string(GetValue<int>());
            if(*(reinterpret_cast<int*>(value_)) < 0) {
                str = "(" + str + ")";
            }
            break;
        }
        case DataType::kLong: {
            str = std::to_string(GetValue<long>());
            if(*(reinterpret_cast<long*>(value_)) < 0) {
                str = "(" + str + ")";
            }
            break;
        }
        case DataType::kDouble: {
            str = std::to_string(GetValue<double>());
            if(*(reinterpret_cast<double*>(value_)) < 0) {
                str = "(" + str + ")";
            }
            break;
        }
        default: break;
    }
    return str;
}