#pragma once
#ifndef MMLT_UTILS_ELAPSED_TIMER_H
#define MMLT_UTILS_ELAPSED_TIMER_H

#include <QElapsedTimer>

class ElapsedTimer :
        public QElapsedTimer
{
public:
    ElapsedTimer(bool autostart = false) :
        QElapsedTimer()
    {
        if(autostart)
        {
            this->start();
        }
    }
};

#endif // MMLT_UTILS_ELAPSED_TIMER_H
