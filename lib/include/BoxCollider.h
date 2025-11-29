#ifndef BOXCOLLIDER_INCLUDED__H
#define BOXCOLLIDER_INCLUDED__H

#include <Engine/Header/IScript.h>
#include <Tools/Header/PrimitiveTypes.h>

class BoxCollider : public IScript
{
public:
    BoxCollider(std::vector<BoxCollider*>* pColliders);
    ~BoxCollider();

    void OnStart() override {}
    void OnUpdate() override {}
    void OnFixedUpdate() override {};

	void UpdateCollider();

    bool IsColliding(const BoxCollider& other);
    void OnCollisionEnter(BoxCollider& other);
    void OnCollisionStay(BoxCollider& other);
    void OnCollisionExit(BoxCollider& other);

    GameObject* GetOwner() const { return m_pOwner; }

    bool IsPointInside(const Vector3& point) const;

    int boxId = 0;

    bool m_isColliding = false;
    BoxCollider* m_lastCollider = nullptr;

    Vector3 m_min;
    Vector3 m_max;
private:


    std::vector<BoxCollider*>* m_pColliders;
};

#endif
