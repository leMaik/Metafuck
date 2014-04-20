#include "Statement.h"
#include <string>

template <typename T, typename ... List>
struct StatementFinder
{
	static Statement* find(Call const& call)
	{
		if (!T::matches(call))
			return StatementFinder<List...>::find(call);
		else
			return new T{ call };
	}
};

template <>
struct StatementFinder <void>
{
	static Statement* find(Call const& call)
	{
		return nullptr;
	}
};

template <typename ... List>
Statement* get_statement(Call const& call)
{
	return StatementFinder<List..., void>::find(call);
}