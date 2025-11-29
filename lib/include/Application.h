#ifndef APPLICATION_INCLUDED__H
#define APPLICATION_INCLUDED__H
#include "SafeQueue.hpp"
#include <vector>
#include <Engine/Header/Scene.h>

#include "ChaoticAttractor.h"

class GameObject;
class BoxCollider;
class Particle;
struct ZoneThreadData
{
	BoxCollider* zoneCollider = nullptr;
	SafeQueue<Particle*> queue;
	std::vector<Particle*> currentParticles;
	ZoneThreadData* allZones = nullptr;
	int zoneCount = 0;
	int zoneId = 0;
	bool* running = nullptr;
	CRITICAL_SECTION cs{};

	~ZoneThreadData()
	{
		DeleteCriticalSection(&cs);
	}
};

class Application
{
public:
	Application() = default;
	~Application();

	void StartUp(bool useObj = false, AttractorType attractorType = AttractorType::None);
	void ClearUp();

private:
	void Initialize();
	void CreateSpacePartition();
	void CreateThreads();
	void CreateParticles(bool useObj, AttractorType attractorType);

private:

	Scene* m_pScene;

	int m_zoneGridX;
	int m_zoneGridY;
	int m_zoneGridZ;
	int m_zoneCount;

	float m_zoneScale;

	bool m_running;

	HANDLE* m_pZThreads;
	ZoneThreadData* m_pZData;
	std::vector<GameObject*> m_zoneGOs;
	std::vector<BoxCollider*> m_zoneColliders;
	std::vector<BoxCollider*>* m_pObjectColliders;


};

#endif