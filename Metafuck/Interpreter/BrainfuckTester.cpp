#include "BrainfuckTester.h"

namespace BrainfuckCompiler {
	const std::map <ErrorMessageID, std::string> errors = {
		{EID_UNKNOWN_ERROR, "UNKNOWN ERROR"},
		{EID_INVALID_LOOPS, "at least one jump instruction has a missing counterpart"},
		{EID_BAILING_OUT, "earlier errors prevent further testings"}
	};
//#############################################################
	BrainfuckTester::BrainfuckTester()
	{
		
	}
//#############################################################
	void TestResults::addEntry(EntrySeverityLevel severity_level, ErrorMessageID message_id)
	{
		entries_.push_back({severity_level, message_id});
	}
	bool TestResults::containsEntryOfLevel(EntrySeverityLevel severity_level)
	{
		for (auto const& e : entries_) {
			if (e.serverity_level == severity_level)
				return true;
		}
		return false;
	}
//#############################################################
	void BrainfuckTester::runTest(std::string code, std::string input, VirtualMemory& memory, BrainfuckTestLevel level)
	{
		switch (level) {
			case (BFTL_SYNTAX): {
				runSyntaxCheck(code);
				break;
			}
			case (BFTL_PREDICTION): {
				runSyntaxCheck(code);
				runPredictionEngine(code);
				break;
			}
			case (BFTL_THOROUGH): {
				runSyntaxCheck(code);
				runPredictionEngine(code);
				checkRuntime(code);
				break;
			}
			default:
				break;
		}
	}
	
	void BrainfuckTester::runSyntaxCheck(std::string const& code)
	{
		BrainfuckPrecompiler compiler;

		auto err =  compiler.verify(code);
		switch (err) 
		{
			case(CEC_NO_ERROR): {
				break;
			}
			case(CEC_LOOP_MISMATCH): {
				results_.addEntry(ESL_CRITICAL, EID_INVALID_LOOPS);
				break;
			}
			default:
				break;
		}	
	}
	void BrainfuckTester::runPredictionEngine(std::string const& code)
	{
		if (results_.containsEntryOfLevel(ESL_CRITICAL)) {
			results_.addEntry(ESL_CRITICAL, EID_BAILING_OUT);
			return;
		}

		BrainfuckPrecompiler compiler;
	}
	void BrainfuckTester::checkRuntime(std::string const& code)
	{
		if (results_.containsEntryOfLevel(ESL_CRITICAL)) {
			results_.addEntry(ESL_CRITICAL, EID_BAILING_OUT);
			return;
		}
	}
//#############################################################
}