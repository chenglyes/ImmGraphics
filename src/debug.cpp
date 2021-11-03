#include "debug.h"

#ifdef _DEBUG
    #include <iostream>
#endif

using namespace ImmGraphics;

void Debug::Print(const std::string& message)
{
    #ifdef _DEBUG
        std::cout << "ImmGraphics::Print > " << message << std::endl;
    #endif
}
