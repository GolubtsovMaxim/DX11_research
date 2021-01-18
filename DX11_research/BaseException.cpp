#include "BaseException.h"
#include <sstream>

BaseException::BaseException(const unsigned line, const char* file) noexcept
	:
	line_(line),
	file_(file)
{
	
}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;
	oss << get_type() << "\n" << get_origin_string();
	what_buffer = oss.str();
	return what_buffer.c_str();
}

const char* BaseException::get_type() const noexcept
{
	return "very base exception";
}

const std::string& BaseException::get_file() const noexcept
{
	return file_;
}

unsigned BaseException::get_line() const noexcept
{
	return line_;
}


std::string BaseException::get_origin_string() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file_ << "\n"
		<< "[Line] " << line_;
	return oss.str();
}
