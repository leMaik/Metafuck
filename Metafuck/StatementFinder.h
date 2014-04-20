#include "Statement.h"
#include <string>

template <typename T, typename ... List>
struct StatementFinder
{
	static Statement* find(std::string const& code)
	{
		if (!T::matches(code))
			return StatementFinder<List...>::find(code);
		else
			return new T{ code };
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
Statement* get_statement(std::string const& code)
{
	return StatementFinder<List..., void>::find(code);
}