#ifndef __IMMGRAPHICS_HEADER_PIPELINES_SHADERPIP__
#define __IMMGRAPHICS_HEADER_PIPELINES_SHADERPIP__

#include "../pipeline.h"
#include "shader.h"

namespace ImmGraphics
{
    class ShaderPipeline : public RenderPipeline
    {
    public:
        ShaderPipeline();
        virtual ~ShaderPipeline();

    public:
        virtual void StartPipeline(const VertexBuffer& vertices, const IndexBuffer& indices);

        void AddShader(Shader* shader);

    protected:
        Container<Shader*> m_shaders;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_PIPELINES_SHADERPIP__
