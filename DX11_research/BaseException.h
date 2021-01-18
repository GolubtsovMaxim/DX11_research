#pragma once

#include <exception>
#include <string>

class BaseException : public std::exception
{
public:
	BaseException(const unsigned line, const char* file) noexcept;
	const char* what() const noexcept override; //inherited from std::exception
	unsigned get_line() const noexcept;
	virtual const char* get_type() const noexcept;
	const std::string& get_file() const noexcept;
	std::string get_origin_string() const noexcept;
private:
	unsigned line_;
	std::string file_;
protected:
	mutable std::string what_buffer; //this one is mutable since what() is declared const and we need to make whatBuffer mutable in order to set it from within what()
};