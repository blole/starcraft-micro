#pragma once

#include <BWAPI.h>

#include <cassert>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

#include <boost/range.hpp>
using boost::sub_range;

#include <vector>
#include <deque>
#include <list>
#include <functional>
#include <iostream>
#include <algorithm>
#include <iterator>
using std::vector;
using std::deque;
using std::list;
using std::function;
using std::endl;

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
	template<class ForwardIt, class R, class T = typename iterator_traits<ForwardIt>::reference>
	ForwardIt min_element(ForwardIt first, ForwardIt last, function<R(T)> map)
	{
		return min_element(first, last, [&map](const T& a, const T& b) { return min(map(a), map(b)); });
	}
	template<class ForwardIt, class R, class T = typename iterator_traits<ForwardIt>::reference>
	ForwardIt max_element(ForwardIt first, ForwardIt last, function<R(T)> map)
	{
		return max_element(first, last, [&map](const T& a, const T& b) { return max(map(a), map(b)); });
	}
}
