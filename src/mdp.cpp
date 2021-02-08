#include "mdp.h"
#include <string.h>

void MDP::AddCol(int len, DataType type, std::string name) {
    if(row_num_ == 0 || row_num_ == len) {
        row_num_ = len;
        ++col_num_;
        col_types_.push_back(type);
        void* ptr;
        int bytes = AllocValueMem(type, ptr, len);
        if(ptr != NULL) {
            memset(ptr, 0, bytes);
        }
        cols_.push_back(ptr);
        col_names_.push_back(name);
    }
}

void* MDP::GetColPtr(int idx) {
    if(idx < col_num_) {
        return cols_[idx];
    }
}

DataType MDP::GetColType(int idx) {
    return col_types_[idx];
}

std::string MDP::Show() {
    std::string str;
    // 打印表头
    for(int i = 0; i < col_num_; i++) {
        str += col_names_[i];
        if(i != (col_num_ - 1)) {
            str += "\t";
        }
    }
    str += "\n";
    // 打印数据
    for(int i = 0; i < row_num_; i++) {
        for(int j = 0; j < col_num_; j++) {
            str += Value2Str(col_types_[j], cols_[j], i);
            if(j != (col_num_ - 1)) {
                str += "\t";
            }
        }
        str += "\n";
    }
    return str;
}
