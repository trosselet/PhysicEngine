#include "RotateMesh.h"

#include <Engine/Header/GameManager.h>

RotateMesh::RotateMesh()
{
    rotationSpeed = 1.f;
}

RotateMesh::~RotateMesh()
{
}

void RotateMesh::OnStart()
{
}

void RotateMesh::OnUpdate()
{
}

void RotateMesh::OnFixedUpdate()
{
    m_pOwner->transform.RotateYPR(Vector3{ (float)rotationSpeed, 0.0f, 0.0f }.ToXMFLOAT3());
    //m_pOwner->Destroy();
}