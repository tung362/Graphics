#include "Input.h"
#include "Timer.h"
#include "Camera.h"

void FlyCamera::LookAt(const glm::vec3 &target)
{
	//auto mat =
	//glm::lookAt(position, normalize(target - position), glm::vec3(0, 1, 0));
	direction = normalize(target - position);


}

void FlyCamera::Update(const Input &in, const Time &t)
{

	yaw -= in.GetMouseAxisHorizontal() / 200.f;
	pitch -= in.GetMouseAxisVertical() / 200.f;

	pitch = glm::clamp<float>(pitch, -glm::pi<float>() / 2 + 0.0001f,
		glm::pi<float>() / 2 - 0.0001f);

	glm::mat4 &view = transform;
	view = glm::rotate(yaw, glm::vec3(0, 1, 0));
	view = glm::rotate(pitch, glm::vec3(view[0])) * view;
	view = glm::rotate(roll, glm::vec3(view[2])) * view;

	view = glm::eulerAngleYXZ(yaw, pitch, roll);
	view = glm::translate(-position) * view;

	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	if (in.GetKeyState('W') == Input::DOWN) moveDir += glm::vec3(view[2]);
	if (in.GetKeyState('S') == Input::DOWN) moveDir -= glm::vec3(view[2]);
	if (in.GetKeyState('A') == Input::DOWN) moveDir += glm::vec3(view[0]);
	if (in.GetKeyState('D') == Input::DOWN) moveDir -= glm::vec3(view[0]);
	if (in.GetKeyState('E') == Input::DOWN) moveDir -= glm::vec3(view[1]);
	if (in.GetKeyState('Q') == Input::DOWN) moveDir += glm::vec3(view[1]);

	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.GetDeltaTime() * speed;
	}
}
