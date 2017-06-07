#ifndef STRINGBUFFER_H_
#define STRINGBUFFER_H_

#include <algorithm>
#include <string>
#include <limits.h>
#include "Log.h"


class StringBuffer
{
public:
	explicit StringBuffer();
	int writeBuffer(const std::string& data);
	int writeBuffer(const char* addr, const size_t size);
	size_t readBuffer(const char* addr, const size_t size);
	size_t readBuffer(std::string& data, const size_t size);
	void retrieve(size_t n)
	{
		read_index_ += n;
		read_index_ = (read_index_ <= buffer_.size() ? read_index_ : buffer_.size());
	}
	void retrieveAll()
	{
		retrieve(getReadableBytes());
	}
	std::string retrieveAllAsString()
	{
		retrieveAsString(getReadableBytes());
	}

	std::string retrieveAsString(long len)
	{
		LOG4CPLUS_DEBUG(_logger, "string info: "<<read_index_<<"  "<<len) ;
		std::string str(peek(), len);
		retrieve(len);
		return str;
	}
	void clear()
	{
		buffer_.clear();
		read_index_ = 0;
		write_index_ = 0;
	}
	const char* getReadAddr() const
	{
		return static_cast<const char*>(&(*(buffer_.begin() + read_index_)));
	}
	const char* peek() const
	{
		return getReadAddr();
	}
	const char* end() const
	{
		return static_cast<const char*>(&(*(buffer_.begin() + write_index_)));
	}
	const char* findCRLF() const
	{
	    const char* crlf = std::search(peek(), end(), kCRLF, kCRLF+2);
	    return crlf == end() ? NULL : crlf;
	}
	size_t getReadableBytes() const
	{
		return write_index_ - read_index_; 
	}
	size_t getTotalWritableBytes() const
	{
		return getBackwardWritableBytes() + getForwardWritableBytes();
	}
	void append(const std::string& str)
	{
		writeBuffer(str);
	}
	void unwrite(long size)
	{
		if(getReadableBytes() >= size)
		{
			write_index_ -= size;
		}
	}
	void retrieveUntil(const char* ptr)
	{
	    if(peek() <= ptr && ptr <= end());
	    {
	    	retrieve(ptr - peek());
	    }
	}
	
private:
	uint64_t read_index_;
	uint64_t write_index_;
	std::vector<char> buffer_;

	void resizeBuffer(const long size);
	void adjustBuffer();

	size_t getBackwardWritableBytes() const
	{
		return buffer_.capacity() - write_index_;
	}
	size_t getForwardWritableBytes() const
	{
		return read_index_;
	}

	const static char kCRLF[];
};

#endif

