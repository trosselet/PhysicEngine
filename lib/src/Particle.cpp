#include "Particle.h"
#include <cmath>
Particle::Particle(float posx, float posy, float posz, float vx, float vy, float vz, float lt) :
	m_x(posx),
	m_y(posy),
	m_z(posz),
	m_velocityX(vx),
	m_velocityY(vy),
	m_velocityZ(vz),
	m_lifetime(lt)
{
}

Particle::Particle()
{
	m_x = rand() % 10 - 5;
	m_y = rand() % 10 - 5;
	m_z = rand() % 10 - 5;

	float min = -10.f;
	float max =  10.f;

	/*m_velocityX = 2.1f + (4.7f - 2.1f) * ((double)rand() / (double)RAND_MAX);
	m_velocityY = 2.1f + (4.7f - 2.1f) * ((double)rand() / (double)RAND_MAX);
	m_velocityZ = 2.1f + (4.7f - 2.1f) * ((double)rand() / (double)RAND_MAX);*/

	m_velocityX = min + (max - min) * ((double)rand() / (double)RAND_MAX);
	m_velocityY = min + (max - min) * ((double)rand() / (double)RAND_MAX);
	m_velocityZ = min + (max - min) * ((double)rand() / (double)RAND_MAX);

	//m_lifetime = 5.0f + (10.0f - 5.0f) * ((double)rand() / (double)RAND_MAX);
	m_lifetime = 1000.0f;	
}


void Particle::OnStart()
{
	

}

void Particle::OnUpdate()
{

}

void Particle::OnFixedUpdate()
{
	//Update();
}

void Particle::Update(float deltaTime, std::vector<Particle*>& allParticleInThread)
{
	m_updateCounter++;
	if (m_updateCounter >= updateFrequency)
	{
		m_updateCounter = 0;
		UpdateNearbyParticles(allParticleInThread, m_pOwner->transform.GetScalingFLOAT().x);
	}

	if (m_lifetime <= 0)
	{
		m_pOwner->Deactivate();
		return;
	}

	m_lifetime -= 0.05f;

	

	if (m_type == AttractorType::Lorenz)
	{
		Vector3 pos = GetPosition();

		m_attractor.UpdateLorenz(pos, 0.01f);

		SetPosition(pos);
	}
	else if(m_type == AttractorType::Thomas)
	{
		Vector3 pos = GetPosition();

		m_attractor.UpdateThomas(pos, 0.01f);

		SetPosition(pos);
	}
	else if(m_type == AttractorType::Rossler)
	{
		Vector3 pos = GetPosition();

		m_attractor.UpdateRossler(pos, 0.01f);

		SetPosition(pos);
	}
	else if(m_type == AttractorType::Aizawa)
	{
		Vector3 pos = GetPosition();

		m_attractor.UpdateAizawa(pos, 0.01f);

		SetPosition(pos);
	}
	else if(m_type == AttractorType::Chen)
	{
		Vector3 pos = GetPosition();

		m_attractor.UpdateChen(pos, 0.001f);

		SetPosition(pos);
	}
	else
	{
		Move(deltaTime);
	}

	m_pOwner->transform.SetPosition(Vector3{ m_x, m_y, m_z });
}

void Particle::Move(float deltaTime)
{
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;
	m_z += m_velocityZ * deltaTime;
}

float Particle::GetDistanceSquared(Particle const& other)
{
	float distX = other.m_x - m_x;
	float distY = other.m_y - m_y;
	float distZ = other.m_z - m_z;

	return distX * distX + distY * distY + distZ * distZ;
}

float Particle::GetDistance(Particle const& other)
{
	return sqrt(GetDistanceSquared(other));
}

void Particle::UpdateNearbyParticles(const std::vector<Particle*>& allParticles, float maxDistance)
{
	m_nearbyParticles.clear();
	float maxDistSq = maxDistance * maxDistance;

	for (Particle* other : allParticles)
	{
		if (other == this) continue;

		if (GetDistanceSquared(*other) <= maxDistSq)
		{
			m_nearbyParticles.push_back(other);
		}
	}
}