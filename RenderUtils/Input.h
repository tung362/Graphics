#pragma once

class Input
{
public:
	enum KEY_STATE { UP, DOWN, PRESS, RELEASE };

private:
	struct  GLFWwindow *winHandle;
	KEY_STATE keys[350];

	double mouseX, mouseY;
	float mouseHorz, mouseVert;

public:
	bool Init(const class Window &);
	bool Step();
	bool Term();

	KEY_STATE GetKeyState(int key) const
	{
		return keys[key];
	}

	float GetMouseAxisHorizontal() const
	{
		return mouseHorz;
	}

	float GetMouseAxisVertical() const
	{
		return mouseVert;
	}
};