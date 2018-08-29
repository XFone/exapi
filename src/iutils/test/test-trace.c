#include "Base.h"
#include "Trace.h"

void test_trace()
{
    set_trace_prefix("UTILTEST");
    set_trace_level(7, TRUE); // print timestamp
    
    TRACE(3, "current trace level is %d", g_nTraceLevel);
}
