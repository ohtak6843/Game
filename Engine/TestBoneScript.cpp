#include "pch.h"
#include "TestBoneScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "Timer.h"
#include "Input.h"

TestBoneScript::TestBoneScript()
{
	_baseRotation.x = DegreeToRadian(_baseRotation.x);
	_baseRotation.y = DegreeToRadian(_baseRotation.y);
	_baseRotation.z = DegreeToRadian(_baseRotation.z);
}

TestBoneScript::~TestBoneScript()
{
}

void TestBoneScript::LateUpdate()
{
	//if (INPUT->GetButton(KEY_TYPE::R))
	//{
	//	_baseRotation.x += DELTA_TIME * 30.0f;
	//}

	//if (INPUT->GetButton(KEY_TYPE::T))
	//{
	//	_baseRotation.x -= DELTA_TIME * 30.0f;
	//}

	//if (INPUT->GetButton(KEY_TYPE::F))
	//{
	//	_baseRotation.y += DELTA_TIME * 30.0f;
	//}

	//if (INPUT->GetButton(KEY_TYPE::G))
	//{
	//	_baseRotation.y -= DELTA_TIME * 30.0f;
	//}

	//if (INPUT->GetButton(KEY_TYPE::V))
	//{
	//	_baseRotation.z += DELTA_TIME * 30.0f;
	//}

	//if (INPUT->GetButton(KEY_TYPE::B))
	//{
	//	_baseRotation.z -= DELTA_TIME * 30.0f;
	//}

	if (_parentObject == nullptr)
		return;

	int32 rightHandBoneIndex = _parentObject->GetMeshRenderer()->GetMesh()->GetRightHandBoneIndex();
	Matrix rightHandBoneMatrix = _parentObject->GetAnimator()->GetBoneMatrix(rightHandBoneIndex);
	Matrix parentMatrix = _parentObject->GetTransform()->GetLocalToWorldMatrix();

	rightHandBoneMatrix = rightHandBoneMatrix * parentMatrix;

	// Base 행렬 계산
	Matrix matScale = Matrix::CreateScale(_baseScale);

	SimpleMath::Quaternion q;

	float sp = sinf(_baseRotation.x * 0.5f);
	float cp = cosf(_baseRotation.x * 0.5f);

	float sy = sinf(_baseRotation.y * 0.5f);
	float cy = cosf(_baseRotation.y * 0.5f);

	float sr = sinf(_baseRotation.z * 0.5f);
	float cr = cosf(_baseRotation.z * 0.5f);

	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * sp * cr + sy * cp * sr;
	q.y = sy * cp * cr - cy * sp * sr;
	q.z = cy * cp * sr - sy * sp * cr;

	Matrix matRotation = Matrix::CreateFromQuaternion(q);
	Matrix matTranslation = Matrix::CreateTranslation(_basePosition);

	Matrix matLocal = matScale * matRotation * matTranslation;

	matLocal *= rightHandBoneMatrix;

	Vec3 scale{};
	Vec3 rotation{};
	Vec3 translation{};
	SimpleMath::Quaternion orientation{};
	matLocal.Decompose(scale, orientation, translation);

	rotation = Transform::QuaternionToEuler(orientation);
	rotation.x = RadianToDegree(rotation.x);
	rotation.y = RadianToDegree(rotation.y);
	rotation.z = RadianToDegree(rotation.z);

	static float updateTime;
	updateTime += DELTA_TIME;
	if (updateTime >= 1.0f)
	{
		//std::cout << "Scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;
		//std::cout << "Orientation: " << orientation.x << ", " << orientation.y << ", " << orientation.z << ", " << orientation.w << std::endl;
		//std::cout << "Traslation: " << translation.x << ", " << translation.y << ", " << translation.z << std::endl;
		std::cout << "Rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;
		//std::cout << "Transform: " << _basePosition.x << ", " << _basePosition.y << ", " << _basePosition.z << endl;

		updateTime = 0.0f;
	}

	GetTransform()->SetLocalScale(scale);
	GetTransform()->SetLocalRotation(rotation);
	GetTransform()->SetLocalPosition(translation);
}
