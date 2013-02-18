#pragma once
#ifndef MMLT_UTILS_ASSERTIONS_H
#define MMLT_UTILS_ASSERTIONS_H

#include <QString>

#ifdef NDEBUG
#  define MMLT_precondition(EX)         (static_cast<void>(0))
#  define MMLT_precondition_msg(EX,MSG) (static_cast<void>(0))
#else

void MMLT_precondition_fail(const char* expression,
                            const char* file_name,
                            int line,
                            const QString& message = "");

#define MMLT_precondition(EX) \
    if(!(EX)) { MMLT_precondition_fail( # EX , __FILE__, __LINE__); }
#define MMLT_precondition_msg(EX,MSG) \
    if(!(EX)) { MMLT_precondition_fail( # EX , __FILE__, __LINE__, MSG); }
#endif

#ifdef NDEBUG
#  define MMLT_postcondition(EX)         (static_cast<void>(0))
#  define MMLT_postcondition_msg(EX,MSG) (static_cast<void>(0))
#else

void MMLT_postcondition_fail(const char* expression,
                             const char* file_name,
                             int line,
                             const QString& message = "");

#define MMLT_postcondition(EX) \
    if(!(EX)) { MMLT_postcondition_fail( # EX , __FILE__, __LINE__); }
#define MMLT_postcondition_msg(EX,MSG) \
    if(!(EX)) { MMLT_postcondition_fail( # EX , __FILE__, __LINE__, MSG); }

#endif

#endif // MMLT_UTILS_ASSERTIONS_H
