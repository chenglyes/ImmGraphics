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

        void SetShader(Shader* Shader);
        void SetBackCulled(bool option);
        void SetMutiSampled(bool option);
        void SetFastSampled(bool option);

    private:
        inline Vec3 BufferToRelativeTView(const Vec3& position);
        inline Vec3 RelativeToBufferView(const Vec3& position);
        void RenderPrimitive(const VertexBuffer& vertices, int id_a, int id_b, int id_c);
        void HandleVarying(const Vec3& weight, VaryingData* datas);
        void Rasterize(const Triangle3D& triangle, VaryingData* datas);
        void RasterizeRect(const Triangle3D& triangle, VaryingData* datas, const RectI& rect);
        void FastRasterize(const Triangle3D& triangle, VaryingData* datas);

    protected:
        Shader* m_shader;

        bool m_backCulled;
        bool m_mutiSampled;
        bool m_fastSampled;
    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_PIPELINES_SHADERPIP__
