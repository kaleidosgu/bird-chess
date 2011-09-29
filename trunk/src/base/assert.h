#ifndef __base_assert_h_
#define __base_assert_h_

#include "log.h"

#ifndef ASSERT

#ifdef NDEBUG
#define ASSERT(x) ((void)(0))
#else // Not NDEBUG
#define ASSERT(x) ((x) ? (void)(0) : WriteLog(LEVEL_ERROR, "ASSERT(%s) Failed. %s(%d): %s.\n", #x, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#endif // NDEBUG

#endif // ASSERT

#endif // __base_assert_h_
