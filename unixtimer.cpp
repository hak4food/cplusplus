#include <unistd.h>
#include "unixtimer.h"

#if __GNUC__ >= 3
using namespace std;
#endif

UnixTimer::UnixTimer ()
:mIsStopped(MSTTrue)
{
    mStartTime.tv_sec = 0;
    mStartTime.tv_usec = 0;
    mStopTime.tv_sec = 0;
    mStopTime.tv_usec = 0;
}

UnixTimer::UnixTimer (const UnixTimer& timer)
{
    mStartTime = timer.mStartTime;
    mStopTime = timer.mStopTime;
    mIsStopped = timer.mIsStopped;
}

UnixTimer& UnixTimer::operator= (const UnixTimer& timer)
{
    if (this == &timer) {
        return *this;
    }
    mStartTime = timer.mStartTime;
    mStopTime = timer.mStopTime;
    mIsStopped = timer.mIsStopped;
    return *this;
}

UnixTimer::~UnixTimer ()
{
}

void UnixTimer::start()
{
    if (mIsStopped) {
        gettimeofday(&mStartTime, NULL);
        mIsStopped = MSTFalse;
    }
}

void UnixTimer::stop()
{
    if (!mIsStopped) {
        gettimeofday(&mStopTime, NULL);
        mIsStopped = MSTTrue;
    }
}

void UnixTimer::reset()
{
    mStartTime.tv_sec = 0;
    mStartTime.tv_usec = 0;
    mStopTime.tv_sec = 0;
    mStopTime.tv_usec = 0;
    mIsStopped = MSTTrue;
}

double UnixTimer::read()
{
    double dTime;
    struct timeval current;
    
    if (mIsStopped)
        current = mStopTime;
    else
        gettimeofday(&current, NULL);

    if (mStartTime.tv_usec > current.tv_usec) {
        current.tv_usec += 1000000;
        current.tv_sec--;
    }

    dTime= (double) ((current.tv_sec - mStartTime.tv_sec) * 1000000 +
                     (current.tv_usec - mStartTime.tv_usec));
    dTime=dTime*1.0e-6;
    return (double) dTime; 
                   
}


###################################################################
###                  Bryan Gonzalez                             ### 
###               github.com/rbgonzalez                         ###
###	              July 23, 2008                             ###
###           gonzalez.bryan@gmail.com                          ###
################################################################### 
