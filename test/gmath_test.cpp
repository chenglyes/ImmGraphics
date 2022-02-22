#include "gmath.h"
#include <iostream>

using namespace std;
using namespace ImmGraphics;

int main()
{
    cout << Math::Sign(-1.0f) << endl;
    cout << "==============================" << endl;

    Vec3 vector3;
    cout << vector3.toString() << endl;
    cout << vector3.isZero() << endl;

    vector3[0] = 1;
    vector3[1] = 1;
    vector3[2] = 1;

    cout << vector3.toString() << endl;
    cout << vector3.isIdentity() << endl;

    cout << "==============================" << endl;

    cout << Vec3::Zero().toString() << endl;
    cout << Vec3::Identity().toString() << endl;
    cout << Vec3::Up().toString() << endl;
    cout << Vec3::Down().toString() << endl;
    cout << Vec3::Left().toString() << endl;
    cout << Vec3::Right().toString() << endl;
    cout << Vec3::Forward().toString() << endl;
    cout << Vec3::Back().toString() << endl;
    cout << Vec3::PositiveInfinity().toString() << endl;
    cout << Vec3::NegativeInfinity().toString() << endl;

    cout << "===============================" << endl;

    cout << Vec3::Distance(Vec3::Zero(), vector3) << endl;
    cout << Vec3::Dot(Vec3(1, 2, 3), vector3) << endl;
    cout << Vec3::Cross(Vec3::Up(), Vec3::Right()).toString() << endl;
    cout << Vec3::Angle(Vec3::Up(), Vec3::Down()) << endl;
    
    vector3.Normalize();
    cout << vector3.Length() << endl;
    cout << vector3.isNormalized() << endl;
    cout << vector3.toString() << endl;

    cout << "===============================" << endl;
    cout << vector3.Lerp(Vec3::Zero(), vector3, 0.5f).toString() << endl;
    cout << vector3.getReflected(Vec3::Up()).toString() << endl;

    cout << "===============================" << endl;
    cout << Matrix4().toString() << endl;
    cout << Matrix4().Identity().toString() << endl;
    cout << Matrix4().Identity().getDeterminant() << endl;
    cout << Matrix4().Identity().getAdjoint().toString() << endl;
    cout << Matrix4().Identity().getInversed().toString() << endl;
    cout << Matrix4().Identity().getTransposed().toString() << endl;
    cout << Matrix4().Identity().getAdjoint().toString() << endl;
    cout << Matrix4().Identity().getRow(1).toString() << endl;
    cout << Matrix4().Identity().getColumn(1).toString() << endl;
    cout << (Matrix4::Identity() * vector3).toString() << endl;

    system("pause");

    return 0;
}
