#pragma once
#include <chrono>
#include <map>
#include <string>
#include <functional>

using namespace std;
using namespace std::chrono;

class Timer
{
public:
	inline int getDeltaTime()	{ return (duration_cast<milliseconds>(system_clock::now() - last).count()); }
	inline void reset() { last = system_clock::now(); }

	void addBeforeHandler(string name,function<void()> func)	{ functions_before.insert(make_pair(name,func)); }
	void deleteBeforeHandler(string name) { 
		auto f = functions_before.find(name);
		if (f != functions_before.end()) functions_before.erase(name);
	}

	void addAfterHandler(string name, function<void()> func)	{ functions_after.insert(make_pair(name, func)); }
	void deleteAfterHandler(string name) {
		auto f = functions_after.find(name);
		if (f != functions_after.end()) functions_after.erase(name);
	}
	inline void doBefore() { for (auto f : functions_before) f.second(); }
	inline void doAfter() { for (auto f : functions_after) f.second(); }
private: 
	system_clock::time_point last;
	map<string,function<void()>> functions_before;
	map<string, function<void()>> functions_after;
};

