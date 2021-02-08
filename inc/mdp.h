#ifndef CPP_11_2_1_MDP_H_
#define CPP_11_2_1_MDP_H_

#include "node_base.h"
#include "utils.h"
#include <vector>

class MDP {
public:
    MDP() : col_num_(0), row_num_(0) { };
    ~MDP() { for(int i = 0; i < col_num_; i++) FreeValueMem(col_types_[i], cols_[i]); };

public:
    // 获取单个数据
    template <typename T>
    std::vector<T> Get(int row, int col);
    // 设置单个数据
    template <typename T>
    void Set(int row, int col, std::vector<T> values);
    void* GetColPtr(int idx);
    DataType GetColType(int idx);
    void AddCol(int len, DataType type, std::string name);
    std::string Show();

private:
    // 列名
    std::vector<std::string> col_names_;
    // 列类型
    std::vector<DataType> col_types_;
    // 列数
    int col_num_;
    // 行数
    int row_num_;
    // 列数据
    std::vector<void*> cols_;
};

template <typename T>
std::vector<T> MDP::Get(int row, int col) {
    if(col_num_ > col && row_num_ > row && (~(row == -1 && col == -1))) {
        if(row == -1) {
            std::vector<T> vec(row_num_);
            for(int i = 0; i < row_num_; i++) {
                vec.push_back(*(reinterpret_cast<T*>(cols_[col]) + i));
            }
            return vec;
        } else if(col == -1) {
            std::vector<T> vec(col_num_);
            for(int i = 0; i < col_num_; i++) {
                vec.push_back(*(reinterpret_cast<T*>(cols_[i]) + row));
            }
            return vec;
        } else {
            std::vector<T> vec(1);
            vec.push_back(*(reinterpret_cast<T*>(cols_[col]) + row));
            return vec;
        }
    }
    return std::vector<T>();
}

template <typename T>
void MDP::Set(int row, int col, std::vector<T> values) {
    // 修改数据
    if(col_num_ > col && row_num_ > row && (~(row == -1 && col == -1))) {
        if(col != -1 && cols_[col] == NULL) {
            AllocValueMem(Type2DataType<T>(), cols_[col], row_num_);
        }
        if(row == -1) {
            if(values.size() == row_num_) {
                T* ptr = reinterpret_cast<T*>(cols_[col]);
                for(int i = 0; i < row_num_; i++) {
                    *(ptr + i) = values[i];
                }
            }
        } else if(col == -1) {
            if(values.size() == col_num_) {
                for(int i = 0; i < col_num_; i++) {
                    T* ptr = reinterpret_cast<T*>(cols_[i]) + row;
                    *ptr = values[i];
                }
            }
        } else {
            if(values.size() == 1) {
                T* ptr = reinterpret_cast<T*>(cols_[col]) + row;
                *ptr = values[0];
            }
        }
    }
}

#endif