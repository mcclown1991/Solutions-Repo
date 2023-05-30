#pragma once
#include <vector>
#include <iostream>
#include "Solution Tester/SolutionTester.h"

namespace knapsack {
    class Solution {
    public:
        size_t max(size_t a, size_t b) { return (a > b) ? a : b; }

        size_t knapSack(size_t W, std::vector<size_t> const& wt, std::vector<size_t> const& val, size_t n)
        {
            size_t i, w;
            std::vector<std::vector<size_t>> K(n + 1, std::vector<size_t>(W + 1));

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

        size_t knapSackSpaceOpt(size_t W, std::vector<size_t> const& wt, std::vector<size_t> const& val, size_t n) {
        // Making and initializing dp array
        std::vector<size_t> dp(W + 1, 0);

        for (int i = 1; i < n + 1; i++) {
            for (int w = W; w >= 0; w--) {

                if (wt[i - 1] <= w)

                    // Finding the maximum value
                    dp[w] = max(dp[w],
                        dp[w - wt[i - 1]] + val[i - 1]);
            }
        }
        // Returning the maximum value of knapsack
        return dp[W];
	}

        size_t execute(size_t w, std::vector<size_t> const& wt, std::vector<size_t> const& val, size_t n) {
            return knapSackSpaceOpt(w, wt, val, n);
        }
    };

	

    class Driver : public TestDriverBase {
        virtual bool RunDriver() override {
            std::vector<size_t> cost = { 60, 100, 120, 220, 200 };
            std::vector<size_t> aff = { 10, 20, 30, 10, 15};
            size_t W = 50;
            size_t n = cost.size();
            Solution sol;
            auto result = sol.execute(W, aff, cost, n);
            std::cout << result;
            return true;
        }
    };
}
