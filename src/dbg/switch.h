#if   defined(DBG_NONE)
  #include "dbg/none.h"
#elif defined(DBG_TRACE)
  #include "dbg/trace.h"
#else
  #error DBG_* is undefined
#endif

#include "dbg/common.h"
