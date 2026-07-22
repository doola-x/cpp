#pragma once
#include <unordered_map>
#include <list>

class LRUCache {
public:
	int capacity;
	std::unordered_map<int, std::list<std::pair<int, int>>::iterator> map;
	std::list<std::pair<int, int>> order;

	LRUCache(int _capacity) : capacity(_capacity) {}

	int get(int key) {
		auto it = map.find(key); 
		if (it != map.end()) {
			//found key, return value and adjust order
			order.splice(order.begin(), order, it->second);
			return it->second->second;
		}
		return -1;
	}

	void put(int key, int value) {
		auto it = map.find(key);
		if (it != map.end()) {
			//found key, overwrite previous value, mark used
			it->second->second = value;
			order.splice(order.begin(), order, it->second);
			return;
		}
		if ((int)order.size() == capacity) {
			map.erase(order.back().first);
			order.pop_back();
		}
		
		order.emplace_front(key, value);
		map[key] = order.begin();
	}
};
