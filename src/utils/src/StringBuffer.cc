#include "StringBuffer.h"
#include <string.h>

const char StringBuffer::kCRLF[] = "\r\n";

StringBuffer::StringBuffer(const uint64_t high_water_level_threahold):
high_water_level_threahold_(high_water_level_threahold),
string_buffer_(),
read_index_(0)
{
}

int StringBuffer::writeBuffer(const std::string& data)
{
	if(!highWaterLevelFlag())
	{
		if(getWritableBytes() < data.length())
		{
			string_buffer_.append(data);
		}
		else
		{
			if(getBackwardBytes() < data.length())
			{
				eraseBuffer(read_index_);
			}
			string_buffer_.append(data);
		}
		return 0;
	}
	return -1;
}

int StringBuffer::writeBuffer(const char* addr, const size_t size)
{
	if(addr && !highWaterLevelFlag())
	{
		if(getWritableBytes() < size)
		{
			string_buffer_.append(addr, size);
		}
		else
		{
			if(getBackwardBytes() < size)
			{
				eraseBuffer(read_index_);
			}
			string_buffer_.append(addr, size);
		}
		return 0;
	}
	return -1;
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
int StringBuffer::eraseBuffer(const size_t size)
{
	if(size <= string_buffer_.length())
	{
		string_buffer_.erase(string_buffer_.begin(), string_buffer_.begin() + size);
		read_index_  -= size;
		return 0;
	}
	return -1;
}
