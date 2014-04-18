#include "Statement.h"
#include <string>

template <typename ... List>
Statement* getStatement(std::string const& code);