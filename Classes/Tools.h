#pragma once

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

string format(const char* szFormat, ...);

template<typename T>
string integer2str(T n)
{
	stringstream strStream;
	strStream << n;
	return strStream.str();
}

template<typename T>
T str2interge(const string& s)
{
	T nReslult = 0;
	stringstream strStream;
	strStream << s;
	strStream >> nReslult;
	return nReslult;
}

template<typename T>
string seq2string(T _begin, T _end, const string& _sep = ", ")
{
	if (_begin == _end)
	{
		return "[]";
	}
	stringstream ss;
	ss << "[" << *_begin;
	for (auto it = ++_begin; it != _end; ++it)
	{
		ss << _sep << *it;
	}
	ss << "]";
	return ss.str();
}

template<typename T>
string map2string(T _begin, T _end, const string& _sep = ", ")
{
	if (_begin == _end)
	{
		return "{}";
	}
	stringstream ss;
	ss << "{" << _begin->first << ": " << _begin->second;
	for (auto it = ++_begin; it != _end; ++it)
	{
		ss << _sep << _begin->first << ": " << _begin->second;
	}
	ss << "}";
	return ss.str();
}

time_t get_tomorrow_start_ts();
time_t get_today_start_ts();

string format_time(const string& fmt);
