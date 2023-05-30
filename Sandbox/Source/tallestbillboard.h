#pragma once

#include <algorithm>
#include <vector>
#include <unordered_map>

#include "Solution Tester/SolutionTester.h"

namespace tallest_billboard {
	class Solution {
	public:
        std::vector<std::unordered_map<int, int>> dp;
        // dp
		int dphelper(int i, std::vector<int>& vec, int delta) {
            if (i == vec.size()) {
                if (delta == 0)
                    return 0;
                return INT_MIN;
            }

            if (dp[i].find(delta) != dp[i].end())
                return dp[i][delta];

            int a = dphelper(i + 1, vec, delta);
            int b = vec[i] + dphelper(i + 1, vec, vec[i] + delta) ;
            int c = dphelper(i + 1, vec, delta - vec[i]);

            int best = std::max({ a, b, c });
            dp[i][delta] = best;
            return best;
		}
		
		// brute force
        int helper(int i, std::vector<int>& vec, int left, int right) {
            if (i == vec.size()) {
                if (left == right)
                    return left;
                return 0;
            }

            int a = helper(i + 1, vec, left, right);
            int b = helper(i + 1, vec, left + vec[i], right);
            int c = helper(i + 1, vec, left, right + vec[i]);

            return std::max({ a, b, c });
        }

        int tallestBillboard(std::vector<int>& rods) {
            //return helper(0, rods, 0, 0);
            dp.assign(21, {});
            return dphelper(0, rods, 0);
        }

		int execute(std::vector<int>& rods) {
            return tallestBillboard(rods);
		}
	};

	class Driver : public TestDriverBase {
		virtual bool RunDriver() override {
            std::vector<int> rods{1, 2, 3};
            Solution sol;
            sol.execute(rods);
            return true;
		}
	};
}

