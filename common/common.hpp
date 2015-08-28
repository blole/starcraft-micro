#pragma once

#include <BWAPI.h>

#include <cassert>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;

#include <boost/range.hpp>
using boost::sub_range;

#include <vector>
#include <deque>
#include <list>
using std::vector;
using std::deque;
using std::list;

template <class T>
vector<unique_ptr<T>> clone(const vector<unique_ptr<T>>& in)
{
	vector<unique_ptr<T>> out;
	out.reserve(in.size());
	for (auto& e : in)
		out.emplace_back(e->clone());
	return out;
}
