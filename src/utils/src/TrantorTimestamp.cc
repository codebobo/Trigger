#include "TrantorTimestamp.h"

TrantorTimestamp::TrantorTimestamp(const int64_t time_since_epoch_usec)
{
	time_since_epoch_usec_ = time_since_epoch_usec;
	tv_.tv_sec = time_since_epoch_usec/1000000;
	tv_.tv_usec = time_since_epoch_usec - tv_.tv_sec * 1000000;
	ts_.tv_sec = time_since_epoch_usec/1000000;
	ts_.tv_nsec = time_since_epoch_usec * 1000 - ts_.tv_sec * 1000000000;
	int64_t seconds = tv_.tv_sec;
	gmtime_r(&seconds, &tm_time_);
}
std::string TrantorTimestamp::transToString() const
{
	std::string date;
	std::string time;
	std::stringstream sstr;
	sstr<<(tm_time_.tm_year + 1900)<<"-"<<(tm_time_.tm_mon + 1)<<"-"<<tm_time_.tm_mday<<" ";
	sstr>>date;

	sstr<<tm_time_.tm_hour<<":"<<tm_time_.tm_min<<":"<<tm_time_.tm_sec<<".";
	sstr<<ts_.tv_nsec/1000;
	sstr>>time;

	return date + " " + time;
}
std::string TrantorTimestamp::getFormatDateTime() const
{
	std::string date;
	std::string time;
	std::stringstream sstr;
	std::stringstream sstr1;
	sstr.width(4);
	sstr<<(tm_time_.tm_year + 1900);
	sstr.width(2);
	sstr1.fill('0');
	sstr<<(tm_time_.tm_mon + 1);
	sstr.width(2);
	sstr1.fill('0');
	sstr<<tm_time_.tm_mday;
	sstr>>date;

	sstr1.width(2);
	sstr1.fill('0');
	sstr1<<tm_time_.tm_hour;
	sstr1.width(2);
	sstr1.fill('0');
	sstr1<<tm_time_.tm_min;
	sstr1.width(2);
	sstr1.fill('0');
	sstr1<<tm_time_.tm_sec;
	sstr1>>time;
	std::cout<<"date: "<<date<<" time: "<<time<<" "<<tm_time_.tm_hour<<std::endl;
	return (date + time);
}

struct tm TrantorTimestamp::getDateTime() const
{
	return tm_time_;
}

std::string TrantorTimestamp::getDate() const
{
	std::string date;
	std::stringstream sstr;
	std::stringstream sstr1;
	sstr.width(4);
	sstr<<(tm_time_.tm_year + 1900);
	sstr.width(2);
	sstr1.fill('0');
	sstr<<(tm_time_.tm_mon + 1);
	sstr.width(2);
	sstr1.fill('0');
	sstr<<tm_time_.tm_mday;
	sstr>>date;
	return date;
}

timeval TrantorTimestamp::getTimeval() const
{
	return tv_;
}
timespec TrantorTimestamp::getTimespec() const
{
	return ts_; 
}
int64_t TrantorTimestamp::getUsecond() const
{
	return time_since_epoch_usec_;
}

TrantorTimestamp TrantorTimestamp::now()
{
	timeval tv;
	if(gettimeofday(&tv, NULL) == 0)
	{
		return TrantorTimestamp(tv.tv_sec*1000000 + tv.tv_usec);
	}
}

TrantorTimestamp TrantorTimestamp::getCurrentDayStartTimestamp()
{
	TrantorTimestamp start_timestamp = TrantorTimestamp::now();
	struct tm tm_time = start_timestamp.getDateTime();
	tm_time.tm_hour = 0;
	tm_time.tm_min = 0;
	tm_time.tm_sec = 0;
	return TrantorTimestamp(mktime(&tm_time) * 1000000);
}

