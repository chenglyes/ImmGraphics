#include "device.h"

#include "uitls.h"

using namespace ImmGraphics;

MemoryDevice::MemoryDevice()
	: m_width(0), m_height(0), m_frameBuffer(nullptr), m_zBuffer(nullptr)
{

}

MemoryDevice::~MemoryDevice()
{

}

void MemoryDevice::ClearBuffer()
{
	assert(m_frameBuffer && m_zBuffer);
	memset(m_frameBuffer, 0, m_width * m_height * sizeof(unsigned));

	for (int i = 0; i < m_width * m_height; i++)
		m_zBuffer[i] = 100.0f;
}

void MemoryDevice::SetPixel(int x, int y, const Vec3& color)
{
	assert(m_frameBuffer);
	((unsigned*)m_frameBuffer)[y * m_width + x] = Color::RGB(color);
}

Vec3 MemoryDevice::GetPixel(int x, int y)
{
	assert(m_frameBuffer);
	return Color::UINT(((unsigned*)m_frameBuffer)[y * m_width + x]);
}

void MemoryDevice::SetZ(int x, int y, float z)
{
	assert(m_zBuffer);
	m_zBuffer[y * m_width + x] = z;
}

float MemoryDevice::GetZ(int x, int y)
{
	assert(m_zBuffer);
	return m_zBuffer[y * m_width + x];
}

int MemoryDevice::GetWidth()
{
	return m_width;
}

int MemoryDevice::GetHeight()
{
	return m_height;
}

void MemoryDevice::SetWidth(int width)
{
	m_width = width;
}

void MemoryDevice::SetHeight(int height)
{
	m_height = height;
}

void MemoryDevice::SetFrameBuffer(unsigned* buffer)
{
	m_frameBuffer = buffer;
}

unsigned*& MemoryDevice::GetFrameBuffer()
{
	return m_frameBuffer;
}

void MemoryDevice::SetZBuffer(float* buffer)
{
	m_zBuffer = buffer;
}

float*& MemoryDevice::GetZBuffer()
{
	return m_zBuffer;
}
