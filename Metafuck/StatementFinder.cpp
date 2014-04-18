#include "StatementFinder.h"

template <typename T, typename ... List>
struct StatementFinder
{
	static Statement* find(std::string const& code)
	{
		if (!T::matches(code))
			return A<List...>::find(code);
		else
			return new T{};
	}
};

template <>
struct StatementFinder <void>
{
	static Statement* find(std::string const& code)
	{
		return nullptr;
	}
};

template <typename ... List>
Statement* getStatement(std::string const& code)
{
	return StatementFinder<List..., void>::find(code);
}