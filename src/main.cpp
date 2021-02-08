#include <iostream>
#include "node_base.h"
#include "node_value.h"
#include "node_column.h"
#include "node_operator.h"
#include "node_result.h"
#include "utils.h"
#include "mdp.h"
#include "calculator_simple.h"

int main(int argc, char *argv[]) {
    ResultNode root1;
    // 表达式树构建(10 + (-3)) * 1.2 / (2 - 1) = result
    root1.SetRoot(
        new OperatorNode(
            OptType::kMult,
            new OperatorNode(
                OptType::kPlus,
                new ValueNode(10),
                new ValueNode(-3)
            ),
            new OperatorNode(
                OptType::kDiv,
                new ValueNode(1.2),
                new OperatorNode(
                    OptType::kSub,
                    new ValueNode(2),
                    new ValueNode(1)
                )
            )
        )
    );
    std::cout << root1.Show() << std::endl;
    SimpleCalculator cal;
    cal.bind(&root1);
    cal.calculate();
    std::cout << root1.Show() << std::endl;

    ResultNode root2;
    // 表达式树构建(c1 + (-3)) * c1 / (2 - c2) = result
    root2.SetRoot(
        new OperatorNode(
            OptType::kMult,
            new OperatorNode(
                OptType::kPlus,
                new ColumnNode(0, "c1"),
                new ValueNode(-3)
            ),
            new OperatorNode(
                OptType::kDiv,
                new ColumnNode(0, "c1"),
                new OperatorNode(
                    OptType::kSub,
                    new ValueNode(2),
                    new ColumnNode(1, "c2")
                )
            )
        )
    );
    std::cout << root2.Show() << std::endl;
    MDP mdp;
    mdp.AddCol(3, DataType::kInt, "c1");
    mdp.AddCol(3, DataType::kDouble, "c2");
    mdp.AddCol(3, DataType::kNULL, "Result");
    mdp.Set<int>(-1, 0, std::vector<int>({-1, 2, 3}));
    mdp.Set<double>(-1, 1, std::vector<double>({0, -1.2, +1.2}));
    std::cout << mdp.Show() << std::endl;

    return 0;
}