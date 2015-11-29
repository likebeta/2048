#include "Tools.h"

string format(const char* szFormat, ...)
{
	char szBuffer[4096];
	va_list args;
	va_start(args, szFormat);
	int nTmpLen = vsnprintf(szBuffer, 4096, szFormat, args);
	va_end(args);
	if (nTmpLen < 0)
	{
		return "";
	}

	return szBuffer;
}

time_t get_tomorrow_start_ts()
{
	time_t now_ts = time(NULL);
	tm* _tm = localtime(&now_ts);
	return now_ts + 86400 - _tm->tm_hour * 3600 - _tm->tm_min * 60 - _tm->tm_sec;
}

time_t get_today_start_ts()
{
	time_t now_ts = time(NULL);
	tm* _tm = localtime(&now_ts);
	return now_ts - _tm->tm_hour * 3600 - _tm->tm_min * 60 - _tm->tm_sec;
}

string format_time(const string& fmt)
{
	time_t rawtime = time(&rawtime);
	tm* timeinfo = localtime(&rawtime);
	char buffer[80];
	strftime(buffer, 80, fmt.c_str(), timeinfo);
	return buffer;
}
