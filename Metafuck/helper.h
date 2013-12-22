#include <stack>

template <typename T>
T pop(std::stack<T> &s);

template <typename T>
T pop(std::stack<T> &s) {
	T l = s.top();
	s.pop();
	return l;
}