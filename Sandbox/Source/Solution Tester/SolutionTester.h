#pragma once

class TestDriverBase {
public:
	TestDriverBase() {}
	~TestDriverBase() {};

	virtual bool RunDriver() = 0;
};

class SolutionTester {
public:
	SolutionTester() {};
	~SolutionTester() {};

	template<class Return, class SolutionObj, class ...Input>
	Return execute(SolutionObj* object, Input ... input) {
		return object->execute(input...);
	}

	bool execute(std::shared_ptr<TestDriverBase> driver) {
		return driver->RunDriver();
	}
};

