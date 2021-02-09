#include "mdp.h"
#include <string.h>

void MDP::AddCol(DataType type, std::string name) {
    ++col_num_;
    col_types_.push_back(type);
    void* ptr;
    int bytes = AllocValueMem(type, ptr, row_num_);
    if(ptr != nullptr) {
        memset(ptr, 0, bytes);
    }
    cols_.push_back(ptr);
    col_names_.push_back(name);
}

void MDP::FreeColMem(int idx) {
    if(idx < col_num_) {
        FreeValueMem(col_types_[idx], cols_[idx]);
    }
}

void MDP::SetColType(int idx, DataType type) {
    if(idx < col_num_) {
        DataType &top = col_types_[idx];
        top = type;
    }
}

void MDP::SetColPtr(int idx, void* ptr) {
    if(idx < col_num_) {
        void* &top = cols_[idx];
        top = ptr;
    }
}

void* MDP::GetColPtr(int idx) {
    if(idx < col_num_) {
        return cols_[idx];
    }
    return nullptr;
}

DataType MDP::GetColType(int idx) {
    return col_types_[idx];
}

int MDP::GetColNum() {
    return col_num_;
}

int MDP::GetRowNum() {
    return row_num_;
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
