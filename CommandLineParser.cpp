#include "CommandLineParser.h"

class CommandLineParser {
private:

	static const size_t DEFAULT_M_ITERATIONS = 10 * 1000 * 1000;
	static const DWORD  DEFAULT_M_PRIORITY_CLASS = NORMAL_PRIORITY_CLASS;
	static const int    DEFAULT_M_THREAD_PRIORITY = THREAD_PRIORITY_NORMAL;

	bool m_iteration_flag_present = false;
	bool m_priority_class_flag_present = false;
	bool m_thread_priority_flag_present = false;

	char** m_argv;
	int m_argc;
	int m_argument_index = 1; // ... = 1: Omit the program name from the command line.
	std::unordered_map<string, std::function<void(CommandLine&)>> m_flag_processor_map;

	size_t m_iterations = DEFAULT_M_ITERATIONS;
	DWORD m_priority_class = DEFAULT_M_PRIORITY_CLASS;
	int m_thread_priority = DEFAULT_M_THREAD_PRIORITY;

	void parseCommandLine() {
		while (m_argument_index < m_argc) {
			processFlagPair();
		}
	}

	void loadDispatchMap() {
		std::function<void(CommandLine&)> function_flag_iterations = &CommandLine::processIterationFlags;

		m_flag_processor_map[FLAG_LONG_NUMBER_OF_ITERATIONS] = function_flag_iterations;
		m_flag_processor_map[FLAG_SHORT_NUMBER_OF_ITERATIONS] = function_flag_iterations;
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
	CommandLine(int argc, char* argv[]) : m_argc{ argc }, m_argv{ argv } {
		loadDispatchMap();
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
};

#endif // COMMAND_LINE_PARSER_HPP