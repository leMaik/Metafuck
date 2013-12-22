#ifndef BRAINFUCKTESTER_H_INCLUDED
#define BRAINFUCKTESTER_H_INCLUDED

#include "PrecomFuck.h"

#include <string>
#include <sstream>
#include <map>

namespace BrainfuckCompiler {

	enum BrainfuckTestLevel {
		BFTL_SYNTAX,		// check compiler verify (can the code actually be compiled?) and do obvious checkings
		BFTL_PREDICTION,	// enable error prediction
		BFTL_THOROUGH		// run the program and log overflows and memptr offset failures (!DO NOT USE WITH PROGRAMS THAT DO NOT TERMINATE!)
	};

	enum ErrorMessageID {
		EID_UNKNOWN_ERROR,
		EID_INVALID_LOOPS,
		EID_BAILING_OUT
	};

	enum EntrySeverityLevel {
		ESL_INFORMATION,
		ESL_WARNING,
		ESL_ERROR,
		ESL_CRITICAL
	};

	extern const std::map <ErrorMessageID, std::string> errors;

	struct TestResultEntry {
		EntrySeverityLevel serverity_level;
		ErrorMessageID message_id;
	};

	class TestResults {
	public:
		void addEntry(EntrySeverityLevel severity_level, ErrorMessageID message_id);
		bool containsEntryOfLevel(EntrySeverityLevel severity_level);
	private:
		std::vector <TestResultEntry> entries_;
	};

	class BrainfuckTester {
	public:
		BrainfuckTester();
		void runTest(std::string code, std::string input, VirtualMemory& memory, BrainfuckTestLevel level = BFTL_SYNTAX);

	private:
		TestResults results_;

	private: // member function
		void runSyntaxCheck(std::string const& code);
		void runPredictionEngine(std::string const& code);
		void checkRuntime(std::string const& code);
	};

} // namespace BrainfuckCompiler

#endif // BRAINFUCKTESTER_H_INCLUDED