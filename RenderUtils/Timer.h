#pragma once

class Time
{
	float currentTime;
	float previousTime;
	float deltaTime;
public:
	bool Init();
	bool Step();
	bool Term();

	float GetDeltaTime() const;
	float GetTotalTime() const;
	float GetRealTime() const;
};