#ifndef CPP_11_2_1_OPERATOR_H_
#define CPP_11_2_1_OPERATOR_H_

#include "node_operator.h"

void* RunOperator(OptType opt, DataType &type, void* left, void* right, DataType left_type, DataType right_type, int left_len = 1, int right_len = 1);

template <typename A>
static void* RunOperatorA(OptType opt, DataType &type, void* left, void* right, DataType right_type, int left_len = 1, int right_len = 1);
template <typename A, typename B>
static void* RunOperatorB(OptType opt, DataType &type, void* left, void* right, int left_len = 1, int right_len = 1);

template <typename A>
static void* RunOperatorA(OptType opt, DataType &type, void* left, void* right, DataType right_type, int left_len, int right_len) {
    switch(right_type) {
        case DataType::kInt: return RunOperatorB<A, int>(opt, type, left, right, left_len, right_len);
        case DataType::kLong: return RunOperatorB<A, long>(opt, type, left, right, left_len, right_len);
        case DataType::kDouble: return RunOperatorB<A, double>(opt, type, left, right, left_len, right_len);
        default: return nullptr;
    }
}

template <typename A, typename B>
static void* RunOperatorB(OptType opt, DataType &type, void* left, void* right, int left_len, int right_len) {
    using CastA = typename std::conditional<std::is_floating_point<B>::value, B, A>::type;
    using CastB = typename std::conditional<std::is_floating_point<A>::value, A, B>::type;
    using Result = typename std::conditional<(sizeof(CastA) > sizeof(CastB)), CastA, CastB>::type;
    A* l = reinterpret_cast<A*>(left);
    B* r = reinterpret_cast<B*>(right);
    int len = std::max(left_len, right_len);
    type = Type2DataType<Result>();
    Result* ptr = new Result[len];
    switch (opt) {
        case OptType::kPlus: {
            if(left_len == right_len) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) + static_cast<Result>(*(r+i));
            } else if(left_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*l) + static_cast<Result>(*(r+i));
            } else if(right_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) + static_cast<Result>(*r);
            }
            return ptr;
        }
        case OptType::kSub: {
            if(left_len == right_len) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) - static_cast<Result>(*(r+i));
            } else if(left_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*l) - static_cast<Result>(*(r+i));
            } else if(right_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) - static_cast<Result>(*r);
            }
            return ptr;
        }
        case OptType::kMult: {
            if(left_len == right_len) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) * static_cast<Result>(*(r+i));
            } else if(left_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*l) * static_cast<Result>(*(r+i));
            } else if(right_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) * static_cast<Result>(*r);
            }
            return ptr;
        }
        case OptType::kDiv: {
            if(left_len == right_len) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) / static_cast<Result>(*(r+i));
            } else if(left_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*l) / static_cast<Result>(*(r+i));
            } else if(right_len == 1) {
                for(int i = 0; i < len; i++)
                    *(ptr+i) = static_cast<Result>(*(l+i)) / static_cast<Result>(*r);
            }
            return ptr;
        }
        default: return nullptr;
    }
}

#endif