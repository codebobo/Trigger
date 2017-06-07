#ifndef STRINGBUFFER_H_
#define STRINGBUFFER_H_

#include <algorithm>
#include <string>
#include <limits.h>
#include "Log.h"


class StringBuffer
{
public:
	explicit StringBuffer(const uint64_t high_water_level_threahold = INT_MAX);
	int writeBuffer(const std::string& data);
	int writeBuffer(const char* addr, const size_t size);
	size_t readBuffer(const char* addr, const size_t size);
	size_t readBuffer(std::string& data, const size_t size);
	void retrieve(size_t n)
	{
		read_index_ += n;
		read_index_ = (read_index_ <= string_buffer_.length() ? read_index_ : string_buffer_.length());
	}

	std::string retrieveAllAsString()
	{
		LOG4CPLUS_DEBUG(_logger, "string info: "<<read_index_<<"  "<<getReadableBytes()) ;
		std::string str = string_buffer_.substr(read_index_, getReadableBytes());
		clear();
		return str;
	}
	void clear()
	{
		string_buffer_.clear();
		read_index_ = 0;
	}
	
	bool highWaterLevelFlag() const
	{
		return getReadableBytes() >= high_water_level_threahold_ ? true : false;
	}
	const char* getReadAddr() const
	{
		return static_cast<const char*>(&(*(string_buffer_.begin() + read_index_)));
	}
	const char* peek() const
	{
		return getReadAddr();
	}
	const char* stringEnd() const
	{
		return string_buffer_.c_str() + string_buffer_.length();
	}
	const char* findCRLF() const
	{
	    const char* crlf = std::search(peek(), stringEnd(), kCRLF, kCRLF+2);
	    return crlf == stringEnd() ? NULL : crlf;
	}
	size_t getReadableBytes() const
	{
		return string_buffer_.length() - read_index_; 
	}
	size_t getWritableBytes() const
	{
		return getForwardBytes() + getBackwardBytes();
	}
private:
	uint64_t high_water_level_threahold_;
	uint64_t read_index_;
	std::string string_buffer_;

	int eraseBuffer(const size_t size);
	size_t getBackwardBytes() const
	{
		return string_buffer_.capacity() - string_buffer_.length();
	}
	size_t getForwardBytes() const
	{
		return read_index_;
	}

	const static char kCRLF[];
};

#endif

