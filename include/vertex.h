#ifndef __IMMGRAPHICS_HEADER_VERTEX__
#define __IMMGRAPHICS_HEADER_VERTEX__

#include "gmath.h"
#include "uitls.h"

namespace ImmGraphics
{
    
    class Vertex
    {
    public:
        Vec3 pos;
        unsigned color;
        Vec3 norm;
        Vec2 uv;
    };

    typedef Container<Vertex> VertexBuffer;
    typedef Container<unsigned> IndexBuffer;

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_VERTEX__