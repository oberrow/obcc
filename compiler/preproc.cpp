#include "preproc.h"

#include <numeric>
#include <algorithm>
#include <thread>
#include <fstream>

#include <intrin.h>

// Preprocesser Statements in c:
// - #define
// - #undef
// - #include
// #ifdef
// #ifndef
// #if
// #else
// #elif
// - #endif
// - #error
// #pragma 
// I won't do pragma until I have the COMPILER done
// Although #pragma once will be implemented

namespace preproc
{
	enum class linetype
	{
		REGULAR,
		COMMENT,
		COMMENT_LINE,
		PRE_PROC
	};
	std::vector<std::filesystem::path> includePath;
	std::vector<std::filesystem::path> blockedIncludes;
	parsed_output parse(std::istringstream& input)
	{	
		__nop();
		parsed_output* ret = new parsed_output();
		std::string workingLine;
		std::string workingOn;
		linetype workingLineType = linetype::REGULAR;
		std::string firstTwoChars;
		std::stringstream out;
		int line = 0;
		bool isCommented = false;
		bool isInConditionalFalse = false;
		while (std::getline(input, workingOn))
		{
			line++;
			workingLine = workingOn;
			remove_trailing_whitespaces(workingLine);
			// we're in a multiline comment
			if (isCommented)
			{
				std::string twoChars;
				size_t posMultilineCommentStart = 0;
				size_t posMultilineCommentEnd = 0;
				size_t index = 0;
				commentTop1:
				for (auto& i : workingLine)
				{
					if (twoChars.length() == 3)
						twoChars = "";
					twoChars.push_back(i);
					if (twoChars == "*/")
					{
						isCommented = false;
						posMultilineCommentEnd = index;
						workingLine.erase(posMultilineCommentEnd);
						goto commentTop1;
					}
					else if (workingLine.str_ncontains("*/"))
						workingLine = "";
					index++;
				}
			}
			if (workingLine.empty()) continue;
			firstTwoChars = workingLine.substr(0, 2);
			if (workingLine.front() == '#')
				workingLineType = linetype::PRE_PROC;
			else if (firstTwoChars == "//")
				workingLineType = linetype::COMMENT_LINE;
			else if(workingLine.str_contains("//"))
				workingLineType = linetype::COMMENT;
			else
				workingLineType = linetype::REGULAR;
			switch (workingLineType)
			{
			case preproc::linetype::REGULAR:
			{
				std::string twoChars;
				size_t posMultilineCommentStart = 0;
				size_t posMultilineCommentEnd   = 0;
				size_t index = 0;
				commentTop2:
				for (auto& i : workingLine)
				{
					if (twoChars.length() == 3)
						twoChars = "";
					twoChars.push_back(i);
					if (twoChars == "/*")
					{
						isCommented = true;
						posMultilineCommentStart = index;
					}
					else if (twoChars == "*/")
					{
						isCommented = false;
						posMultilineCommentEnd = index;
						workingLine.erase(posMultilineCommentStart, workingLine.length() - posMultilineCommentEnd);
						goto commentTop2;
					}
					else if (workingLine.str_contains("/*") && workingLine.str_ncontains("*/"))
						workingLine = "";
					index++;
				}
				if (workingLine.empty())
					continue;
				typedef struct
				{
					size_t size;
					std::string name;
					std::string value;
				} macroData;
				std::vector<macroData> macrosNames;
				for (const auto& [name, value] : macros) macrosNames.push_back(macroData{ .size = name.length(), .name = name, .value = value.value });
				struct {
					bool operator()(macroData one, macroData two) const { return one.size > two.size; }
				} greater;
				std::sort(ITER_PAIR(macrosNames), greater);
				for (const auto& i : macrosNames)
				{
					const auto& name = i.name;
					const auto& value = i.value;
					while (workingLine.str_contains(name)) // continue finding the macro on the current line until all instances are replaced with value
					{
						size_t instance = workingLine.find(name);
						std::string half1 = workingLine.substr(0, instance);
						std::string half2 = workingLine.substr(instance);
						half2.erase(0, name.length());
						half1.append(value);
						half1.append(half2);
						workingLine = half1;
					}
				}
				break;
			}
			case preproc::linetype::COMMENT_LINE:
			{
				workingLine = ""; // delete the line
				break;
			}
			case preproc::linetype::COMMENT:
			{
				int i = 0;
				std::string current = "";
				for (auto& c : workingLine)
				{
					current.push_back(c);
					if (current.find("//") != std::string::npos)
					{
						workingLine.erase(i - 1, std::string::npos);
						break;
					}
					i++;
				}
				typedef struct
				{
					size_t size;
					std::string name;
					std::string value;
				} macroData;
				std::vector<macroData> macrosNames;
				for (const auto& [name, value] : macros) macrosNames.push_back(macroData{ .size = name.length(), .name = name, .value = value.value });
				struct {
					bool operator()(macroData one, macroData two) const { return one.size > two.size; }
				} greater;
				std::sort(ITER_PAIR(macrosNames), greater);
				for (const auto& i : macrosNames)
				{
					const auto& name = i.name;
					const auto& value = i.value;
					while (workingLine.str_contains(name)) // continue finding the macro on the current line until all instances are replaced with value
					{
						size_t instance = workingLine.find(name);
						std::string half1 = workingLine.substr(0, instance);
						std::string half2 = workingLine.substr(instance);
						half2.erase(0, name.length());
						half1.append(value);
						half1.append(half2);
						workingLine = half1;
					}
				}
				break;
			}
			case preproc::linetype::PRE_PROC:
			{
				std::string command = workingLine.substr(workingLine.find('#') + 1, workingLine.find(' ') - 1);
				std::string arguments = workingLine.substr(workingLine.find(' ') + 1);
				bool isValid = isPreprocCommand(command);
				if (!isValid)
				{
					std::vector<error::byte> e{};
					std::string str(command);
					e.assign(ITER_PAIR(str));
					e.push_back('\0');
					error::emit_error(ret->errors, error::error::INVALID_PREPROC_DIRECTIVE, line, (int)workingLine.find('#') + 1, e, filename); // push error
				}
				if (command == "define")
				{
					macro m;
					m.name = arguments.substr(0, arguments.find(' '));
					if (arguments.find(' ') != std::string::npos && !((workingLine.find(' ') + 1) > arguments.length()))
						m.value = arguments.substr(arguments.find(' ') + 1);
					macros.try_emplace(m.name, m);
				}
				else if (command == "undef")
				{
					if (arguments.str_contains(' '))
					{
						std::vector<error::byte> warn{};
						std::string str(arguments.substr(arguments.find(' ') + 1));
						warn.assign(ITER_PAIR(str));
						warn.push_back('\0');
						error::emit_warning(ret->errors, error::error::UNEXPECTED_TOKEN, line, (int)workingLine.find(' ', workingLine.find(' ') + 1) + 1, warn, filename);
					}
					macros.erase(arguments.substr(0, arguments.find(' ')));
				}
				else if (command == "include")
				{
					if (arguments.str_contains('\"'))
					{
						// look in working directory and ignore the include path
						std::string includeFilename = arguments;
						std::erase(includeFilename, '\"');
						std::ifstream include{ std::string(workingDirectory) + includeFilename };
						if (!include)
						{
							std::vector<error::byte> e{};
							std::string str(arguments.substr(arguments.find(' ') + 1));
							e.assign(ITER_PAIR(str));
							e.push_back('\0');
							error::emit_fatal_error(ret->errors, error::error::FATAL_INCLUDE_NOT_FOUND, line, (int)workingLine.find(' ', workingLine.find(' ') + 1) + 1, e, filename);
						}
						std::string includeFilecontents;
						{
							// https://stackoverflow.com/a/2602258
							std::stringstream buffer;
							buffer << include.rdbuf();
							includeFilecontents = buffer.str();
						}
						if (includeFilecontents.str_contains("#include"))
						{
							parsed_output includeFileParsedOutput;
							std::thread includeParser
							{
								[&]()
								{
									std::istringstream iss{ includeFilecontents };
									includeFileParsedOutput = parse(iss);
									out.str(includeFileParsedOutput.output + out.str());
								}
							};
							includeParser.join();
							out.str(includeFileParsedOutput.output + out.str());
						}
						else
						{
							std::istringstream iss{ includeFilecontents };
							parsed_output includeFileParsedOutput;
							includeFileParsedOutput = parse(iss);
							out.str(includeFileParsedOutput.output + out.str());
							for(auto& ec : includeFileParsedOutput.errors)
							{
								ec.filename = includeFilename.c_str();
								error::re_emit(ret->errors, ec);
							}
						}
					}
					else if (arguments.str_contains('<'))
					{
						// look in include path and ignore working directory
						std::string includeFilename = arguments;
						std::erase(includeFilename, '<');
						std::erase(includeFilename, '>');
						std::filesystem::path includeFilepath;
						std::ifstream include{};
						bool found = false;
						for (const auto& path : includePath)
						{
							std::filesystem::path temp = path.string() + "/" + includeFilename;
							if (std::filesystem::exists(temp))
							{
								if (found)
								{
									std::vector<error::byte> e{};
									std::string str(includeFilename + " (Note: First instance found at: " + includeFilepath.string() + ", second instance found at: " + temp.string() + ")");
									e.assign(ITER_PAIR(str));
									e.push_back('\0');
									error::emit_fatal_error(ret->errors, error::error::INCLUDE_FILE_AMBIGUOUS, line, workingLine.find('<') + 1, e, filename);
									break;
								}
								includeFilepath = temp;
								found = true;
							}
						}
						if (!found)
						{
							std::vector<error::byte> e{};
							std::string str(arguments.substr(arguments.find(' ') + 1));
							e.assign(ITER_PAIR(arguments));
							e.push_back('\0');
							error::emit_warning(ret->errors, error::error::FATAL_INCLUDE_NOT_FOUND, line, (int)workingLine.find('#'), e, filename);
						}
						bool includedOnce = false;
						for (const auto& blockedInclude : blockedIncludes)
						{
							if (blockedInclude == includeFilepath)
								includedOnce = true;
						}
						if (includedOnce)
							continue;
						include.open(includeFilepath);
						std::string includeFilecontents;
						{
							// https://stackoverflow.com/a/2602258
							std::stringstream buffer;
							buffer << include.rdbuf();
							includeFilecontents = buffer.str();
						}
						if (includeFilecontents.str_contains("#include"))
						{
							parsed_output includeFileParsedOutput;
							std::thread includeParser
							{
								[&]()
								{
									std::istringstream iss{ includeFilecontents };
									includeFileParsedOutput = parse(iss);
									out.str(includeFileParsedOutput.output + out.str());
							}
							};
							includeParser.join();
							out.str(includeFileParsedOutput.output + out.str());
						}
						else
						{
							std::istringstream iss{ includeFilecontents };
							parsed_output includeFileParsedOutput;
							includeFileParsedOutput = parse(iss);
							out.str(includeFileParsedOutput.output + out.str());
							for (auto& ec : includeFileParsedOutput.errors)
							{
								ec.filename = includeFilename.c_str();
								error::re_emit(ret->errors, ec);
							}
						}
					}
				}
				else if (command == "if")
				{
					std::string currentExpression;
					std::istringstream currentExpressionSS{ arguments };
					while (currentExpressionSS >> currentExpression)
					{
						std::string& currentMacro = currentExpression;
						if (!macros.contains(currentMacro))
						{
							std::vector<error::byte> e{};
							std::string str(currentExpression);
							e.assign(ITER_PAIR(str));
							e.push_back('\0');
							error::emit_warning(ret->errors, error::error::WARNING_MACRO_UNDEFINED, line, (int)workingLine.find(' ', workingLine.find(' ') + 1) + 1, e, filename);
						}
						const auto& [key, value] = macros.at(currentMacro);
					}
				}
				else if (command == "error")
				{
					std::vector<error::byte> e{};
					std::string str(arguments);
					e.assign(ITER_PAIR(arguments));
					e.push_back('\0');
					error::emit_fatal_error(ret->errors, error::error::FATAL_USER_DEFINED_ERROR, line, workingLine.find('#') + 1, e, filename);
				}
				else if (command == "pragma")
				{
					if (arguments.str_contains("once"))
					{
						if (arguments.str_contains(' '))
						{
							std::vector<error::byte> e{};
							std::string str(arguments);
							e.assign(ITER_PAIR(arguments));
							e.push_back('\0');
							error::emit_warning(ret->errors, error::error::UNEXPECTED_TOKEN, line, (int)workingLine.find(' ') + 1, e, filename);
						}
						blockedIncludes.push_back(filename); // block current file from being included more times
					}
				}
				if(isValid) workingLine = ""; // when a preprocesser command is processed remove it so it doesn't cause errors later on...
				break;
			}
			default:
				break;
			}
			out << workingLine << '\n';
		}
		if(ret->state)
			ret->output = out.str();
		auto [output, errors, state] = *ret;
		delete ret;
		return { output, errors, state };
	}
	std::string remove_trailing_whitespaces(std::string& str)
	{
		size_t pos = 0;
		int i;
		for (i = 0; i < str.size(); i++)
		{
			if (str[i] != ' ' && str[i] != '\t' && str[i] != '\r')
				break;
		}
		if (i == 0 || i == str.length()) return str;
		pos = i;
		str.erase(0, pos);
		return str;
	}
	bool isKeyword(const std::string& in)
	{
		const char* keywords[33] =
		{
		   "auto"  ,   "break",  "case"    , "char"  , "const"   , "continue", "default" , "do"    ,
		   "double",   "else" ,  "enum"    , "extern", "float"   , "for"     , "goto"    , "if"    ,
		   "int"   ,   "long" ,  "register", "return", "short"   , "signed"  , "sizeof"  , "static",
		   "struct",  "switch",  "typedef" , "union" , "unsigned", "void"    , "volatile", "while" ,
		   "__attributes(thread_local)"
		};
		for (int i = 0; i < sizeof(keywords); i++)
			if (in == keywords[i]) 
				return true;
		return false;
	}
	bool isMacro(const std::string& in)
	{
		for (const auto&[name, value] : macros)
			if (in == name)
				return true;
		return false;
	}
	bool isPreprocCommand(const std::string& in)
	{
		const char* commands[11] =
		{
			"define",
			"undef",
			"include",
			"ifdef",
			"ifndef",
			"if",
			"else",
			"elif",
			"endif",
			"error",
			"pragma"
		};
		for (int i = 0; i < 11; i++)
		{
			bool equal = in.length() == strlen(commands[i]);
			if (equal) equal = equal && in.str_contains(commands[i]);
			if (equal)
				return true;
		}
		return false;
	}
	std::string remove_trailing_tabs(std::string& str)
	{
		size_t pos = str.find_first_not_of('\t');
		str.erase(0, pos);
		return str;
	}
}