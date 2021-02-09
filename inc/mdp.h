#ifndef CPP_11_2_1_MDP_H_
#define CPP_11_2_1_MDP_H_

#include "node_base.h"
#include "utils.h"
#include <vector>
#include <initializer_list>

class MDP {
public:
    MDP(int rows) : col_num_(0), row_num_(rows) { }
    ~MDP() {
        for(int i = 0; i < col_num_; i++) {
            FreeValueMem(col_types_[i], cols_[i]);
            cols_[i] = nullptr;
        }
    }

public:
    // 获取单个数据
    template <typename T>
    T Get(int row, int col);
    // 设置单个数据
    template <typename T>
    void Set(int row, int col, T values);
    template <typename T>
    void SetCol(int idx, std::initializer_list<T> input);
    void SetColType(int idx, DataType type);
    void SetColPtr(int idx, void* ptr);
    void* GetColPtr(int idx);
    DataType GetColType(int idx);
    int GetColNum();
    int GetRowNum();
    void AddCol(DataType type, std::string name);
    void FreeColMem(int idx);
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
T MDP::Get(int row, int col) {
    if(col_num_ > col && row_num_ > row) {
        std::vector<T> vec(1);
        vec.push_back(*(reinterpret_cast<T*>(cols_[col]) + row));
        return vec;
    }
    return T();
}

template <typename T>
void MDP::Set(int row, int col, T value) {
    // 修改数据
    if(col_num_ > col && row_num_ > row) {
        T* ptr = reinterpret_cast<T*>(cols_[col]) + row;
        *ptr = value;
    }
}

template <typename T>
void MDP::SetCol(int idx, std::initializer_list<T> values) {
    T* data = reinterpret_cast<T*>(cols_[idx]);
    for (const T* i = values.begin(); i != values.end(); ++i)
    {
        *data = *i;
        ++data;
    }
}

#endif