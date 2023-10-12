#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include "flags.h"
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <windows.h>

using std::string;

// Priority class names:
static const string ABOVE_NORMAL_PRIORITY_CLASS_STR = "ABOVE_NORMAL_PRIORITY_CLASS";   // 0x00008000
static const string BELOW_NORMAL_PRIORITY_CLASS_STR = "BELOW_NORMAL_PRIORITY_CLASS";   // 0x00004000
static const string HIGH_PRIORITY_CLASS_STR = "HIGH_PRIORITY_CLASS";           // 0x00000080
static const string IDLE_PRIORITY_CLASS_STR = "IDLE_PRIORITY_CLASS";           // 0x00000040
static const string NORMAL_PRIORITY_CLASS_STR = "NORMAL_PRIORITY_CLASS";         // 0x00000020
static const string PROCESS_MODE_BACKGROUND_BEGIN_STR = "PROCESS_MODE_BACKGROUND_BEGIN"; // 0x00100000
static const string PROCESS_MODE_BACKGROUND_END_STR   = "PROCESS_MODE_BACKGROUND_END";   // 0x00200000
static const string REALTIME_PRIORITY_CLASS_STR       = "REALTIME_PRIORITY_CLASS";       // 0x00000100


class CommandLineParser {
private:

	static const size_t DEFAULT_M_ITERATIONS      = 10 * 1000 * 1000;
	static const DWORD  DEFAULT_M_PRIORITY_CLASS  = NORMAL_PRIORITY_CLASS;
	static const int    DEFAULT_M_THREAD_PRIORITY = THREAD_PRIORITY_NORMAL;

	bool m_iteration_flag_present       = false;
	bool m_priority_class_flag_present  = false;
	bool m_thread_priority_flag_present = false;
	bool m_help_flag_present            = false;

	char** m_argv;
	int m_argc;
	int m_argument_index = 1; // ... = 1: Omit the program name from the command line.
	std::unordered_map<string, std::function<void(CommandLineParser&)>> m_flag_processor_map;
	std::unordered_map<string, DWORD> priority_class_name_map;
	std::map<string, int> thread_priority_name_map;
	std::unordered_set<string> thread_priority_number_set;
	std::unordered_set<string> flag_set;

	size_t m_iterations = DEFAULT_M_ITERATIONS;
	DWORD m_priority_class = DEFAULT_M_PRIORITY_CLASS;
	int m_thread_priority = DEFAULT_M_THREAD_PRIORITY;

	void parseCommandLine() {
		while (m_argument_index < m_argc) {
			processFlagPair();

			if (m_help_flag_present) {
				return;
			}
		}
	}

	void loadDispatchMap() {
		std::function<void(CommandLineParser&)> function_flag_iterations = &CommandLineParser::processIterationFlags;

		m_flag_processor_map[FLAG_LONG_NUMBER_OF_ITERATIONS]  = function_flag_iterations;
		m_flag_processor_map[FLAG_SHORT_NUMBER_OF_ITERATIONS] = function_flag_iterations;

		std::function<void(CommandLineParser&)> function_flag_priority_class = &CommandLineParser::processPriorityClassFlags;

		m_flag_processor_map[FLAG_LONG_PRIORITY_CLASS]  = function_flag_priority_class;
		m_flag_processor_map[FLAG_SHORT_PRIORITY_CLASS] = function_flag_priority_class;

		std::function<void(CommandLineParser&)> function_flag_thread_priority = &CommandLineParser::processThreadPriorityFlags;

		m_flag_processor_map[FLAG_LONG_THREAD_PRIORITY]  = function_flag_thread_priority;
		m_flag_processor_map[FLAG_SHORT_THREAD_PRIORITY] = function_flag_thread_priority;

		std::function<void(CommandLineParser&)> function_flag_help = &CommandLineParser::processHelpFlags;

		m_flag_processor_map[FLAG_LONG_HELP]  = function_flag_help;
		m_flag_processor_map[FLAG_SHORT_HELP] = function_flag_help;
	}

	void loadFlagSet() {
		flag_set.emplace(FLAG_LONG_HELP);
		flag_set.emplace(FLAG_LONG_PRIORITY_CLASS);
		flag_set.emplace(FLAG_LONG_THREAD_PRIORITY);
		flag_set.emplace(FLAG_LONG_NUMBER_OF_ITERATIONS);

		flag_set.emplace(FLAG_SHORT_HELP);
		flag_set.emplace(FLAG_SHORT_PRIORITY_CLASS);
		flag_set.emplace(FLAG_SHORT_THREAD_PRIORITY);
		flag_set.emplace(FLAG_SHORT_NUMBER_OF_ITERATIONS);
	}

	void checkFlagIsValid(string& flag) {
		if (!flag_set.contains(flag)) {
			std::stringstream ss;
			ss << "Unknown flag: " << m_argv[m_argument_index] << ".";
			throw std::logic_error{ ss.str() };
		}
	}

	void checkMoreParametersAvailable() {
		if (m_argument_index == m_argc) {
			// Once here, the last flag has no value:
			std::stringstream ss;
			ss << "No value for the flag '" << m_argv[m_argument_index - 1] << ".";
			throw std::logic_error{ ss.str() };
		}
	}

	void processFlagPair() {
		string flag = m_argv[m_argument_index++];

		// First check that the currently indexed flag is correct:
		checkFlagIsValid(flag);

		// Check that there is more parameters in the command line:
		checkMoreParametersAvailable();

		
	}

	void processHelpFlags() {
		m_help_flag_present = true;
	}

	void processIterationFlags() {
		std::stringstream ss;
		ss << m_argv[m_argument_index++];
		ss >> m_iterations;
	}

	void processPriorityClassFlags() {
		auto pair = priority_class_name_map.find(m_argv[m_argument_index]);

		if (pair == priority_class_name_map.cend()) {
			std::stringstream ss;
			ss << m_argv[m_argument_index];
			ss >> m_priority_class;
		}
		else {
			m_priority_class = pair->second;
		}

		m_argument_index += 2;
	}

	void processThreadPriorityFlags() {
		auto pair = thread_priority_name_map.find(m_argv[m_argument_index]);

		if (pair == thread_priority_name_map.cend()) {
			std::stringstream ss;
			ss << m_argv[m_argument_index++];
			ss >> m_thread_priority;
		}
		else {
			m_thread_priority = pair->second;
		}
	}

public:
	CommandLineParser(int argc, char* argv[]) : m_argc{ argc }, m_argv{ argv } {
		loadDispatchMap();
		loadFlagSet();
		parseCommandLine();
		std::cout << "HELLO";
	}

	size_t getNumberOfIterations() {
		return m_iterations;
	}

	DWORD getPriorityClass() {
		return m_priority_class;
	}

	int getThreadPriority() {
		return m_thread_priority;
	}

	bool helpRequested() {
		return m_help_flag_present;
	}
};

#endif // COMMAND_LINE_PARSER_HPP