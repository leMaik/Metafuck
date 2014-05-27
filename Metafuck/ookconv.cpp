#include "ookconv.h"
#include <sstream>
#include <map>

std::string bf2ook(const std::string& bf)
{
	const std::map<char, std::string> bf_ook = {
		{ '+', "Ook. Ook." },
		{ '-', "Ook! Ook!" },
		{ '>', "Ook. Ook?" },
		{ '<', "Ook? Ook." },
		{ '[', "Ook! Ook?" },
		{ ']', "Ook? Ook!" },
		{ '.', "Ook! Ook." },
		{ ',', "Ook. Ook!" }
	};

	std::stringstream ook;

	for (auto c : bf) {
		auto o = bf_ook.find(c);
		if (o != bf_ook.end())
			ook << o->second;
		else
			ook << c;
		ook << ' ';
	}

	auto ookstr = ook.str();
	return ookstr.substr(0, ookstr.length() - 1);
}