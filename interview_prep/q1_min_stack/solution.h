#pragma once
#include <stack>
#include <climits>

class MinStack {
public:
	// stack that holds a pair? second element is the last min
	// push(1) -- 1, 1
	// push(2) -- {2, 1}, {1, 1}
	// push(0) -- {0, 0}, {2, 1}, {1, 1}
	// push(4) -- {4, 0}, {0, 0}, {2, 1}, {1, 1}
	// push(-1) -- {-1, -1}, {4, 0}, {0, 0}, {2, 1}, {1, 1}

	std::stack<std::pair<int, int>> s;
	int min = 2147483647;
	MinStack(){}

	void push(int val) {
		if (val < min) min = val;
		s.push({val, min});	
	}

	void pop() {
		s.pop();
	}

	int top() {
		std::pair<int, int> node = s.top();
		return node.first;
	}

	int getMin() {
		std::pair<int, int> node = s.top();
		return node.second;
	}
};
