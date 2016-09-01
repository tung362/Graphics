#pragma once

//#define GLM_SWIZZLE
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class FlyCamera
{
	glm::vec3 position;
	glm::vec3 direction;

	//float azimuth, altitude;
	float pitch, yaw, roll;

	//Make sure near is > 0
	float aspect, fov, near, far;
	float speed;

public:
	FlyCamera(float aspect = 16.f / 9.f, float fov = 45.f, float near = 1.f, float far = 999.f, float speed = 5)
		: aspect(aspect), fov(fov), near(near), far(far), direction(0, 0, 1), speed(speed), pitch(0), yaw(0) , roll(0){}

	void Update(const class Input &, const class Time &);

	void LookAt(const glm::vec3 target)
	{
		direction = glm::normalize(target - position);
	}

	void JumpTo(const glm::vec3 &location)
	{
		position = location;
	}

	glm::mat4 GetView() const
	{
		return glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	}

	glm::mat4 GetProjection() const
	{
		return glm::perspective(fov, aspect, near, far);
	}
};