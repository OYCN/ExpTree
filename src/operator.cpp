#include "operator.h"

void* RunOperator(OptType opt, DataType &type, void* left, void* right, DataType left_type, DataType right_type, int left_len, int right_len) {
    switch(left_type) {
        case DataType::kInt: return RunOperatorA<int>(opt, type, left, right, right_type, left_len, right_len);
        case DataType::kLong: return RunOperatorA<long>(opt, type, left, right, right_type, left_len, right_len);
        case DataType::kDouble: return RunOperatorA<double>(opt, type, left, right, right_type, left_len, right_len);
        default: return nullptr;
    }
}