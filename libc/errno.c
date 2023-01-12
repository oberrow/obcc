#include "pch.h"
#include "framework.h"

__declspec(thread) int realerrno;
int* _get_errno_address()
{
	return &realerrno;
}