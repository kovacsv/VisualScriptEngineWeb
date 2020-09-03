#ifndef PRECOMPLIED_HPP
#define PRECOMPILED_HPP

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

inline int swprintf_custom (wchar_t* const buffer, size_t const bufferCount, wchar_t const* const format, ...)
{
	va_list args;
	va_start (args, format);
	int result = vswprintf (buffer, bufferCount, format, args);
	va_end (args);
	return result;
}

#define swprintf swprintf_custom

#endif
