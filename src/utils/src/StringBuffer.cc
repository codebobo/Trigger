#include <string.h>
#include "StringBuffer.h"
#include "Log.h"

const char StringBuffer::kCRLF[] = "\r\n";

StringBuffer::StringBuffer():
read_index_(0),
write_index_(0)
{
}

int StringBuffer::writeBuffer(const std::string& data)
{
	writeBuffer(data.c_str(), data.length());
}

void StringBuffer::resizeBuffer(const long size)
{
	buffer_.resize(size);
}


void StringBuffer::adjustBuffer()
{
	long len = getReadableBytes();
	memcpy(static_cast<char*>(&(*buffer_.begin())), peek(), len);
	read_index_ = 0;
	write_index_ = len;
}

int StringBuffer::writeBuffer(const char* addr, const size_t size)
{
	//LOG4CPLUS_DEBUG(_logger, "write buffer "<<getBackwardWritableBytes()<<" "<<getForwardWritableBytes()<<" "<<buffer_.capacity());
	if(getBackwardWritableBytes() >= size)
	{
		memcpy(const_cast<char*>(end()), addr, size);
	}
	else if(getBackwardWritableBytes() + getForwardWritableBytes() >= size)
	{
		adjustBuffer();
		memcpy(const_cast<char*>(end()), addr, size);
	}
	else
	{
		resizeBuffer(buffer_.capacity() + size - getBackwardWritableBytes() + 1);
		memcpy(const_cast<char*>(end()), addr, size);
	}
	write_index_ += size;
	//LOG4CPLUS_DEBUG(_logger, "write buffer "<<write_index_);
}
size_t StringBuffer::readBuffer(const char* addr, const size_t size)
{
	if(addr)
	{
		uint64_t readableBytes = getReadableBytes();
		if(size >= readableBytes)
		{
			memcpy(const_cast<char*>(addr), getReadAddr(), readableBytes);
			read_index_ += readableBytes;
			return readableBytes;
		}
		else
		{
			memcpy(const_cast<char*>(addr), getReadAddr(), size);
			read_index_ += readableBytes;
			return size;
		}
	}
	return -1;
}
size_t StringBuffer::readBuffer(std::string& data, const size_t size)
{
	uint64_t readableBytes = getReadableBytes();
	
	if(size >= readableBytes)
	{
		data.append(getReadAddr(), readableBytes);
		read_index_ += readableBytes;
		return readableBytes;
	}
	else
	{
		data.append(getReadAddr(), size);
		read_index_ += size;
		return size;
	}
}

