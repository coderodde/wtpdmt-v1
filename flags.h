#ifndef FLAGS_H
#define FLAGS_H

#include <string>

using std::string;

// Program flags:
static string FLAG_LONG_NUMBER_OF_ITERATIONS = "--iterations";
static string FLAG_SHORT_NUMBER_OF_ITERATIONS = "-i";
static string FLAG_LONG_PRIORITY_CLASS = "--priority-class";
static string FLAG_SHORT_PRIORITY_CLASS = "-p";
static string FLAG_LONG_THREAD_PRIORITY = "--thread-priority";
static string FLAG_SHORT_THREAD_PRIORITY = "-t";
static string FLAG_LONG_HELP = "--help";
static string FLAG_SHORT_HELP = "-h";

#endif // FLAGS_h