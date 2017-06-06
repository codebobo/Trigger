#ifndef TRANTOR_TIMESTAMP_H_
#define TRANTOR_TIMESTAMP_H_

#include <sys/time.h>
#include <stdint.h>
#include <cstddef>
#include <iostream>
#include <sstream>


class TrantorTimestamp
{
public:
	explicit TrantorTimestamp(const int64_t time_since_epoch_usec = 0);
	TrantorTimestamp(const TrantorTimestamp& timestamp)
	{
		time_since_epoch_usec_ = timestamp.time_since_epoch_usec_;
		tv_ = timestamp.tv_;
		ts_ = timestamp.ts_;
		tm_time_ = timestamp.tm_time_;
	}
	inline void operator> (const TrantorTimestamp& rhs)
	{
		time_since_epoch_usec_ = rhs.time_since_epoch_usec_;
	}
	inline bool operator> (const TrantorTimestamp& rhs) const 
	{
		return time_since_epoch_usec_ > rhs.time_since_epoch_usec_;
	}
	inline bool operator< (const TrantorTimestamp& rhs) const
	{
		return time_since_epoch_usec_ < rhs.time_since_epoch_usec_;
	}
	inline bool operator>= (const TrantorTimestamp& rhs) const 
	{
		return time_since_epoch_usec_ >= rhs.time_since_epoch_usec_;
	}
	inline bool operator<= (const TrantorTimestamp& rhs) const
	{
		return time_since_epoch_usec_ <= rhs.time_since_epoch_usec_;
	}
	inline bool operator== (const TrantorTimestamp& rhs) const 
	{
		return time_since_epoch_usec_ == rhs.time_since_epoch_usec_;
	}

	static TrantorTimestamp now();
	static TrantorTimestamp getCurrentDayStartTimestamp();
	std::string transToString() const;
	struct tm getDateTime() const;
	std::string getFormatDateTime() const;
	std::string getDate() const;
	timeval getTimeval() const;
	timespec getTimespec() const;
	int64_t getUsecond() const;

	int64_t getTimeMicroSeconds() const
	{
		return time_since_epoch_usec_;
	}

	void swap(TrantorTimestamp& timestamp)
	{
		TrantorTimestamp tmp = timestamp;
		timestamp.time_since_epoch_usec_ = time_since_epoch_usec_;
		timestamp.tv_ = tv_;
		timestamp.ts_ = ts_;
		timestamp.tm_time_ = tm_time_;

		time_since_epoch_usec_ = tmp.time_since_epoch_usec_;
		tv_ = tmp.tv_;
		ts_ = tmp.ts_;
		tm_time_ = tmp.tm_time_;
	}

	friend inline TrantorTimestamp operator+ (const TrantorTimestamp& lhs, const int64_t interval);
	friend inline int64_t operator- (const TrantorTimestamp& lhs, const TrantorTimestamp& rhs);
	friend inline std::ostream& operator<< (std::ostream& os, const TrantorTimestamp& rhs) ;

private:
	int64_t time_since_epoch_usec_;
	timeval tv_;
	timespec ts_;
	struct tm tm_time_;
};

inline TrantorTimestamp operator + (const TrantorTimestamp& lhs, const int64_t interval)
{
	return TrantorTimestamp(lhs.time_since_epoch_usec_ + interval); 
}
inline int64_t operator - (const TrantorTimestamp& lhs, const TrantorTimestamp& rhs)
{
	return lhs.time_since_epoch_usec_ - rhs.time_since_epoch_usec_; 
}
inline std::ostream& operator<< (std::ostream& os, const TrantorTimestamp& rhs)  
{
	os<<rhs.transToString();
	return os;
}

#endif
