#pragma once
#include <vector>
#include <iostream>
#include "Solution Tester/SolutionTester.h"

namespace knapsack {
    class Solution {
    public:
        int max(int a, int b) { return (a > b) ? a : b; }

        int knapSack(int W, std::vector<int> const& wt, std::vector<int> const& val, int n)
        {
            int i, w;
            std::vector<std::vector<int>> K(n + 1, std::vector<int>(W + 1));

            // Build table K[][] in bottom up manner
            for (i = 0; i <= n; i++) {
                for (w = 0; w <= W; w++) {
                    if (i == 0 || w == 0)
                        K[i][w] = 0;
                    else if (wt[i - 1] <= w)
                        K[i][w] = max(val[i - 1]
                            + K[i - 1][w - wt[i - 1]],
                            K[i - 1][w]);
                    else
                        K[i][w] = K[i - 1][w];
                }
            }
            return K[n][W];
        }

        int execute(int w, std::vector<int> const& wt, std::vector<int> const& val, int n) {
            return knapSack(w, wt, val, n);
        }
    };

    class Driver : public TestDriverBase {
        virtual bool RunDriver() override {
            std::vector<int> cost = { 60, 100, 120 };
            std::vector<int> aff = { 10, 20, 30 };
            int W = 1000;
            int n = sizeof(cost) / sizeof(cost[0]);
            Solution sol;
            auto result = sol.execute(W, aff, cost, n);
            std::cout << result;
            return true;
        }
    };
}
