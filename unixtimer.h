#ifndef _UNIX_TIMER_H
#define _UNIX_TIMER_H
#include <sys/time.h>

class UnixTimer {
public:
    UnixTimer ();
    UnixTimer (const UnixTimer& timer);
    ~UnixTimer ();
    void start ();
    void stop ();
    void reset ();
    double read ();
    UnixTimer& operator= (const UnixTimer& timer);
private:
    typedef enum {MSTFalse = 0, MSTTrue = 1} MSTBoolean;
    struct timeval      mStartTime;
    struct timeval      mStopTime;
    MSTBoolean          mIsStopped;
};
#endif /* _MST_TIMER_H */


###################################################################
###                  Bryan Gonzalez                             ### 
###               github.com/rbgonzalez                         ###
###	              July 23, 2008                             ###
###           gonzalez.bryan@gmail.com                          ###
################################################################### 
