#pragma once
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "error.h"

#define ADD_TO_INCLUDE_PATH(path) ::preproc::includePath.push_back(path)

namespace preproc
{
	extern std::vector<std::filesystem::path> includePath;
	extern std::vector<std::filesystem::path> blockedIncludes;
	struct parsed_output
	{
		std::string output;
		std::vector<::error::error> errors;
		bool state;
		operator bool() { return state; }
		parsed_output()
		{
			output = "";
			errors.clear();
			state = true;
		}
		parsed_output(std::string output, std::vector<::error::error> errors, bool state)
		{
			this->output = output;
			this->errors = errors;
			this->state = state;
		}
	};
	struct macro
	{
		std::string name;
		std::string value;
	};
	static std::unordered_map<std::string, macro> macros;
	parsed_output parse(std::istringstream& input);
	std::string remove_trailing_whitespaces(std::string& str);
	bool isKeyword(const std::string& in);
	bool isMacro(const std::string& in);
	bool isPreprocCommand(const std::string& in);
}