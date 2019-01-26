#ifndef __XTIME_H__
#define __XTIME_H__
#include <time.h>
#include <sys/time.h>

class xtime
{
protected:
	struct timeval t;
public:
	xtime(time_t sec = -1)
	{
		(-1 == sec) ? (void)init() : set(sec);
	}
	const time_t second(void)const
	{
		return t.tv_sec;
	}
	const time_t msecond(void)const
	{
		return t.tv_usec / 1000;
	}
	const time_t usecond(void)const
	{
		return t.tv_usec;
	}
	const time_t diff(void)const
	{
		return time(0) - t.tv_sec;
	}
	const time_t sdiff(void)const
	{
		return time(0) - t.tv_sec;
	}
	const time_t mdiff(void)const
	{
		xtime x;
		x.t.tv_sec -= t.tv_sec;
		return x.t.tv_sec*1000 + x.t.tv_usec/1000 - t.tv_usec/1000;
	}
	const time_t udiff(void)const
	{
		xtime x;
		x.t.tv_sec -= t.tv_sec;
		return x.t.tv_sec*1000*1000 + x.t.tv_usec - t.tv_usec;
	}
	bool set(const char *s, const char *fmt)
	{
		struct tm m;;
		if(s && fmt && strptime(s, fmt, &m))
		{
			t.tv_sec = mktime(&m);
			return true;
		}
		return false;
	}
	void set(time_t sec)
	{
		t.tv_sec = sec;
	}
	bool stime(void)const
	{
		return ::stime(&(t.tv_sec));
	}
	bool update(void)
	{
		return init();
	}
	bool init(void)
	{
		return gettimeofday(&t, 0)==0;
	}
};
class datetime : public xtime
{
protected:
	char data[32];
public:
	datetime(time_t sec = -1)
	{
		(-1 == sec) ? (void)init() : set(sec);
	}
	const char* format(const char *fmt)const
	{
		strftime((char*)data, sizeof(data), fmt, localtime(&t.tv_sec));
		return data;
	}
};
#define Datetime datetime

#endif//__XTIME_H__
