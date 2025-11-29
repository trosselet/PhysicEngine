#include "BoxCollider.h"
#include <Engine/Header/GameManager.h>

#include <algorithm>
#undef min
#undef max

static int globalID = 0;

BoxCollider::BoxCollider(std::vector<BoxCollider*>* pColliders)
{
    boxId = globalID++;
    m_pColliders = pColliders;
}

BoxCollider::~BoxCollider()
{
    
}

void BoxCollider::UpdateCollider()
{
    Vector3 pos = m_pOwner->transform.GetPosition();
    Vector3 scale = m_pOwner->transform.GetScaling() * 0.5f;

    m_min = pos - scale;
    m_max = pos + scale;

    for (BoxCollider* other : *m_pColliders)
    {
        if (other == this) continue;

        bool isColliding = IsColliding(*other);

        if (isColliding)
        {
            if (!m_isColliding)       
                OnCollisionEnter(*other);
            else                      
                OnCollisionStay(*other);

            m_isColliding = true;
            m_lastCollider = other;
        }
        else
        {
            if (m_isColliding && m_lastCollider == other)
            {
                OnCollisionExit(*other);
                m_isColliding = false;
                m_lastCollider = nullptr;
            }
        }
    }
}

bool BoxCollider::IsColliding(const BoxCollider& other)
{
    return !(
        m_max.data[0] < other.m_min.data[0] ||
        m_min.data[0] > other.m_max.data[0] ||
        m_max.data[1] < other.m_min.data[1] ||
        m_min.data[1] > other.m_max.data[1] ||
        m_max.data[2] < other.m_min.data[2] ||
        m_min.data[2] > other.m_max.data[2]
        );
}

void BoxCollider::OnCollisionEnter(BoxCollider& other)
{
    //std::cout << "Enter: " << boxId << " with " << other.boxId << std::endl;
}

void BoxCollider::OnCollisionStay(BoxCollider& other)
{

}

void BoxCollider::OnCollisionExit(BoxCollider& other)
{
    //std::cout << "Exit: " << boxId << " with " << other.boxId << std::endl;
}

bool BoxCollider::IsPointInside(const Vector3& point) const
{
    return point.data[0] >= std::min(m_min.data[0], m_max.data[0]) &&
        point.data[0] <= std::max(m_min.data[0], m_max.data[0]) &&
        point.data[1] >= std::min(m_min.data[1], m_max.data[1]) &&
        point.data[1] <= std::max(m_min.data[1], m_max.data[1]) &&
        point.data[2] >= std::min(m_min.data[2], m_max.data[2]) &&
        point.data[2] <= std::max(m_min.data[2], m_max.data[2]);
}
