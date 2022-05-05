#ifndef __IMMGRAPHICS_HEADER_DEVICE__
#define __IMMGRAPHICS_HEADER_DEVICE__

#include "gmath.h"

namespace ImmGraphics
{

    class RenderDevice
    {
    public:
        virtual ~RenderDevice() = default;

        virtual void ClearBuffer() = 0;

        virtual void SetPixel(int x, int y, const Vec3& color) = 0;
        virtual Vec3 GetPixel(int x, int y) = 0;

        virtual void SetZ(int x, int y, float z) = 0;
        virtual float GetZ(int x, int y) = 0;

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;

    };

    class MemoryDevice : public RenderDevice
    {
    public:
        MemoryDevice();
        virtual ~MemoryDevice();

        void ClearBuffer() override;

        void SetPixel(int x, int y, const Vec3 & color) override;
        Vec3 GetPixel(int x, int y) override;

        void SetZ(int x, int y, float z) override;
        float GetZ(int x, int y) override;

        int GetWidth() override;
        int GetHeight() override;
        void SetWidth(int width);
        void SetHeight(int height);

        void SetFrameBuffer(unsigned* buffer);
        unsigned*& GetFrameBuffer();

        void SetZBuffer(float* buffer);
        float*& GetZBuffer();

    private:
        int m_width, m_height;
        unsigned* m_frameBuffer;
        float* m_zBuffer;
    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_DEVICE__
