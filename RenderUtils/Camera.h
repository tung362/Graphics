#pragma once

#define GLM_SWIZZLE
#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

class FlyCamera
{
public:
	glm::vec3 position;
	glm::vec3 direction;

	glm::mat4 transform;

	//float azimuth, altitude;
	// YAW is along the Y
	// PITCH is along the X
	// ROLL is along the Z
	float pitch, yaw, roll;

	// make sure near is > 0
	float aspect, fov, near, far;

	float speed;

public:
	FlyCamera(float a_aspect = 16.f / 9.f, float a_fov = 45.f, float a_near = 1.f, float a_far = 100.f)
		: aspect(a_aspect), fov(a_fov), near(a_near), far(a_far),
		direction(0, 0, 1), speed(20), pitch(0), yaw(0), roll(0)
	{

	}

	void Update(const class Input &, const class Time &);

	void LookAt(const glm::vec3 &target);

	void JumpTo(const glm::vec3 &location)
	{
		position = location;
	}

	glm::mat4 GetView() const
	{
		return glm::inverse(transform);
	}

	glm::mat4 GetProjection() const
	{
		return glm::perspective(fov, aspect, near, far);
	}
};