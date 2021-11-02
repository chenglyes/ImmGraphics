#ifndef __IMMGRAPHICS_HEADER_GMATH__
#define __IMMGRAPHICS_HEADER_GMATH__

#include <cmath>
#include <cstring>
#include <sstream>

#ifdef _NO_ASSERT
    #define _ASSERT
#endif

#ifndef _ASSERT
    #include <cassert>
    #define _ASSERT assert
#endif // !_ASSERT

namespace ImmGraphics
{

    /**
     * @brief Basic math method library (Based on <cmath>.)
     */
    class Math
    {
    public:
        static constexpr float PositiveInfinity = 3.402823466e+38F;
        static constexpr float NegativeInfinity = -3.402823466e+38F;
        static constexpr float PI = 3.1415926F;
        static constexpr float RAD = PI / 180;
        static constexpr float RRAD = 180 / PI;

        static float Precision;

    public:
        static float toRad(float angle) { return angle * RAD; }
        static float toAngle(float rad) { return rad * RRAD; }

        static float Sin(float rad) { return sinf(rad); }
        static float Cos(float rad) { return cosf(rad); }
        static float Tan(float rad) { return tanf(rad); }
        static float ASin(float rad) { return asinf(rad); }
        static float ACos(float rad) { return acosf(rad); }
        static float ATan(float rad) { return atanf(rad); }
        static float ATan(float r1, float r2) { return atan2f(r1, r2); }

        static bool Sign(float v) { return std::signbit(v); }
        static float Abs(float v) { return v < 0 ? -v : v; }
        static float Sqr(float v) { return v * v; }
        static float Sqrt(float v) { return sqrtf(v); }
        static float Exp(float i) { return expf(i); }
        static float Log2(float v) { return log2f(v); }
        static float Log10(float v) { return log10f(v); }
        static float LogE(float v) { return logf(v); }

        static float Ceil(float v) { return ceilf(v); }
        static float Floor(float v) { return floorf(v); }

        static bool NearZero(float v) { return Abs(v) < Precision; }
        static bool NearOne(float v) { return Abs(v - 1) < Precision; }

        static bool Near(float a, float b) { return Abs(a - b) < Precision; }
        static float Pow(float v, float i) { return powf(v, i); }
        static float Log(float v, float i) { return log(v) / log(i); }

        static float Min(float a, float b) { return a <= b ? a : b; }
        static float Min(float a, float b, float c) { return (a <= b ? (a <= c ? a : c) : (b <= c ? b : c)); }
        static float Max(float a, float b) { return a >= b ? a : b; }
        static float Max(float a, float b, float c) { return (a >= b ? (a >= c ? a : c) : (b >= c ? b : c)); }
        static float Clamp(float value, float min, float max) { return Min(Max(value, min), max); }
        static float Lerp(float start, float end, float k) { return start + (end - start) * k; }

    };

    float Math::Precision = 0.000001f;

    /**
    * @brief 2D Vector or Point
    */
    class Vec2
    {
    public:
        float x, y;

    public:
        Vec2(float x = 0, float y = 0): x(x), y(y) {}
        static Vec2 Zero() { return Vec2(); }
        static Vec2 Identity() { return Vec2(1, 1); }
        static Vec2 PositiveInfinity() 
            { return Vec2(Math::PositiveInfinity, Math::PositiveInfinity); }
        static Vec2 NegativeInfinity() 
            { return Vec2(Math::NegativeInfinity, Math::NegativeInfinity); }

        bool isZero() const { return Math::NearZero(x) && Math::NearZero(y); }
        bool isIdentity() const { return Math::NearOne(x) && Math::NearOne(y); }
        bool isNormalized() const { return Math::NearOne(LengthSqr()); }

        float& operator[](unsigned i) { _ASSERT(i < 2 && "The index is out of bounds."); if (i == 0) return x; return y; }
        float operator[](unsigned i) const { _ASSERT(i < 2 && "The index is out of bounds."); if (i == 0) return x; return y; }

        Vec2 operator+(const Vec2& obj) const { return Vec2(x + obj.x, y + obj.y); }
        Vec2 operator-(const Vec2& obj) const { return Vec2(x - obj.x, y - obj.y); }
        Vec2 operator-() const { return Vec2(-x, -y); }
        Vec2 operator*(float k) const { return Vec2(x * k, y * k); }
        friend Vec2 operator*(float k, const Vec2& vec) { return vec * k; }
        Vec2 operator/(float k) const { return Vec2(x / k, y / k); }

        void operator+=(const Vec2& obj) { x += obj.x; y += obj.y; }
        void operator-=(const Vec2& obj) { x -= obj.x; y -= obj.y; }
        void operator*=(float k) { x *= k; y *= k; }
        void operator/=(float k) { x /= k; y /= k; }

        bool operator==(const Vec2& obj) const { return Math::Near(x, obj.x) && Math::Near(y, obj.y); }
        bool operator!=(const Vec2& obj) const { return !Math::Near(x, obj.x) || !Math::Near(y, obj.y); }

        float LengthSqr() const { return x * x + y * y; }
        float Length() const { return Math::Sqrt(x * x + y * y); }

        void Normalize() { float len = Length(); if (!Math::NearZero(len)) { x /= len; y /= len; } }
        Vec2 getNormalized() const { Vec2 res(*this); res.Normalize(); return res; }
        void Reflect(const Vec2& normal) { float temp = 2 * Dot(*this, normal); operator-=(normal * temp); }
        Vec2 getReflected(const Vec2& normal) { Vec2 res(*this); res.Reflect(normal); return res; }

        static float Distance(const Vec2& pointA, const Vec2& pointB) { return Vec2(pointB - pointA).Length(); }
        static Vec2 Lerp(const Vec2& start, const Vec2& end, float k) { return start + (end - start) * k; }
        static float Dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
        static float Cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }
        static float Angle(const Vec2& a, const Vec2& b) { return Math::toAngle(Math::ACos(Dot(a, b) / (a.Length() * b.Length()))); }

        std::string toString() const
        {
            std::ostringstream os;
            os << "Vec2 { x = " << x << ", y = " << y << " }";
            return os.str();
        }
    };

    /**
    * @brief 3D Vector or Point
    */
    class Vec3
    {
    public:
        float x, y, z;

    public:
        Vec3(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}
        Vec3(const Vec2& v2, float z = 0): x(v2.x), y(v2.y), z(z) {}
        static Vec3 Zero() { return Vec3(); }
        static Vec3 Identity() { return Vec3(1, 1, 1); }
        static Vec3 Left() { return Vec3(-1, 0, 0); }
        static Vec3 Right() { return Vec3(1, 0, 0); }
        static Vec3 Forward() { return Vec3(0, 1, 0); }
        static Vec3 Back() { return Vec3(0, -1, 0); }
        static Vec3 Up() { return Vec3(0, 0, 1); }
        static Vec3 Down() { return Vec3(0, 0, -1); }
        static Vec3 PositiveInfinity() 
            { return Vec3(Math::PositiveInfinity, Math::PositiveInfinity, Math::PositiveInfinity); }
        static Vec3 NegativeInfinity() 
            { return Vec3(Math::NegativeInfinity, Math::NegativeInfinity, Math::NegativeInfinity); }

        bool isZero() const { return Math::NearZero(x) && Math::NearZero(y) && Math::NearZero(z); }
        bool isIdentity() const { return Math::NearOne(x) && Math::NearOne(y) && Math::NearOne(z); }
        bool isNormalized() const { return Math::NearOne(LengthSqr()); }

        float& operator[](unsigned i) 
            { _ASSERT(i < 3 && "The index is out of bounds."); if (i == 0) return x; if (i == 1) return y; return z; }
        float operator[](unsigned i) const
            { _ASSERT(i < 3 && "The index is out of bounds."); if (i == 0) return x; if (i == 1) return y; return z; }

        Vec3 operator+(const Vec3& obj) const { return Vec3(x + obj.x, y + obj.y, z + obj.z); }
        Vec3 operator-(const Vec3& obj) const { return Vec3(x - obj.x, y - obj.y, z - obj.z); }
        Vec3 operator-() const { return Vec3(-x, -y, -z); }
        Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }
        friend Vec3 operator*(float k, const Vec3& vec) { return vec * k; }
        Vec3 operator/(float k) const { return Vec3(x / k, y / k, z / k); }

        void operator+=(const Vec3& obj) { x += obj.x; y += obj.y; z += obj.z; }
        void operator-=(const Vec3& obj) { x -= obj.x; y -= obj.y; z -= obj.z; }
        void operator*=(float k) { x *= k; y *= k; z *= k; }
        void operator/=(float k) { x /= k; y /= k; z /= k; }

        bool operator==(const Vec3& obj) const { return Math::Near(x, obj.x) && Math::Near(y, obj.y) && Math::Near(z, obj.z); }
        bool operator!=(const Vec3& obj) const { return !Math::Near(x, obj.x) || !Math::Near(y, obj.y) || !Math::Near(z, obj.z); }

        operator Vec2() { return Vec2(x, y); }

        float LengthSqr() const { return x * x + y * y + z * z; }
        float Length() const { return Math::Sqrt(x * x + y * y + z * z); }

        void Normalize() { float len = Length(); if (!Math::NearZero(len)) { x /= len; y /= len; z /= len; } }
        Vec3 getNormalized() const { Vec3 res(*this); res.Normalize(); return res; }
        void Reflect(const Vec3& normal) { float temp = 2 * Dot(*this, normal); operator-=(normal * temp); }
        Vec3 getReflected(const Vec3& normal) { Vec3 res(*this); res.Reflect(normal); return res; }

        static float Distance(const Vec3& pointA, const Vec3& pointB) { return Vec3(pointB - pointA).Length(); }
        static Vec3 Lerp(const Vec3& start, const Vec3& end, float k) { return start + (end - start) * k; }
        static float Dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
        static Vec3 Cross(const Vec3& a, const Vec3& b) 
            { return Vec3(a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y); }
        static float Angle(const Vec3& a, const Vec3& b) { return Math::toAngle(Math::ACos(Dot(a, b) / (a.Length() * b.Length()))); }

        std::string toString() const
        {
            std::ostringstream os;
            os << "Vec3 { x = " << x << ", y = " << y << ", z = " << z << " }";
            return os.str();
        }
    };

    /**
    * @brief Homogeneous Coordinates for 3D Vector or Point
    */
    class Vec4
    {
    public:
        float x, y, z, w;

    public:
        Vec4(float x = 0, float y = 0, float z = 0, float w = 0): x(x), y(y), z(z), w(w) {}
        Vec4(const Vec3& v3, float w = 0): x(v3.x), y(v3.y), z(v3.z), w(w) {}
        Vec4(const Vec2& v2, float z = 0, float w = 0): x(v2.x), y(v2.y), z(z), w(w) {}
        static Vec4 Zero() { return Vec4(); }
        static Vec4 Identity() { return Vec4(1, 1, 1, 1); }
        static Vec4 PositiveInfinity() 
            { return Vec4(Math::PositiveInfinity, Math::PositiveInfinity, Math::PositiveInfinity, Math::PositiveInfinity); }
        static Vec4 NegativeInfinity() 
            { return Vec4(Math::NegativeInfinity, Math::NegativeInfinity, Math::NegativeInfinity, Math::NegativeInfinity); }

        bool isZero() const { return Math::NearZero(x) && Math::NearZero(y) && Math::NearZero(z) && Math::NearZero(w); }
        bool isIdentity() const { return Math::NearOne(x) && Math::NearOne(y) && Math::NearOne(z) && Math::NearOne(w); }
        bool isNormalized() const { return Math::NearOne(LengthSqr()); }

        float& operator[](unsigned i) 
            { _ASSERT(i < 4 && "The index is out of bounds."); if (i == 0) return x; if (i == 1) return y; if (i == 2) return z; return w; }
        float operator[](unsigned i) const
            { _ASSERT(i < 4 && "The index is out of bounds."); if (i == 0) return x; if (i == 1) return y; if (i == 2) return z; return w; }

        Vec4 operator+(const Vec4& obj) const { return Vec4(x + obj.x, y + obj.y, z + obj.z, w + obj.w); }
        Vec4 operator-(const Vec4& obj) const { return Vec4(x - obj.x, y - obj.y, z - obj.z, w - obj.w); }
        Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }
        Vec4 operator*(float k) const { return Vec4(x * k, y * k, z * k, w * k); }
        friend Vec4 operator*(float k, const Vec4& vec) { return vec * k; }
        Vec4 operator/(float k) const { return Vec4(x / k, y / k, z / k, w / k); }

        void operator+=(const Vec4& obj) { x += obj.x; y += obj.y; z += obj.z; w += obj.w; }
        void operator-=(const Vec4& obj) { x -= obj.x; y -= obj.y; z -= obj.z; w -= obj.w; }
        void operator*=(float k) { x *= k; y *= k; z *= k; w *= k; }
        void operator/=(float k) { x /= k; y /= k; z /= k; w /= k; }

        bool operator==(const Vec4& obj) const 
            { return Math::Near(x, obj.x) && Math::Near(y, obj.y) && Math::Near(z, obj.z) && Math::Near(w, obj.w); }
        bool operator!=(const Vec4& obj) const 
            { return !Math::Near(x, obj.x) || !Math::Near(y, obj.y) || !Math::Near(z, obj.z) || !Math::Near(w, obj.w); }

        operator Vec3() { return Vec3(x, y, z); }

        float LengthSqr() const { return x * x + y * y + z * z + w * w; }
        float Length() const { return Math::Sqrt(x * x + y * y + z * z + w * w); }

        void Normalize() { float len = Length(); if (!Math::NearZero(len)) { x /= len; y /= len; z /= len; w /= len; } }
        Vec4 getNormalized() const { Vec4 res(*this); res.Normalize(); return res; }

        static float Distance(const Vec4& pointA, const Vec4& pointB) { return Vec4(pointB - pointA).Length(); }
        static Vec4 Lerp(const Vec4& start, const Vec4& end, float k) { return start + (end - start) * k; }
        static float Dot(const Vec4& a, const Vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        std::string toString() const
        {
            std::ostringstream os;
            os << "Vec4 { x = " << x << ", y = " << y << ", z = " << z << ", w = " << w << " }";
            return os.str();
        }
    };

    /**
     * @brief 3 x 3 Matrix
     */
    class Matrix3
    {
    public:
        float value[3][3];

    public:
        Matrix3() { memset(value, 0, sizeof(value)); }
        Matrix3(float v[3][3])
        {
            value[0][0] = v[0][0]; value[0][1] = v[0][1]; value[0][2] = v[0][2];
            value[1][0] = v[1][0]; value[1][1] = v[1][1]; value[1][2] = v[1][2];
            value[2][0] = v[2][0]; value[2][1] = v[2][1]; value[2][2] = v[2][2];
        }
        Matrix3(float _00, float _01, float _02,
                float _10, float _11, float _12,
                float _20, float _21, float _22)
        {
            value[0][0] = _00; value[0][1] = _01; value[0][2] = _02;
            value[1][0] = _10; value[1][1] = _11; value[1][2] = _12;
            value[2][0] = _20; value[2][1] = _21; value[2][2] = _22;
        }
        static Matrix3 Zero() { return Matrix3(); }
        static Matrix3 Identity() { return Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1); }

        bool isZero() const { return operator==(Zero()); }
        bool isIdentity() const { return operator==(Identity()); }
        
        float* operator[](unsigned i) { _ASSERT(i < 3 && "The index is out of bounds."); return value[i]; }
        const float* operator[](unsigned i) const { _ASSERT(i < 3 && "The index is out of bounds."); return value[i]; }

        Matrix3 operator+(const Matrix3& obj) const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = value[i][j] + obj[i][j];
            return res;
        }
        Matrix3 operator-(const Matrix3& obj) const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = value[i][j] - obj[i][j];
            return res;
        }
        Matrix3 operator-() const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = -value[i][j];
            return res;
        }
        Matrix3 operator*(float k) const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = value[i][j] * k;
            return res;
        }
        friend Matrix3 operator*(float k, const Matrix3& obj) { return obj * k; }
        Matrix3 operator/(float k) const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = value[i][j] / k;
            return res;
        }
        
        void operator+=(const Matrix3& obj) { *this = operator+(obj); }
        void operator-=(const Matrix3& obj) { *this = operator-(obj); }
        void operator*=(float k) { *this = operator*(k); }
        void operator/=(float k) { *this = operator/(k); }

        bool operator==(const Matrix3& obj) const
        {
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    if (!Math::Near(value[i][j], obj[i][j])) return false;
            return true;
        }
        bool operator!=(const Matrix3& obj) const
        {
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    if (!Math::Near(value[i][j], obj[i][j])) return true;
            return false;
        }

        Vec3 operator*(const Vec3& obj) const
        {
            return Vec3(
                value[0][0] * obj.x + value[0][1] * obj.y + value[0][2] * obj.z,
                value[1][0] * obj.x + value[1][1] * obj.y + value[1][2] * obj.z,
                value[2][0] * obj.x + value[2][1] * obj.y + value[2][2] * obj.z
            );
        }
        Matrix3 operator*(const Matrix3& obj) const
        {
            Matrix3 res;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    res[i][j] = 
                        value[i][0] * obj[0][j] + value[i][1] * obj[1][j] + value[i][2] * obj[2][j];
            return res;
        }

        Vec3 getRow(unsigned i) const 
            { _ASSERT(i < 3 && "The index is out of bounds."); return Vec3(value[i][0], value[i][1], value[i][2]); }
        Vec3 getColumn(unsigned i) const 
            { _ASSERT(i < 3 && "The index is out of bounds."); return Vec3(value[0][i], value[1][i], value[2][i]); }

        float getDeterminant() const
        {
            return 
                value[0][0] * value[1][1] * value[2][2] +
                value[0][1] * value[1][2] * value[2][0] +
                value[0][2] * value[1][0] * value[2][1] -
                value[2][0] * value[1][1] * value[0][2] -
                value[1][0] * value[0][1] * value[2][2] -
                value[0][0] * value[2][1] * value[1][2];
        }

        void Transpose() { operator=(getTransposed()); }
        Matrix3 getTransposed() const
        {
            return Matrix3(
                value[0][0], value[1][0], value[2][0],
                value[0][1], value[1][1], value[2][1],
                value[0][2], value[1][2], value[2][2]
            );
        }
        void Inverse() { operator=(getInversed()); }
        Matrix3 getInversed() const { return getAdjoint() / Math::Abs(getDeterminant()); }
        Matrix3 getAdjoint() const
        {
            Matrix3 res;
            res[0][0] = value[1][1] * value[2][2] - value[1][2] * value[2][1];
            res[0][1] = value[1][2] * value[2][0] - value[1][0] * value[2][2];
            res[0][2] = value[1][0] * value[2][1] - value[1][1] * value[2][0];
            res[1][0] = value[0][2] * value[2][1] - value[0][1] * value[2][2];
            res[1][1] = value[0][0] * value[2][2] - value[0][2] * value[2][0];
            res[1][2] = value[0][1] * value[2][0] - value[0][0] * value[2][1];
            res[2][0] = value[0][1] * value[1][2] - value[0][2] * value[1][1];
            res[2][1] = value[0][2] * value[1][0] - value[0][0] * value[1][2];
            res[2][2] = value[0][0] * value[1][1] - value[0][1] * value[1][0];
            return res.getTransposed();
        }

        static Matrix3 Translate(const Vec2& vec) { return Translate(vec.x, vec.y); }
        static Matrix3 Translate(float x, float y)
        {
            return Matrix3(
                1, 0, 0,
                0, 1, 0,
                x, y, 1
            );
        }
        static Matrix3 Scale(const Vec2& vec) { return Scale(vec.x, vec.y); }
        static Matrix3 Scale(float x, float y)
        {
            return Matrix3(
                x, 0, 0,
                0, y, 0,
                0, 0, 1
            );
        }

        std::string toString() const
        {
            std::ostringstream os;
            os << "Matrix3 {";

            for (size_t i = 0; i < 3; i++)
            {
                os << "\n\t";
                for (size_t j = 0; j < 3; j++)
                {
                    if (j != 0) os << ", ";
                    os << value[i][j];
                }
            }
            
            os << "\n}";
            return os.str();
        }
    
    };

    /**
     * @brief 4 x 4 Matrix
     */
    class Matrix4
    {
    public:
        float value[4][4];

    public:
        Matrix4() { memset(value, 0, sizeof(value)); }
        Matrix4(float v[4][4])
        {
            value[0][0] = v[0][0]; value[0][1] = v[0][1]; value[0][2] = v[0][2]; value[0][3] = v[0][3];
            value[1][0] = v[1][0]; value[1][1] = v[1][1]; value[1][2] = v[1][2]; value[1][3] = v[1][3];
            value[2][0] = v[2][0]; value[2][1] = v[2][1]; value[2][2] = v[2][2]; value[2][3] = v[2][3];
            value[3][0] = v[3][0]; value[3][1] = v[3][1]; value[3][2] = v[3][2]; value[3][3] = v[3][3];
        }
        Matrix4(float _00, float _01, float _02, float _03,
                float _10, float _11, float _12, float _13,
                float _20, float _21, float _22, float _23,
                float _30, float _31, float _32, float _33)
        {
            value[0][0] = _00; value[0][1] = _01; value[0][2] = _02; value[0][3] = _03;
            value[1][0] = _10; value[1][1] = _11; value[1][2] = _12; value[1][3] = _13;
            value[2][0] = _20; value[2][1] = _21; value[2][2] = _22; value[2][3] = _23;
            value[3][0] = _30; value[3][1] = _31; value[3][2] = _32; value[3][3] = _33;
        }
        static Matrix4 Zero() { return Matrix4(); }
        static Matrix4 Identity() 
        {
            return Matrix4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        bool isZero() const { return operator==(Zero()); }
        bool isIdentity() const { return operator==(Identity()); }
        
        float* operator[](unsigned i) { _ASSERT(i < 4 && "The index is out of bounds."); return value[i]; }
        const float* operator[](unsigned i) const { _ASSERT(i < 4 && "The index is out of bounds."); return value[i]; }

        Matrix4 operator+(const Matrix4& obj) const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = value[i][j] + obj[i][j];
            return res;
        }
        Matrix4 operator-(const Matrix4& obj) const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = value[i][j] - obj[i][j];
            return res;
        }
        Matrix4 operator-() const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = -value[i][j];
            return res;
        }
        Matrix4 operator*(float k) const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = value[i][j] * k;
            return res;
        }
        friend Matrix4 operator*(float k, const Matrix4& obj) { return obj * k; }
        Matrix4 operator/(float k) const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = value[i][j] / k;
            return res;
        }
        
        void operator+=(const Matrix4& obj) { *this = operator+(obj); }
        void operator-=(const Matrix4& obj) { *this = operator-(obj); }
        void operator*=(float k) { *this = operator*(k); }
        void operator/=(float k) { *this = operator/(k); }

        bool operator==(const Matrix4& obj) const
        {
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    if (!Math::Near(value[i][j], obj[i][j])) return false;
            return true;
        }
        bool operator!=(const Matrix4& obj) const
        {
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    if (!Math::Near(value[i][j], obj[i][j])) return true;
            return false;
        }

        Vec4 operator*(const Vec4& obj) const
        {
            return Vec4(
                value[0][0] * obj.x + value[0][1] * obj.y + value[0][2] * obj.z + value[0][3] * obj.w,
                value[1][0] * obj.x + value[1][1] * obj.y + value[1][2] * obj.z + value[1][3] * obj.w,
                value[2][0] * obj.x + value[2][1] * obj.y + value[2][2] * obj.z + value[2][3] * obj.w,
                value[3][0] * obj.x + value[3][1] * obj.y + value[3][2] * obj.z + value[3][3] * obj.w
            );
        }
        Matrix4 operator*(const Matrix4& obj) const
        {
            Matrix4 res;
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    res[i][j] = 
                        value[i][0] * obj[0][j] + value[i][1] * obj[1][j] + value[i][2] * obj[2][j] + value[i][3] * obj[3][j];
            return res;
        }

        Vec4 getRow(unsigned i) const 
            { _ASSERT(i < 4 && "The index is out of bounds."); return Vec4(value[i][0], value[i][1], value[i][2], value[i][3]); }
        Vec4 getColumn(unsigned i) const 
            { _ASSERT(i < 4 && "The index is out of bounds."); return Vec4(value[0][i], value[1][i], value[2][i], value[3][i]); }

        float getDeterminant() const
        {
            return 
                value[0][0] * value[1][1] * value[2][2] * value[3][3] - value[0][0] * value[1][1] * value[2][3] * value[3][2] -
                value[0][0] * value[1][2] * value[2][1] * value[3][3] + value[0][0] * value[1][2] * value[2][3] * value[3][1] +

                value[0][0] * value[1][3] * value[2][1] * value[3][2] - value[0][0] * value[1][3] * value[2][2] * value[3][1] -
                value[0][1] * value[1][0] * value[2][2] * value[3][3] + value[0][1] * value[1][0] * value[2][3] * value[3][2] +

                value[0][1] * value[1][2] * value[2][0] * value[3][3] - value[0][1] * value[1][2] * value[2][3] * value[3][0] -
                value[0][1] * value[1][3] * value[2][0] * value[3][2] + value[0][1] * value[1][3] * value[2][3] * value[3][0] +

                value[0][2] * value[1][0] * value[2][1] * value[3][3] - value[0][2] * value[1][0] * value[2][3] * value[3][1] -
                value[0][2] * value[1][1] * value[2][0] * value[3][3] + value[0][2] * value[1][1] * value[2][3] * value[3][0] +

                value[0][2] * value[1][3] * value[2][0] * value[3][1] - value[0][2] * value[1][3] * value[2][1] * value[3][0] -
                value[0][3] * value[1][0] * value[2][1] * value[3][2] + value[0][3] * value[1][0] * value[2][2] * value[3][1] +

                value[0][3] * value[1][1] * value[2][0] * value[3][2] - value[0][3] * value[1][1] * value[2][2] * value[3][0] -
                value[0][3] * value[1][2] * value[2][0] * value[3][1] + value[0][3] * value[1][2] * value[2][1] * value[3][0];
        }

        void Transpose() { operator=(getTransposed()); }
        Matrix4 getTransposed() const
        {
            return Matrix4(
                value[0][0], value[1][0], value[2][0], value[3][0],
                value[0][1], value[1][1], value[2][1], value[3][1],
                value[0][2], value[1][2], value[2][2], value[3][2],
                value[0][3], value[1][3], value[2][3], value[3][3]
            );
        }
        void Inverse() { operator=(getInversed()); }
        Matrix4 getInversed() const { return getAdjoint() / Math::Abs(getDeterminant()); }
        Matrix4 getAdjoint() const
        {
            float _00 = Matrix3(value[1][1], value[1][2], value[1][3], value[2][1], value[2][2], value[2][3], value[3][1], value[3][2], value[3][3]).getDeterminant();
            float _01 = Matrix3(value[1][1], value[1][2], value[1][3], value[2][1], value[2][2], value[2][3], value[3][1], value[3][2], value[3][3]).getDeterminant();
            float _02 = Matrix3(value[1][0], value[1][1], value[1][3], value[2][0], value[2][1], value[2][3], value[3][0], value[3][1], value[3][3]).getDeterminant();
            float _03 = Matrix3(value[1][0], value[1][1], value[1][2], value[2][0], value[2][1], value[2][2], value[3][0], value[3][1], value[3][2]).getDeterminant();
            float _10 = Matrix3(value[0][1], value[0][2], value[0][3], value[2][1], value[2][2], value[2][3], value[3][1], value[3][2], value[3][3]).getDeterminant();
            float _11 = Matrix3(value[0][0], value[0][2], value[0][3], value[2][0], value[2][2], value[2][3], value[3][0], value[3][2], value[3][3]).getDeterminant();
            float _12 = Matrix3(value[0][0], value[0][1], value[0][3], value[2][0], value[2][1], value[2][3], value[3][0], value[3][1], value[3][3]).getDeterminant();
            float _13 = Matrix3(value[0][0], value[0][1], value[0][2], value[2][0], value[2][1], value[2][2], value[3][0], value[3][1], value[3][2]).getDeterminant();
            float _20 = Matrix3(value[0][1], value[0][2], value[0][3], value[1][1], value[1][2], value[1][3], value[3][1], value[3][2], value[3][3]).getDeterminant();
            float _21 = Matrix3(value[0][0], value[0][2], value[0][3], value[1][0], value[1][2], value[1][3], value[3][0], value[3][2], value[3][3]).getDeterminant();
            float _22 = Matrix3(value[0][0], value[0][1], value[0][3], value[1][0], value[1][1], value[1][3], value[3][0], value[3][1], value[3][3]).getDeterminant();
            float _23 = Matrix3(value[0][0], value[0][1], value[0][2], value[1][0], value[1][1], value[1][2], value[3][0], value[3][1], value[3][2]).getDeterminant();
            float _30 = Matrix3(value[0][1], value[0][2], value[0][3], value[1][1], value[1][2], value[1][3], value[2][1], value[2][2], value[2][3]).getDeterminant();
            float _31 = Matrix3(value[0][0], value[0][2], value[0][3], value[1][0], value[1][2], value[1][3], value[2][0], value[2][2], value[2][3]).getDeterminant();
            float _32 = Matrix3(value[0][0], value[0][1], value[0][3], value[1][0], value[1][1], value[1][3], value[2][0], value[2][1], value[2][3]).getDeterminant();
            float _33 = Matrix3(value[0][0], value[0][1], value[0][2], value[1][0], value[1][1], value[1][2], value[2][0], value[2][1], value[2][2]).getDeterminant();

            Matrix4 res(
                _00, -_01, _02, -_03,
                -_10, _11, -_12, _13,
                _20, -_21, _22, -_23,
                -_30, _31, -_32, _33
            );
            return res.getTransposed();
        }

        static Matrix4 Translate(const Vec3& vec) { return Translate(vec.x, vec.y, vec.z); }
        static Matrix4 Translate(float x, float y, float z)
        {
            return Matrix4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1
            );
        }
        static Matrix4 Scale(const Vec3& vec) { return Scale(vec.x, vec.y, vec.z); }
        static Matrix4 Scale(float x, float y, float z)
        {
            return Matrix4(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
            );
        }
        static Matrix4 RotateByXAxis(float rad)
        {
            return Matrix4(
                1, 0, 0, 0,
                0, Math::Cos(rad), Math::Sin(rad), 0,
                0, -Math::Sin(rad), Math::Cos(rad), 0,
                0, 0, 0, 1
            );
        }
        static Matrix4 RotateByYAxis(float rad)
        {
            return Matrix4(
                Math::Cos(rad), 0, -Math::Sin(rad), 0,
                0, 1, 0, 0,
                Math::Sin(rad), 0, Math::Cos(rad), 0,
                0, 0, 0, 1
            );
        }
        static Matrix4 RotateByZAxis(float rad)
        {
            return Matrix4(
                Math::Cos(rad), Math::Sin(rad), 0, 0,
                -Math::Sin(rad), Math::Cos(rad), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }
        static Matrix4 RotateByAxis(const Vec3& rotation) { return RotateByAxis(rotation.x, rotation.y, rotation.z); }
        static Matrix4 RotateByAxis(float x, float y, float z)
            { return RotateByXAxis(x) * RotateByYAxis(y) * RotateByZAxis(z); }
        static Matrix4 Orthogonal(const Vec3& vec)
        {
            // TODO: Orthogonal Matrix4
            return Identity();
        }
        static Matrix4 Perspective(const Vec3& vec)
        {
            // TODO: Perspective Matrix4
            return Identity();
        }
        static Matrix4 LookAt(const Vec3& origin, const Vec3& target, const Vec3& up)
        {
            Vec3 z = (target - origin).getNormalized();
            Vec3 x = Vec3::Cross(up.getNormalized(), z);
            Vec3 y = Vec3::Cross(z, x);
            
            return Matrix4(
                x.x, y.x, z.x, 0,
                x.y, y.y, z.y, 0,
                x.z, y.z, z.z, 0,
                -Vec3::Dot(x, origin), -Vec3::Dot(y, origin), -Vec3::Dot(z, origin), 1
            );
        }

        std::string toString() const
        {
            std::ostringstream os;
            os << "Matrix4 {";

            for (size_t i = 0; i < 4; i++)
            {
                os << "\n\t";
                for (size_t j = 0; j < 4; j++)
                {
                    if (j != 0) os << ", ";
                    os << value[i][j];
                }
            }
            
            os << "\n}";
            return os.str();
        }
    
    };

    /**
     * @brief Transformation of object including position, rotation and scaling.
     */
    class Transform
    {
    public:
        Vec3 position, rotation, scaling;

    public:
        Transform(
            const Vec3& position = Vec3::Zero(), const Vec3& rotation = Vec3::Zero(), const Vec3& scaling = Vec3::Identity())
            : position(position), rotation(rotation), scaling(scaling) {}
        static Transform Default() { return Transform(); }

        Matrix4 getMatrix() const
        {
            Vec3 rotationRad(Math::toRad(rotation.x), Math::toRad(rotation.y), Math::toRad(rotation.z));
            return Matrix4::Translate(position) * Matrix4::RotateByAxis(rotationRad) * Matrix4::Scale(scaling); 
        }

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_GMATH__
