#include "flags.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <windows.h>

using std::string;

static std::unordered_set<string> flag_set;

static void LoadProgramFlags() {
	flag_set.emplace(FLAG_LONG_NUMBER_OF_ITERATIONS);
	flag_set.emplace(FLAG_SHORT_NUMBER_OF_ITERATIONS);
	flag_set.emplace(FLAG_LONG_PRIORITY_CLASS);
	flag_set.emplace(FLAG_SHORT_PRIORITY_CLASS);
	flag_set.emplace(FLAG_LONG_THREAD_PRIORITY);
	flag_set.emplace(FLAG_SHORT_THREAD_PRIORITY);
}

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
	.emplace(0x8000);
	class_number_set.emplace(0x4000);
	class_number_set.emplace(0x80);
	class_number_set.emplace(0x40);
	class_number_set.emplace(0x20);
	class_number_set.emplace(0x00100000);
	class_number_set.emplace(0x00200000);
	class_number_set.emplace(0x100);
}

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

	for (const auto& i : map) {
		maximum_string_length = max(maximum_string_length, i.first.length());
	}

	return maximum_string_length;
}

void PrintHelp(string& programNameArgument) {
	
}

int main(int argc, char* argv[]) {
	CommandLineParser(argc, argv);

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
		maximumDuration = max(maximumDuration, duration);
	}

	printf("%llu\n", maximumDuration);
	return 0;
}