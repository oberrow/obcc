#pragma once
#include <stdlib.h>
#include <string.h>

#include <vector>

#include <Windows.h>

extern const char* filename;
extern const char* workingDirectory;

#define ITER_RANGE(obj) obj.begin(), obj.end()
#define ITER_PAIR(obj) obj.begin(), obj.end()
#define REVERSE(obj) std::reverse(ITER_RANGE(obj))
#define str_contains(str) find(str) != std::string::npos
#define str_ncontains(str) find(str) == std::string::npos

namespace error
{
	typedef unsigned char byte;
	extern const char* errorstring[8]; // also includes warning strings
	struct error
	{
		int line = 0;
		int character = 0;
		void* parameters = nullptr;
		int sizeParameters = 0;
		int parametersElementSize = 0;
		int countOfParameters;
		bool isWarning = false;
		bool isFatal = false;
		const char* filename;
		enum
		{
			PREPROC_ERROR_NONE,
			UNDEFINED,
			INVALID_PREPROC_DIRECTIVE,
			UNEXPECTED_TOKEN,
			FATAL_INCLUDE_NOT_FOUND,
			WARNING_MACRO_UNDEFINED,
			FATAL_USER_DEFINED_ERROR,
			INCLUDE_FILE_AMBIGUOUS
		};
		error() = default;
		error(int ec) :ec{ ec } {}
		error(int ec, int line) :ec{ ec }, line{ line } {}
		error(int ec, int line, int ch) :ec{ ec }, line{ line }, character{ ch } {}
		error(int ec, int line, int ch, const char* expr)
			:ec{ ec }, line{ line }, character{ ch }
		{
			sizeParameters = strlen(expr) + 1;
			parametersElementSize = sizeof(char);
			parameters = calloc(sizeParameters, sizeof(char));
			memcpy_s(parameters, strlen(expr), expr, strlen(expr));
		}
		error(int ec, int line, int ch, const char* expr, bool isWarning)
			:ec{ ec }, line{ line }, character{ ch }, isWarning{ isWarning }
		{
			sizeParameters = strlen(expr) + 1;
			parametersElementSize = sizeof(char);
			parameters = calloc(sizeParameters, sizeof(char));
			memcpy_s(parameters, strlen(expr), expr, strlen(expr));
		}
		error(int ec, int line, int ch, const char* expr, bool isWarning, bool isFatal)
			:ec{ ec }, line{ line }, character{ ch }, isWarning{ isWarning }, isFatal{ isFatal }
		{
			sizeParameters = strlen(expr) + 1;
			parametersElementSize = sizeof(char);
			parameters = calloc(sizeParameters, sizeof(char));
			memcpy_s(parameters, strlen(expr), expr, strlen(expr));
		}
		error(int ec, int line, int ch, const char* expr, bool isWarning, bool isFatal, const char* filename)
			:ec{ ec }, line{ line }, character{ ch }, isWarning{ isWarning }, isFatal{ isFatal }
		{
			sizeParameters = strlen(expr) + 1;
			parametersElementSize = sizeof(char);
			parameters = calloc(sizeParameters, sizeof(char));
			memcpy_s(parameters, strlen(expr), expr, strlen(expr));
			this->filename = (const char*)calloc(strlen(filename) + 1, sizeof(char));
			memcpy_s((char*)this->filename, strlen(filename), filename, strlen(filename));
			_ASSERT(filename != nullptr);
		}
		error(const error& other)
		{
			this->ec = other.ec;
			this->line = other.line;
			if (other.sizeParameters != 0)
			{
				this->parameters = calloc(other.sizeParameters, other.parametersElementSize);
				memcpy_s(this->parameters, other.sizeParameters, other.parameters, other.sizeParameters);
			}
			this->filename = (const char*)calloc(strlen(other.filename) + 1, sizeof(char));
			memcpy_s((char*)this->filename, strlen(other.filename), other.filename, strlen(other.filename));
			this->character = other.character;
			this->parametersElementSize = other.parametersElementSize;
			this->sizeParameters = other.sizeParameters;
			this->isWarning = other.isWarning;
			this->isFatal = other.isFatal;
		}
		error(error&& other) noexcept
		{
			this->ec = other.ec;
			this->line = other.line;
			this->parameters = other.parameters;
			this->character = other.character;
			this->parametersElementSize = other.parametersElementSize;
			this->sizeParameters = other.sizeParameters;
			this->ref = ++other.ref;
			this->isWarning = other.isWarning;
			this->isFatal = other.isFatal;
			this->filename = other.filename;
		}
		~error()
		{
			// if there is only one reference, then disinitalize everything
			if (ref == 1) 
			{
				free(parameters);
				free((char*)filename);
			}
			// either way decrement the reference count
			ref--;
		}
		error& operator=(const error&) = default;
		int operator=(int _Right)
		{
			return ec = _Right;
		}
		bool operator==(int _Right)
		{
			return ec == _Right;
		}
		int operator-(int _Right)
		{
			return ec - _Right;
		}
		int operator+(int _Right)
		{
			return ec + _Right;
		}
		int operator*(int _Right)
		{
			return ec * _Right;
		}
		int operator/(int _Right)
		{
			return ec / _Right;
		}
		int operator-=(int _Right)
		{
			return ec -= _Right;
		}
		int operator+=(int _Right)
		{
			return ec += _Right;
		}
		int operator*=(int _Right)
		{
			return ec *= _Right;
		}
		int operator/=(int _Right)
		{
			return ec /= _Right;
		}
		int operator--()
		{
			return ec -= 1;
		}
		int operator++()
		{
			return ec += 1;
		}
		operator int()
		{
			return ec;
		}
	private:
		int ref = 1;
		int ec = 0;
	};
	void printErrors(const std::vector<error>& errors);
	const char* strerror(const error& ec);
	void emit_error(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location);
	void emit_warning(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location);
	void emit_fatal_error(std::vector<error>& errors, int ec, int line, int character, std::vector<byte> param, const char* file_location);
	void re_emit(std::vector<error>& errors, error ec);
	enum colors																						 
	{
		black,
		blue,
		green,
		cyan,
		red,
		magenta,
		brown,
		lightgray,
		darkgray,
		lightblue,
		lightgreen,
		lightcyan,
		lightred,
		lightmagenta,
		yellow,
		white
	};
	// Sets the text color of the console for the current scope amount of time
	struct colorAttributes {
		colorAttributes(WORD attrib) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attrib);
		}
		~colorAttributes() {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	};
}