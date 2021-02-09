#ifndef CPP_11_2_1_CALCULATOR_BASE_H_
#define CPP_11_2_1_CALCULATOR_BASE_H_

#include "node_operator.h"
#include "mdp.h"

class ICalculator {
public:
    ICalculator() : root_(nullptr) { }

public:
    virtual void bind(INode*) = 0;
    virtual void calculate(MDP *mdp) = 0;

protected:
    // 传入运算、左右数据及类型，得到结果指针与类型
    void* RunOperator(OptType opt, DataType &type, void* left, void* right, DataType left_type, DataType right_type, int left_len = 1, int right_len = 1);
    // 左边值转换为模板类型A
    template <typename A>
    void* RunOperatorA(OptType opt, DataType &type, void* left, void* right, DataType right_type, int left_len = 1, int right_len = 1);
    // 右边值转换为模板类型B
    template <typename A, typename B>
    void* RunOperatorB(OptType opt, DataType &type, void* left, void* right, int left_len = 1, int right_len = 1);
    INode *root_;
};

template <typename A>
void* ICalculator::RunOperatorA(OptType opt, DataType &type, void* left, void* right, DataType right_type, int left_len, int right_len) {
    switch(right_type) {
        case DataType::kInt: return RunOperatorB<A, int>(opt, type, left, right, left_len, right_len);
        case DataType::kLong: return RunOperatorB<A, long>(opt, type, left, right, left_len, right_len);
        case DataType::kDouble: return RunOperatorB<A, double>(opt, type, left, right, left_len, right_len);
        default: return nullptr;
    }
}

template <typename A, typename B>
void* ICalculator::RunOperatorB(OptType opt, DataType &type, void* left, void* right, int left_len, int right_len) {
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