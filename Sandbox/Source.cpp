#include <iostream>
#include <map>

#include "Source/Solution Tester/SolutionTester.h"
#include "Source/knapsack.h"

int main() {
	SolutionTester solution;
	auto* driver = new knapsack::Driver();
	solution.execute(std::shared_ptr<TestDriverBase>(driver));
	return 1;
}
