#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent ����
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

	static Vec3 QuaternionToEuler(const Vec4& quat)
	{
		Vec3 euler;

		// Roll (X-axis rotation)
		float t0 = +2.0f * (quat.w * quat.x + quat.y * quat.z);
		float t1 = +1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
		euler.x = std::atan2(t0, t1);

		// Pitch (Y-axis rotation)
		float t2 = +2.0f * (quat.w * quat.y - quat.z * quat.x);
		t2 = std::clamp(t2, -1.0f, 1.0f);
		euler.y = std::asin(t2);

		// Yaw (Z-axis rotation)
		float t3 = +2.0f * (quat.w * quat.z + quat.x * quat.y);
		float t4 = +1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
		euler.z = std::atan2(t3, t4);

		return euler;
	}

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent ����
	Vec3 _localPosition;
	Vec3 _localRotation;
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};
	Matrix _matWorld = {};

	weak_ptr<Transform> _parent;
};

