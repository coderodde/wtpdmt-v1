#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <windows.h>

using std::string;

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

static std::size_t size_t_max(std::size_t a, std::size_t b) {
	return a < b ? b : a;
}

// Program flags:
static string FLAG_LONG_NUMBER_OF_ITERATIONS  = "--iterations";
static string FLAG_SHORT_NUMBER_OF_ITERATIONS = "-i";
static string FLAG_LONG_PRIORITY_CLASS        = "--priority-class";
static string FLAG_SHORT_PRIORITY_CLASS       = "-P";
static string FLAG_LONG_THREAD_PRIORITY       = "--thread-priority";
static string FLAG_SHORT_THREAD_PRIORITY      = "-t";

static std::unordered_set<string> flag_set;

static void LoadProgramFlags() {
	flag_set.emplace(FLAG_LONG_NUMBER_OF_ITERATIONS);
	flag_set.emplace(FLAG_SHORT_NUMBER_OF_ITERATIONS);
	flag_set.emplace(FLAG_LONG_PRIORITY_CLASS);
	flag_set.emplace(FLAG_SHORT_PRIORITY_CLASS);
	flag_set.emplace(FLAG_LONG_THREAD_PRIORITY);
	flag_set.emplace(FLAG_SHORT_THREAD_PRIORITY);
}

// static std::unordered_map<std::function<void, const 

// Priority class names:
static string ABOVE_NORMAL_PRIORITY_CLASS_STR   = "ABOVE_NORMAL_PRIORITY_CLASS";   // 0x00008000
static string BELOW_NORMAL_PRIORITY_CLASS_STR   = "BELOW_NORMAL_PRIORITY_CLASS";   // 0x00004000
static string HIGH_PRIORITY_CLASS_STR           = "HIGH_PRIORITY_CLASS";           // 0x00000080
static string IDLE_PRIORITY_CLASS_STR           = "IDLE_PRIORITY_CLASS";           // 0x00000040
static string NORMAL_PRIORITY_CLASS_STR         = "NORMAL_PRIORITY_CLASS";         // 0x00000020
static string PROCESS_MODE_BACKGROUND_BEGIN_STR = "PROCESS_MODE_BACKGROUND_BEGIN"; // 0x00100000
static string PROCESS_MODE_BACKGROUND_END_STR   = "PROCESS_MODE_BACKGROUND_END";   // 0x00200000
static string REALTIME_PRIORITY_CLASS_STR       = "REALTIME_PRIORITY_CLASS";       // 0x00000100

static std::map<string, DWORD> priority_class_name_map;
static std::unordered_set<DWORD> class_number_set;

static void LoadPriorityClassNames() {
	priority_class_name_map[ABOVE_NORMAL_PRIORITY_CLASS_STR]   = ABOVE_NORMAL_PRIORITY_CLASS;
	priority_class_name_map[BELOW_NORMAL_PRIORITY_CLASS_STR]   = BELOW_NORMAL_PRIORITY_CLASS;
	priority_class_name_map[HIGH_PRIORITY_CLASS_STR]           = HIGH_PRIORITY_CLASS;
	priority_class_name_map[IDLE_PRIORITY_CLASS_STR]           = IDLE_PRIORITY_CLASS;
	priority_class_name_map[NORMAL_PRIORITY_CLASS_STR]         = NORMAL_PRIORITY_CLASS;
	priority_class_name_map[PROCESS_MODE_BACKGROUND_BEGIN_STR] = PROCESS_MODE_BACKGROUND_BEGIN;
	priority_class_name_map[PROCESS_MODE_BACKGROUND_END_STR]   = PROCESS_MODE_BACKGROUND_END;
	priority_class_name_map[REALTIME_PRIORITY_CLASS_STR]       = REALTIME_PRIORITY_CLASS;
}

static void LoadPriorityClassNumberSet() {
	class_number_set.emplace(0x8000);
	class_number_set.emplace(0x4000);
	class_number_set.emplace(0x80);
	class_number_set.emplace(0x40);
	class_number_set.emplace(0x20);
	class_number_set.emplace(0x00100000);
	class_number_set.emplace(0x00200000);
	class_number_set.emplace(0x100);
}

// Thread priority names:
static string THREAD_MODE_BACKGROUND_BEGIN_STR  = "THREAD_MODE_BACKGROUND_BEGIN";  // 0x0010000
static string THREAD_MODE_BACKGROUND_END_STR    = "THREAD_MODE_BACKGROUND_END";    // 0x00020000
static string THREAD_PRIORITY_ABOVE_NORMAL_STR  = "THREAD_PRIORITY_ABOVE_NORMAL";  // 1
static string THREAD_PRIORITY_BELOW_NORMAL_STR  = "THREAD_PRIORITY_BELOW_NORMAL";  // -1
static string THREAD_PRIORITY_HIGHEST_STR       = "THREAD_PRIORITY_HIGHEST";       // 2
static string THREAD_PRIORITY_IDLE_STR          = "THREAD_PRIORITY_IDLE";          // -15
static string THREAD_PRIORITY_LOWEST_STR        = "THREAD_PRIORITY_LOWEST";        // -2
static string THREAD_PRIORITY_NORMAL_STR        = "THREAD_PRIORITY_NORMAL";        // 0
static string THREAD_PRIORITY_TIME_CRITICAL_STR = "THREAD_PRIORITY_TIME_CRITICAL"; // 15

static std::map<string, int> thread_priority_name_map;
static std::unordered_set<int> thread_priority_number_set;

static void LoadTheadPriorityNames() {
	thread_priority_name_map[THREAD_MODE_BACKGROUND_BEGIN_STR]  = THREAD_MODE_BACKGROUND_BEGIN;
	thread_priority_name_map[THREAD_MODE_BACKGROUND_END_STR]    = THREAD_MODE_BACKGROUND_END;
	thread_priority_name_map[THREAD_PRIORITY_ABOVE_NORMAL_STR]  = THREAD_PRIORITY_ABOVE_NORMAL;
	thread_priority_name_map[THREAD_PRIORITY_BELOW_NORMAL_STR]  = THREAD_PRIORITY_BELOW_NORMAL;
	thread_priority_name_map[THREAD_PRIORITY_HIGHEST_STR]       = THREAD_PRIORITY_HIGHEST;
	thread_priority_name_map[THREAD_PRIORITY_IDLE_STR]          = THREAD_PRIORITY_IDLE;
	thread_priority_name_map[THREAD_PRIORITY_LOWEST_STR]        = THREAD_PRIORITY_LOWEST;
	thread_priority_name_map[THREAD_PRIORITY_NORMAL_STR]        = THREAD_PRIORITY_NORMAL;
	thread_priority_name_map[THREAD_PRIORITY_TIME_CRITICAL_STR] = THREAD_PRIORITY_TIME_CRITICAL;
}

static void LoadThreadPriorityNumberSet() {
	thread_priority_number_set.emplace(0x00100000);
	thread_priority_number_set.emplace(0x00020000);
	thread_priority_number_set.emplace(1);
	thread_priority_number_set.emplace(-1);
	thread_priority_number_set.emplace(2);
	thread_priority_number_set.emplace(-15);
	thread_priority_number_set.emplace(-2);
	thread_priority_number_set.emplace(0);
	thread_priority_number_set.emplace(15);
}

static DWORD GetPriorityClassFromString(string& priorityClassName) {
	auto p = priority_class_name_map.find(priorityClassName);

	if (p == priority_class_name_map.cend()) {
		std::stringstream ss;
		ss << "Unrecognized priority class: "
		   << priorityClassName
		   << "\n";

		throw std::logic_error{ ss.str() };
	} else {
		return p->second;
	}
}

static int GetThreadPriorityFromString(string& threadPriorityName) {
	auto p = thread_priority_name_map.find(threadPriorityName);

	if (p == thread_priority_name_map.cend()) {
		std::stringstream ss;
		ss << "Unrecognized thread priority: "
			<< threadPriorityName
			<< "\n";

		throw std::logic_error{ ss.str() };
	} else {
		return p->second;
	}
}

template<class Value>
static size_t GetMapStringKeyMaximumLength(std::map<string, Value>& map) {
	size_t maximum_string_length = 0;

	for (auto& i : map) {
		maximum_string_length = size_t_max(maximum_string_length, 
										   i->first.length());
	}

	return maximum_string_length;
}

void PrintHelp(string& programName) {
	size_t maximum_class_name_length = GetMapStringKeyMaximumLength(priority_class_name_map);
	size_t maximum_thread_priority_name_length = GetMapStringKeyMaximumLength(thread_priority_name_map);

	std::cout << programName
		      << " [--class = <CLASS>] [--thread = <THREAD>]\n"
		      << "where:\n"
		      << "  <CLASS> is one of: \n";

	std::cout << std::hex;	
	std::cout << std::setw(maximum_class_name_length);

	for (auto& i : priority_class_name_map) {
		std::cout << i.first
				  << " Sets the process class to "
				  << i.second 
				  << "\n";
	}

	std::cout << "  <THREAD> is one of:\n";
	std::cout << std::setw(maximum_thread_priority_name_length);

	for (auto& i : thread_priority_name_map) {
		std::cout << i.first
	      		  << " Sets the thread priority to "
				  << i.second
				  << "\n";
	}
}

class CommandLine {
private:

	static const size_t DEFAULT_M_ITERATIONS = 10 * 1000 * 1000;
	static const DWORD DEFAULT_M_PRIORITY_CLASS = NORMAL_PRIORITY_CLASS;
	static const int DEFAULT_M_THREAD_PRIORITY = THREAD_PRIORITY_NORMAL;

	bool m_iteration_flag_present = false;
	bool m_priority_class_flag_present = false;
	bool m_thread_priority_flag_present = false;

	char** arguments;
	int tokens_omitted = 1; // Omit the program name from the command line.
	std::unordered_map<string, std::function<void(const CommandLine&, int, int)>> m_flag_processor_map;

	size_t m_iterations = DEFAULT_M_ITERATIONS;
	DWORD m_priority_class = DEFAULT_M_PRIORITY_CLASS;
	int m_thread_priority = DEFAULT_M_THREAD_PRIORITY;

	void parseCommandLine(int argc, char* argv[]) {
		arguments = argv;

		while (tokens_omitted < argc) {
			processFlagPair(tokens_omitted, argc);
		}
	}

	void loadDispatchMap() {
		std::function<void(const CommandLine&)> function_flag_iterations = &CommandLine::processIterationFlags;

		m_flag_processor_map[FLAG_LONG_NUMBER_OF_ITERATIONS] = function_flag_iterations;
		m_flag_processor_map[FLAG_SHORT_NUMBER_OF_ITERATIONS] = function_flag_iterations;
	}

	void processFlagPair(int tokens_ommited, int argc) {
		
		string flag_value = arguments[tokens_omitted];

		// First check that the currently indexed flag is correct:
		if (!flag_set.contains(flag_value)) {
			std::stringstream ss;
			ss << "Unknown flag: " << arguments[tokens_ommited];
			throw std::logic_error{ ss.str() };
		}

		tokens_ommited++; // Omit the flag.

		if (tokens_ommited == argc - 1) {
			// Once here, the last flag has no value:
			std::stringstream ss;
			ss << "No value for the flag '" << flag_value << "'.\n"
			throw std::logic_error{ ss.str() }
		}

		
	}

	void processIterationFlags() {
		std::stringstream ss;
		ss << arguments[tokens_omitted++];
		ss >> m_iterations;
	}

	void processPriorityClassFlags() {
		auto& pair = priority_class_name_map.find(arguments[tokens_omitted]);

		if (pair == priority_class_name_map.cend()) {
			std::stringstream ss;
			ss << arguments[tokens_omitted++];
			ss >> m_priority_class;
		} else {
			m_priority_class = pair->second;
		}
	}

	void processThreadPriorityFlags() {
		auto& pair = thread_priority_name_map.find(arguments[tokens_omitted]);

		if (pair == thread_priority_name_map.cend()) {
			std::stringstream ss;
			ss << arguments[tokens_omitted++];
			ss >> m_thread_priority;
		} else {
			m_thread_priority = pair->second;
		}
	}

public:
	CommandLine(int argc, char* argv[]) {
		loadDispatchMap();
		parseCommandLine(argc, argv);
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

int main(int argc, char* argv[]) {
	LoadPriorityClassNames();
	LoadPriorityClassNumberSet();

	LoadTheadPriorityNames();
	LoadThreadPriorityNumberSet();

    string program_name = argv[0];
	PrintHelp(program_name);

	ULONGLONG ta = GetTickCount64();
	ULONGLONG maximumDuration = 0;

	for (int i = 0; i < 10 * 1000; i++) {
		ULONGLONG tb = GetTickCount64();
		ULONGLONG duration = tb - ta;
		maximumDuration = MAX(maximumDuration, duration);
	}

	printf("%llu\n", maximumDuration);
	return 0;
}