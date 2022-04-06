#ifndef __IMMGRAPHICS_HEADER_SHADER__
#define __IMMGRAPHICS_HEADER_SHADER__

#include "../vertex.h"

namespace ImmGraphics
{
    typedef std::map<std::string, unsigned> DataListUnsigned;
    typedef std::map<std::string, float> DataListFloat;
    typedef std::map<std::string, Vec2> DataListFloat2;
    typedef std::map<std::string, Vec3> DataListFloat3;
    typedef std::map<std::string, Vec4> DataListFloat4;

    struct VaryingData
    {
        DataListUnsigned U1;
        DataListFloat F1;
        DataListFloat2 F2;
        DataListFloat3 F3;
        DataListFloat4 F4;
    };

    class Shader
    {
    public:
        Shader() {}
        virtual ~Shader() {}

    public:
        virtual Vec3 VSMain(const Vertex& now, VaryingData& datas) = 0;
        virtual Vec3 PSMain(VaryingData& datas) = 0;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_SHADER__
