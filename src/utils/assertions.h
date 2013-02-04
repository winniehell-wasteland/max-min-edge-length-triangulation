#ifndef MMT__UTILS__ASSERTIONS_H
#define MMT__UTILS__ASSERTIONS_H

#ifdef NDEBUG
#  define MMT_precondition(EX)         (static_cast<void>(0))
#  define MMT_precondition_msg(EX,MSG) (static_cast<void>(0))
#else

void MMT_precondition_fail(const char* expression,
                           const char* file_name,
                           int line,
                           const char* message = "");

#define MMT_precondition(EX) \
    if(!(EX)) { MMT_precondition_fail( # EX , __FILE__, __LINE__); }
#define MMT_precondition_msg(EX,MSG) \
    if(!(EX)) { MMT_precondition_fail( # EX , __FILE__, __LINE__, MSG); }
#endif

#ifdef NDEBUG
#  define MMT_postcondition(EX)         (static_cast<void>(0))
#  define MMT_postcondition_msg(EX,MSG) (static_cast<void>(0))
#else

void MMT_postcondition_fail(const char* expression,
                            const char* file_name,
                            int line,
                            const char* message = "");

#define MMT_postcondition(EX) \
    if(!(EX)) { MMT_postcondition_fail( # EX , __FILE__, __LINE__); }
#define MMT_postcondition_msg(EX,MSG) \
    if(!(EX)) { MMT_postcondition_fail( # EX , __FILE__, __LINE__, MSG); }

#endif

#endif // MMT__UTILS__ASSERTIONS_H
