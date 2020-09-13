#ifndef PRECOMPLIED_HPP
#define PRECOMPILED_HPP

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

inline int swprintf_hack_only_for_d_and_ls (wchar_t* const buffer, size_t const bufferCount, wchar_t const* const format, ...)
{
	va_list args;
	va_start (args, format);

	int wasError = 0;
	size_t formatLength = wcslen (format);
	size_t bufferIndex = 0;
	for (size_t formatIndex = 0; formatIndex < formatLength; formatIndex++) {
		wchar_t formatChar = format[formatIndex];
		if (formatChar == L'%') {
			wchar_t* bufferToWrite = NULL;
			static wchar_t tempBuffer[128];

			if (formatIndex < formatLength - 1 && format[formatIndex + 1] == L'd') {
				bufferToWrite = tempBuffer;
				swprintf (bufferToWrite, 128, L"%d", va_arg (args, int));
				formatIndex += 1;
			} else if (formatIndex < formatLength - 2 && format[formatIndex + 1] == L'l' && format[formatIndex + 2] == L's') {
				bufferToWrite = va_arg (args, wchar_t*);
				formatIndex += 2;
			} 

			if (bufferToWrite == NULL) {
				wasError = 1;
				break;
			}

			size_t bufferLengthToWrite = wcslen (bufferToWrite);
			if (bufferIndex + bufferLengthToWrite >= bufferCount - 1) {
				wasError = 1;
				break;
			}

			for (size_t strIndex = 0; strIndex < bufferLengthToWrite; strIndex++) {
				buffer[bufferIndex] = bufferToWrite[strIndex];
				bufferIndex += 1;
			}
		} else {
			if (bufferIndex >= bufferCount - 1) {
				wasError = 1;
				break;
			}
			buffer[bufferIndex] = formatChar;
			bufferIndex += 1;
		}
	}

	va_end (args);

	if (wasError == 1) {
		buffer[0] = 0;
		return -1;
	} else {
		buffer[bufferIndex] = 0;
		return (int) bufferIndex;
	}
}

#define swprintf swprintf_hack_only_for_d_and_ls

#endif
