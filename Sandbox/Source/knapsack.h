#pragma once
#include <vector>
#include <iostream>
#include "Solution Tester/SolutionTester.h"

namespace knapsack {
    class Solution {
    public:
        size_t max(size_t a, size_t b) { return (a > b) ? a : b; }

    	struct page {
        public:
            size_t affinity;
            size_t budget;

        	page(size_t a, size_t b) : affinity(a), budget(b) {}
        };

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

        size_t knapSackSpaceOpt(size_t budget, std::vector<page> const& data) {
	        // Making and initializing dp array
	        std::vector<size_t> dp(budget + 1, 0);

	        for (int i = 1; i < data.size() + 1; i++) {
	            for (int w = budget; w >= 0; w--) {

	                if (data[i - 1].affinity <= w)

	                    // Finding the maximum value
	                    dp[w] = max(dp[w],
	                        dp[w - data[i - 1].affinity] + data[i - 1].budget);
	            }
	        }
	        // Returning the maximum value of knapsack
	        return dp[budget];
		}

    	size_t filter(size_t total, std::vector<size_t> const& aff, std::vector<size_t> const& budget) {
            std::vector<page> data;
	        for(auto i = 0; i < aff.size(); i++) {
                data.emplace_back(page(aff[i], budget[i]));
	        }
            decltype(data) prim;
            size_t avg = total / (aff.size() + 1);
			for(auto i : data) {
				if(i.budget >= avg) {
                    i.budget = avg;
                    prim.emplace_back(i);
				}
			}
            return knapSackSpaceOpt(total - avg, prim);
        }

        size_t execute(size_t total, std::vector<size_t> const& aff, std::vector<size_t> const& budget) {
            return filter(total, aff, budget);
        }
    };

	

    class Driver : public TestDriverBase {
        virtual bool RunDriver() override {
            std::vector<size_t> cost = { 600, 100, 200, 800 };
            std::vector<size_t> aff = { 20, 20, 30, 60 };
            size_t W = 1000;
            Solution sol;
            auto result = sol.execute(W, aff, cost);
            std::cout << result;
            return true;
        }
    };
}
