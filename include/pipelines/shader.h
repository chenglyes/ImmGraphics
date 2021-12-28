#ifndef __IMMGRAPHICS_HEADER_SHADER__
#define __IMMGRAPHICS_HEADER_SHADER__

#include "../vertex.h"

namespace ImmGraphics
{

    class Shader
    {
    public:
        virtual ~Shader() {}

    public:
        virtual void VSMain(Vertex& now) = 0;
        virtual unsigned PSMain() = 0;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_SHADER__
