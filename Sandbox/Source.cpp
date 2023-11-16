#include <iostream>
#include <map>

#include <thread>
#include <mutex>

#include "Source/Solution Tester/SolutionTester.h"
#include "Source/crossword.h"

int main() {
	SolutionTester solution;
	auto* driver = new crossword::Driver();
	solution.execute(std::shared_ptr<TestDriverBase>(driver));

	return 1;
}
