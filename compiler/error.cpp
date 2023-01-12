#include "error.h"

#include <fstream>
#include <string>
#include <vector>

#include <intrin.h>

namespace error
{
	const char* errorstring[8] =
	{
	  "No Error.",
	  "In file %s at line: %d,%d: Error C0001: %s is undefined.",
	  "In file %s at line: %d,%d: Error C0002: #%s: Unrecognized preprocessing directive.",
	  "In file %s at line: %d,%d: Warning C0003: Unexpected token following preprocessor directive: %s.",
	  "In file %s at line: %d,%d: Fatal Error C0004: Cannot open include file: %s",
	  "In file %s at line: %d,%d: Warning C0005: The macro %s is undefined.",
	  "In file %s at line: %d,%d: Fatal Error C0006: User defined error: #error %s",
	  "In file %s at line: %d,%d: Fatal Error C0007: %s is ambiguous"
	};
	void printErrors(const std::vector<::error::error>& errors)
	{
		std::ifstream file{};
		for (auto ec : errors)
		{
			file.open(ec.filename);
			if (!file) continue;
			int line = 0;
			std::string lnstr;
			while (std::getline(file, lnstr) && (line + 1) != ec.line) line++;
			colors color;
			if (ec.isFatal)
				color = colors::red;
			else if (ec.isWarning)
				color = colors::yellow;
			else
				color = colors::lightred;
			colorAttributes colorAtrib{ (WORD)color };
			printf(strerror(ec), ec.filename, ec.line, ec.character, ec.parameters ? (char*)ec.parameters : "");
			printf("\n");
			std::string location;
			int i;
			for (i = 0; i < ec.character; i++) location.push_back('~');
			location.push_back('^');
			for (; i < lnstr.length() - 1; i++) location.push_back('~');
			printf("%s\n%s\n", lnstr.c_str(), location.c_str());
			file.close();
		}
	}
	const char* strerror(const error& ec)
	{
		error cpy = ec;
		return errorstring[cpy];
	}
	void emit_error(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location)
	{
		error temp{ ec, line, character, (char*)param.data(), false, false, file_location };
		temp.countOfParameters = std::count(ITER_PAIR(param), '\0');
		errors.push_back(temp);
#ifdef _DEBUG
		__debugbreak();
#endif
	}
	void emit_warning(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location)
	{
		error temp{ ec, line, character, (char*)param.data(), true, false, file_location };
		temp.countOfParameters = std::count(ITER_PAIR(param), '\0');
		errors.push_back(temp);
#ifdef _DEBUG
		__debugbreak();
#endif
	}
	void emit_fatal_error(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location)
	{
		error temp{ ec, line, character, (char*)param.data(), false, true, file_location };
		temp.countOfParameters = std::count(ITER_PAIR(param), '\0');
		errors.push_back(temp);
#ifdef _DEBUG
		__debugbreak();
#endif
	}
	void re_emit(std::vector<error>& errors, error ec)
	{
		errors.push_back(ec);
	}
}