#pragma once

#include <BWAPI.h>
using BWAPI::Broodwar;

#include <cassert>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <functional>
#include <iostream>
#include <algorithm>
#include <iterator>
using std::vector;
using std::deque;
using std::list;
using std::map;
using std::unordered_map;
using std::function;
using std::endl;
using std::string;

template <class T>
vector<unique_ptr<T>> clone(const vector<unique_ptr<T>>& in)
{
	vector<unique_ptr<T>> out;
	out.reserve(in.size());
	for (auto& e : in)
		out.emplace_back(e->clone());
	return out;
}

namespace std
{
	template<class ForwardIt, class R, class T>
	ForwardIt min_element(ForwardIt first, ForwardIt last, const function<R(T)>& map)
	{
		return min_element(first, last, [&map](T a, T b) { return map(a) < map(b); });
	}
	template<class ForwardIt, class R, class T>
	ForwardIt max_element(ForwardIt first, ForwardIt last, const function<R(T)>& map)
	{
		return max_element(first, last, [&map](T a, T b) { return map(a) < map(b); });
	}
}
