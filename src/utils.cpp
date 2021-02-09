#include "utils.h"

void FreeValueMem(DataType type, void* ptr) {
    if(ptr != nullptr) {
        switch (type) {
            case DataType::kInt: delete reinterpret_cast<int*>(ptr); break;
            case DataType::kLong: delete reinterpret_cast<long*>(ptr); break;
            case DataType::kDouble: delete reinterpret_cast<double*>(ptr); break;
            default: free(ptr);break;
        }
    }
}

int AllocValueMem(DataType type, void* &ptr, int len) {
    switch (type) {
        case DataType::kInt: ptr = new int[len]; return sizeof(int)*len;
        case DataType::kLong: ptr = new long[len]; return sizeof(long)*len;
        case DataType::kDouble: ptr = new double[len]; return sizeof(double)*len;
        default: ptr = nullptr; return 0;
    }
}

void AltValueMem(DataType from_type, DataType to_type, void* &ptr, int len) {
    switch (from_type) {
        case DataType::kInt: AltValueMemA<int>(from_type, to_type, ptr, len); break;
        case DataType::kLong: AltValueMemA<long>(from_type, to_type, ptr, len); break;
        case DataType::kDouble: AltValueMemA<double>(from_type, to_type, ptr, len); break;
        default: break;
    }
}

std::string Value2Str(DataType type, void* ptr) {
    switch (type) {
        case DataType::kInt: return std::to_string(*(reinterpret_cast<int*>(ptr)));
        case DataType::kLong: return std::to_string(*(reinterpret_cast<long*>(ptr)));
        case DataType::kDouble: return std::to_string(*(reinterpret_cast<double*>(ptr)));
        default: return std::string("Null");
    }
}

std::string Value2Str(DataType type, void* ptr, int offset) {
    if(ptr != nullptr) {
        switch (type) {
            case DataType::kInt: return std::to_string(*(reinterpret_cast<int*>(ptr) + offset));
            case DataType::kLong: return std::to_string(*(reinterpret_cast<long*>(ptr) + offset));
            case DataType::kDouble: return std::to_string(*(reinterpret_cast<double*>(ptr) + offset));
            default: return std::string("NaN");
        }
    } else {
        return std::string("");
    }
}
