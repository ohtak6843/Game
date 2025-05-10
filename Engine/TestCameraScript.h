#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 300.f;
	
	Vec2		_mousePos = { 0.f, 0.f };
};

