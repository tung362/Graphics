#include "Input.h"
#include "Timer.h"
#include "Camera.h"

void FlyCamera::Update(const Input &in, const Time &t)
{
	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	glm::vec3 right = glm::cross(direction, glm::vec3(0, 1, 0));
	glm::vec3 up = glm::cross(right, direction);

	yaw -= in.GetMouseAxisHorizontal() * t.GetDeltaTime();
	pitch += in.GetMouseAxisVertical() * t.GetDeltaTime();

	direction = glm::vec3(glm::rotate(yaw, glm::vec3(0, 1, 0))[2]);
	right = glm::cross(direction, glm::vec3(0, 1, 0));

	direction = glm::mat3(glm::rotate(pitch, right)) * direction;

	if (in.GetKeyState('W') == Input::DOWN) moveDir += direction;
	if (in.GetKeyState('S') == Input::DOWN) moveDir -= direction;
	if (in.GetKeyState('A') == Input::DOWN) moveDir -= right;
	if (in.GetKeyState('D') == Input::DOWN) moveDir += right;
	if (in.GetKeyState('E') == Input::DOWN) moveDir -= up;
	if (in.GetKeyState('Q') == Input::DOWN) moveDir += up;

	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.GetDeltaTime() * speed;
	}
}
