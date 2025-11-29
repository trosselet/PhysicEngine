#ifndef PARTICLE_INCLUDED__H
#define PARTICLE_INCLUDED__H

#include <Engine/Header/IScript.h>
#include "ChaoticAttractor.h"

class Particle : public IScript
{
public:
	Particle(float posx, float posy, float posz, float vx, float vy, float vz, float lt);
	Particle();
	~Particle() = default;

	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;

	void Update(float deltaTime, std::vector<Particle*>& allParticleInThread);

	float GetDistance(Particle const& other);
	float GetDistanceSquared(Particle const& other);

	GameObject* GetOwner() const { return m_pOwner; }
	float GetLifetime() const { return m_lifetime; }
	void SetLifetime(float lt) { m_lifetime = lt; }
	Vector3 GetPosition() const { return Vector3{ m_x, m_y, m_z }; }
	void SetPosition(const Vector3& pos) { m_x = pos.data[0];  m_y = pos.data[1];  m_z = pos.data[2]; }
	Vector3 GetVelocity() const { return Vector3{ m_velocityX, m_velocityY, m_velocityZ }; }
	void SetVelocity(const Vector3& vel)  {  m_velocityX = vel.data[0];  m_velocityY = vel.data[1];  m_velocityZ = vel.data[2]; }

	void UseChaoticAttractor(AttractorType type) { m_type = type; }

	std::vector<Particle*> const& GetNearbyParticles() const { return m_nearbyParticles; }

private:
	void UpdateNearbyParticles(const std::vector<Particle*>& allParticles, float maxDistance);
	void Move(float deltaTime);

private:
	float m_x;
	float m_y;
	float m_z;
		  
	float m_velocityX;
	float m_velocityY;
	float m_velocityZ;
		  
	float m_lifetime;

	ChaoticAttractor m_attractor;
	AttractorType m_type = AttractorType::None;

	std::vector<Particle*> m_nearbyParticles;
	int m_updateCounter = 0;
	static constexpr int updateFrequency = 5;

};


#endif // !PARTICLE_INCLUDED_H