#include "utils/logger.h"

#include "assertions.h"

void MMT_precondition_fail(const char* expression,
                           const char* file_name,
                           int line,
                           const char* message)
{
    logger.error(mmt_msg("Precondition %1 failed in file %2 at line %3! %4")
                 .arg(expression)
                 .arg(file_name)
                 .arg(line)
                 .arg(message));
    std::terminate();
}

void MMT_postcondition_fail(const char* expression,
                            const char* file_name,
                            int line,
                            const char* message)
{
    logger.error(mmt_msg("Postcondition %1 failed in file %2 at line %3! %4")
                 .arg(expression)
                 .arg(file_name)
                 .arg(line)
                 .arg(message));
    std::terminate();
}
