#ifndef __IMMGRAPHICS_HEADER_DEBUG__
#define __IMMGRAPHICS_HEADER_DEBUG__

#include <string>

#ifdef _DEBUG
#include <iostream>

#define DEBUG_Print(msg) \
    std::cout << "DEBUG-Print(" << __FILE__ << ", " << __LINE__ <<  "): " << msg << std::endl

#endif

#endif // !__IMMGRAPHICS_HEADER_DEBUG__
