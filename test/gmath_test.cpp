#include "gmath.h"
#include "debug.h"

using namespace std;
using namespace ImmGraphics;

int main()
{
    BEGIN_TEST(Vec3, Zero)
        CASE_STATIC(Vec3(0, 0, 0), Vec3::Zero)
        CASE_STATIC(Vec3(0, 0, 0.0000001), Vec3::Zero)
    END_TEST(Vec3, Zero)

    BEGIN_TEST(Vec3, Identity)
        CASE_STATIC(Vec3(1, 1, 1), Vec3::Identity)
        CASE_STATIC(Vec3(1, 1, 1.0000001), Vec3::Identity)
    END_TEST(Vec3, Identity)

    BEGIN_TEST(Vec3, isZero)
        CASE_RVALUE(Vec3(0, 0, 0), true, isZero)
        CASE_RVALUE(Vec3(0, 0, 0.0000001), true, isZero)
        CASE_RVALUE(Vec3(0, 0, 0.1), false, isZero)
    END_TEST(Vec3, isZero)

    BEGIN_TEST(Vec3, isNormalized)
        CASE_RVALUE(Vec3(1, 0, 0), true, isNormalized)
        CASE_RVALUE(Vec3(1, 0, 0.1), false, isNormalized)
        CASE_RVALUE(Vec3(0, 0, 0.1), false, isNormalized)
    END_TEST(Vec3, isNormalized)

    BEGIN_TEST(Vec3, Length)
        CASE_RVALUE(Vec3(0, 0, 0), 0, Length)
        CASE_RVALUE(Vec3(1, 0, 0), 1, Length)
        CASE_RVALUE(Vec3(0, 1, 0), 1, Length)
        CASE_RVALUE(Vec3(0, 0, 8), 8, Length)
    END_TEST(Vec3, Length)

    BEGIN_TEST(Vec3, )

    END_TEST(Vec3, Length)

    system("pause");

    return 0;
}
