#ifndef CPP_11_2_1_UTILS_H_
#define CPP_11_2_1_UTILS_H_

#include "node_base.h"
#include <type_traits>

// 释放void*指向的内存
void FreeValueMem(DataType type, void* ptr);
// 开辟给定类型的内存空间
int AllocValueMem(DataType type, void* &ptr, int len = 1);
// 对空间进行类型转换
void AltValueMem(DataType from_type, DataType to_type, void* &ptr, int len = 1);
// 变量转字符串
std::string Value2Str(DataType type, void* ptr);
std::string Value2Str(DataType type, void* ptr, int offset);

// 变量类型到类型枚举的映射
template <typename T>
DataType Type2DataType();
// AltValueMem 调用，将原始类型转换为模板A的指针
template <typename A>
static void AltValueMemA(DataType from_type, DataType to_type, void* &ptr, int len = 1);
// AltValueMemA 调用，将原始类型转换为模板A的指针后，将目标类型转为模板B
template <typename A, typename B>
static void AltValueMemB(void* &ptrA, void* &ptrB, int len = 1);

template <typename T>
DataType Type2DataType() {
    if(typeid(T) == typeid(int))  return DataType::kInt;
    else if(typeid(T) == typeid(long )) return DataType::kLong;
    else if(typeid(T) == typeid(double )) return DataType::kDouble;
    else return DataType::kNULL;
}

template <typename A>
static void AltValueMemA(DataType from_type, DataType to_type, void* &ptr, int len) {
    void* temp;
    AllocValueMem(to_type, temp, len);
    switch (to_type) {
        case DataType::kInt: {
            AltValueMemB<A, int>(ptr, temp, len);
            break;
        }
        case DataType::kLong: {
            AltValueMemB<A, long>(ptr, temp, len);
            break;
        }
        case DataType::kDouble: {
            AltValueMemB<A, double>(ptr, temp, len);
            break;
        }
        default: FreeValueMem(to_type, temp); temp = nullptr; return;
    }
    FreeValueMem(from_type, ptr);
    ptr = temp;
}

template <typename A, typename B>
static void AltValueMemB(void* &ptrA, void* &ptrB, int len) {
    for(int i = 0; i < len; i++) {
        B *t = reinterpret_cast<B*>(ptrB) + i;
        *t = static_cast<B>(*(reinterpret_cast<A*>(ptrA) + i));
    }
}

#endif