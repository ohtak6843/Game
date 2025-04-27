#include "pch.h"
#include "Input.h"
#include "Engine.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hInst = (HINSTANCE)::GetWindowLongPtr(_hwnd, GWLP_HINSTANCE);
	_mouseStates.resize(3, KEY_STATE::NONE);
	_keyStates.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);

	// DirectInput �ʱ�ȭ
	HRESULT hr = S_OK;
	if (FAILED(hr = DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(&_directInput),
		nullptr)))
	{
		return;
	}

	// Ű���� Direct Input ����
	if (FAILED(hr = _directInput->CreateDevice(GUID_SysKeyboard,
		&_keyboard,
		nullptr)))
	{
		return;
	}

	if (FAILED(hr = _keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return;
	}

	if (FAILED(hr = _keyboard->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return;
	}

	if (FAILED(hr = _keyboard->Acquire()))
	{
		return;
	}

	// ���콺 Direct Input ����
	if (FAILED(hr = _directInput->CreateDevice(GUID_SysMouse,
		&_mouse,
		nullptr)))
	{
		return;
	}

	if (FAILED(hr = _mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return;
	}

	if (FAILED(hr = _mouse->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return;
	}

	if (FAILED(hr = _mouse->Acquire()))
	{
		return;
	}
}

void Input::Shutdown()
{
	// ���콺 ��ȯ
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = nullptr;
	}

	// Ű���� ��ȯ
	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = nullptr;
	}

	// DirectInput ��ȯ
	if (_directInput)
	{
		_directInput->Release();
		_directInput = nullptr;
	}
}

void Input::Update()
{
	HRESULT hr;
	if (_mouse == nullptr || _keyboard == nullptr) return;

	if (FAILED(hr = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), &_DIMouseState)))
	{
		while (_mouse->Acquire() == DIERR_INPUTLOST);
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (FAILED(hr = _keyboard->GetDeviceState(KEY_TYPE_COUNT, &asciiKeys)))
	{
		while (_keyboard->Acquire() == DIERR_INPUTLOST);
	}

	BYTE mouseButtons[3];
	for (uint32 i = 0; i < 3; i++)
	{
		mouseButtons[i] = _DIMouseState.rgbButtons[i];
	}
	for (uint32 i = 0; i < 3; i++) {
		// Ű�� ���� ������ true
		if (mouseButtons[i] & 0x80)
		{
			KEY_STATE& state = _mouseStates[i];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _mouseStates[i];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}

	_mousePos.x = _DIMouseState.lX;
	_mousePos.y = _DIMouseState.lY;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _keyStates[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _keyStates[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(GEngine->GetWindow().hwnd, &_mousePos);
}