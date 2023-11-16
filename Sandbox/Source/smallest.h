#pragma once
#include <algorithm>
#include <string>
#include <vector>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
using namespace std;
class solution {
public:
	int findsmallest(string& S) {
		// write your code in C++14 (g++ 6.2.0)
		// convert string to int
		int res = 0;
		for (auto i = S.size() - 1; i > 0; --i) {
			++res;
			if (S[i] == '1') { // even
				S[i] = '0';
				++i;
			}
		}
		if (S[0] == '1') ++res;
		return res;
	}
};

