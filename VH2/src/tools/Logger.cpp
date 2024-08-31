#include "tools/Logger.hpp"

#include <stdio.h>

#include <cstdarg>


namespace logger {

	void Info(const char* formatStr, ...) {

		char buffer[512]{};

		va_list args{};
		va_start(args, formatStr);

		vsprintf_s(buffer, sizeof(buffer), formatStr, args);

		va_end(args);

		printf_s("%s\n", buffer);
	}
}