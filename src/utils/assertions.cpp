#include "utils/logger.h"

#include "assertions.h"

namespace MMLT
{

void MMLT_assertion_fail(const char* expression,
                         const char* file_name,
                         int line,
                         const QString& message)
{
    logger.error(mmlt_msg("Assertion %1 failed in file %2 at line %3! %4")
                 .arg(expression)
                 .arg(file_name)
                 .arg(line)
                 .arg(message));
    std::terminate();
}

void MMLT_precondition_fail(const char* expression,
                            const char* file_name,
                            int line,
                            const QString& message)
{
    logger.error(mmlt_msg("Precondition %1 failed in file %2 at line %3! %4")
                 .arg(expression)
                 .arg(file_name)
                 .arg(line)
                 .arg(message));
    std::terminate();
}

void MMLT_postcondition_fail(const char* expression,
                             const char* file_name,
                             int line,
                             const QString& message)
{
    logger.error(mmlt_msg("Postcondition %1 failed in file %2 at line %3! %4")
                 .arg(expression)
                 .arg(file_name)
                 .arg(line)
                 .arg(message));
    std::terminate();
}

}
