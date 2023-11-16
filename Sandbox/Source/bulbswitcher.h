#pragma once
#include <math.h>

class Solution {
public:
	int bulbSwitch(int n) {
		// for range [1-n] at every ith iteration, bulb divisable by i will be toggled
		// example: for range [1-9], when i = 8, bulb 1, 2, 4, 8 is toggled and 1, 2, 4, 8 is 8's divisor
		// each bulb is toggled on every odd iteration
		// iteration 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
		// toggle    1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1
		// so we can conclude that we need to find how many intergers in range [1,n] have an odd number of divisor
		// example for number 6, divisor -> 1, 2, 3, 6 can be expressed as 1x6 and 2x3
		// for number 9, divisor -> 1, 3, 9 can be expressed as 1x9 and 3x3 so for number with odd divisors are numbers that can be expressed as the square of an interger also known as perfect squares
		// so what we need to find is how many integers in the range [1, n] are prefect squares
		// sqrt(n) - sqrt(1) + 1 = sqrt(n)
		// so prefect squares in range [1, n] = sqrt(n)
		return sqrt(n);
	}
};