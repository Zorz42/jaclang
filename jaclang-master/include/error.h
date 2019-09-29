#pragma once

#include <string>

namespace error
{
	void syntaxError(std::string error); // Print out syntax error

	void treeError(std::string error); // Print out syntax error

	void terminate(std::string reason, int exitCode); // Exit if error or something
}