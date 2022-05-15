#ifndef __IMMGRAPHICS_HEADER_CAMERA__
#define __IMMGRAPHICS_HEADER_CAMERA__

#include "gmath.h"

namespace ImmGraphics
{
	class Camera
	{
	public:
		Vec3 Position;
		Vec3 Front;
		Vec3 Up;
		Vec3 Right;
		Vec3 WorldUp;

		float Yaw;
		float Pitch;

		float Zoom;

	public:
		Camera(Vec3 position = Vec3::Zero(), Vec3 up = Vec3::Up(), float yaw = -90, float pitch = 0, float zoom = 45)
			: Front(Vec3::Forward()), Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), Zoom(zoom)
		{
			Update();
		}

		Matrix4 getViewMatrix()
		{
			return Matrix4::View(Position, Position + Front, Up);
		}

		void Update()
		{
			// calculate the new Front vector
			Vec3 front;
			front.x = Math::Cos(Yaw * Math::RAD) * Math::Cos(Pitch * Math::RAD);
			front.y = Math::Sin(Pitch * Math::RAD);
			front.z = Math::Sin(Yaw * Math::RAD) * Math::Cos(Pitch * Math::RAD);
			Front = front.getNormalized();
			// also re-calculate the Right and Up vector
			Right = (Vec3::Cross(Front, WorldUp)).getNormalized();
			Up = (Vec3::Cross(Right, Front)).getNormalized();
		}

	};

}

#endif // !__IMMGRAPHICS_HEADER_CAMERA__
