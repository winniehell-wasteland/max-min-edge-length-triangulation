#pragma once
#ifndef MMLT_UTILS_ABORT_TIMER_H
#define MMLT_UTILS_ABORT_TIMER_H

#include <QTimer>

#include "utils/logger.h"

class AbortTimer :
        public QTimer
{
    Q_OBJECT
public:
    AbortTimer(int timeout) :
        QTimer()
    {
        this->setInterval(timeout);
        this->setSingleShot(true);

        connect(this, SIGNAL(timeout()), this, SLOT(abort()));
    }

public Q_SLOTS:
    void abort()
    {
        logger.info("Aborted.");
        exit(EXIT_FAILURE);
    }
};

#endif // MMLT_UTILS_ABORT_TIMER_H
