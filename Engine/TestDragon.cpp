#include "pch.h"
#include "TestDragon.h"
#include "Input.h"
#include "Animator.h"
#include "Transform.h"

void TestDragon::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex + 1) % count;
		GetAnimator()->Play(index);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_2))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(index);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_3))
	{
		GetTransform()->AddLocalRotation(Vec3(0.0f, 15.f, 0.0f));
	}

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_4))
	{
		GetTransform()->AddLocalRotation(Vec3(0.0f, -15.f, 0.0f));
	}
}