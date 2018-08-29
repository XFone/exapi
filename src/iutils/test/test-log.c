#include "Base.h"
#include "Log.h"

void test_log(void)
{
    set_log_priority(LOG_DEBUG);
    LOGFILE(LOG_WARN, "current log level is %d", g_nLogPriority);
}
