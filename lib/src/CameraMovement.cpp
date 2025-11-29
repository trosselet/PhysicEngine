#include "CameraMovement.h"

#include <Engine/Header/GameManager.h>

CameraMovement::CameraMovement()
{
    cameraSpeed = 50;
}

CameraMovement::~CameraMovement()
{
}

void CameraMovement::OnStart()
{
}

void CameraMovement::OnUpdate()
{
    
}

void CameraMovement::OnFixedUpdate()
{
    bool Z = GetAsyncKeyState('Z') < 0;
    bool Q = GetAsyncKeyState('Q') < 0;
    bool S = GetAsyncKeyState('S') < 0;
    bool D = GetAsyncKeyState('D') < 0;
    bool A = GetAsyncKeyState('A') < 0;
    bool E = GetAsyncKeyState('E') < 0;

    bool LEFT = GetAsyncKeyState(VK_LEFT) < 0;
    bool RIGHT = GetAsyncKeyState(VK_RIGHT) < 0;

    Vector3 forward = m_pOwner->transform.Forward();
    Vector3 right = m_pOwner->transform.Right();
    Vector3 up = m_pOwner->transform.Up();

    float moveAmount = cameraSpeed * GameManager::GetFixedDeltaTime();

    if (Z)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ forward.data[0] * 1.0f, forward.data[1] * 1.0f, forward.data[2] * 1.0f }.ToXMFLOAT3());
    }
    if (Q)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ -right.data[0] * 1.f, -right.data[1] * 1.f, -right.data[2] * 1.f }.ToXMFLOAT3());
    }
    if (S)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ -forward.data[0] * 1.f, -forward.data[1] * 1.f, -forward.data[2] * 1.f }.ToXMFLOAT3());
    }
    if (D)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ right.data[0] * 1.f, right.data[1] * 1.f, right.data[2] * 1.f }.ToXMFLOAT3());
    }
    if (A)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ 0, -up.data[1] * 1.f, 0 }.ToXMFLOAT3());
    }
    if (E)
    {
        m_pOwner->transform.OffsetPosition(Vector3{ 0, up.data[1] * 1.f, 0 }.ToXMFLOAT3());
    }

    if (LEFT)
    {
        m_pOwner->transform.RotateYPR(Vector3{ -0.06f, 0.0f, 0.0f }.ToXMFLOAT3());
    }

    if (RIGHT)
    {
        m_pOwner->transform.RotateYPR(Vector3{ 0.06f, 0.0f, 0.0f }.ToXMFLOAT3());
    }
}