// LLVM

#include "lua.cpp"

// STDLIBC++
#include <iostream>
#include <string>
#include <vector>

int versionToInt(std::string version) {
	if (version == "ALPHA") return 1;
	return 0;
}
void run(std::vector<std::string> argv);

#ifndef VERSION
#define VERSION "ALPHA"
#endif
#define VERSIONI versionToInt(VERSION);
#define HELP                                           \
	"fltcc v" VERSION                                  \
	"\n"                                               \
	"fltcc [INPUT_FILES] [OPTIONS] -o [OUTPUT_FILE]\n" \
	"[OPTIONS]:\n"                                     \
	"    -h   print this help message\n"               \
	"    -v   print the fltcc version\n\n"             \
	"    -s   run in script mode\n"                    \
	"    -S   doesn't assemble\n"                      \
	"    -c   doesn't compile"

std::vector<std::string> arguments = {
	"-v",
	"-h",
	"-o",
	"-i",
	"-s",
	"-S",
	"-c",
};
#define ARGS_HELP argument == arguments.at(0)
#define ARGS_VERSION argument == arguments.at(1)
#define ARGS_OUTPUT argument == arguments.at(2)
#define ARGS_INPUT argument == arguments.at(3)
#define ARGS_SCRIPT argument == arguments.at(4)
#define ARGS_ASSEMBLE argument == arguments.at(5)
#define ARGS_COMPILE argument == arguments.at(6)

#define DEFAULT_FILE "fltcc.lua"

int main(int argc, char** argv) {
	std::vector<std::string> args(argv + 1, argv + argc);

	run(args);
	return 0;
}
void run(std::vector<std::string> argv) {
	std::string				 outputFile;
	std::vector<std::string> inputFiles;
	for (size_t i = 0; i < argv.size(); i++) {
		std::string argument = argv.at(i);

		if (ARGS_HELP) std::cout << "" VERSION "\n";
		else if (ARGS_VERSION) std::cout << HELP << "\n";
		else if (ARGS_OUTPUT) {
			outputFile = argv.at(i + 1);
			i++;
		} else if (ARGS_INPUT) {
			inputFiles.push_back(argv.at(i + 1));
			i++;
		}

		else if (ARGS_SCRIPT) {
			lua::setGlobalString("fltcc_VERSION", VERSION);

			lua::run(DEFAULT_FILE);
			lua::getfield("args");
			lua::integer len = lua::getlen();

			std::vector<std::string> args;
			for (int i = 0; i < len; i++) {
				lua::r_geti(i + 1);
				args.push_back(lua::tostring());
				lua::pop();
			}
			lua::pop();
			lua::getfield("conf");
			lua::getfield("min_ver");
			if (!lua::isstring()) {
				std::cout << "'min_ver' is not string\n";
				exit(1);
			}
			if (!versionToInt(lua::tostring())) {
				std::cout << "'min_ver' does not represent an available version\n"
						  << "(fltcc might be outdated)\n";
				exit(1);
			}

			lua::pop(2);

			lua::close();
			run(args);
		}

		else if (ARGS_ASSEMBLE) {
		} else if (ARGS_COMPILE) {
		}

		else {
			inputFiles.push_back(argument);
		}
	}
}
