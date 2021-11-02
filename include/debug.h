#ifndef __IMMGRAPHICS_HEADER_DEBUG__
#define __IMMGRAPHICS_HEADER_DEBUG__

#define _DEBUG

#include <string>

#ifdef _DEBUG
    #include <iostream>
#endif

namespace ImmGraphics
{

    class Debug
    {
    public:
        static void Print(const std::string& message)
        {
            #ifdef _DEBUG
                std::cout << "ImmGraphics::Print > " << message << std::endl;
            #endif
        }

    };


} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_DEBUG__
