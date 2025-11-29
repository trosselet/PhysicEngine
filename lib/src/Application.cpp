#include "Application.h"

#include <windows.h>
#include <iostream>

#include <Engine/Header/GameManager.h>
#include <Engine/Header/GameObject.h>
#include <Tools/Header/Color.h>

#include "CameraMovement.h"
#include "BoxCollider.h"
#include "Particle.h"


#undef min
#undef max

static DWORD WINAPI ZonePhysicsThread(LPVOID param)
{
    float fixedDt = GameManager::GetFixedDeltaTime();
    float accumulator = 0.0f;

    auto lastTime = std::chrono::steady_clock::now();

    ZoneThreadData* data = (ZoneThreadData*)param;

    std::vector<Particle*> localParticles;
    localParticles.reserve(1024);

    int size = 0;

    while (*data->running)
    {
        EnterCriticalSection(&data->cs);
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = now - lastTime;

        printf("\033[%d;%dH\033[2K  Thread: %d fps: %f, boxAmount: %d", data->zoneId + 8, 0, data->zoneId, 10 / dt.count(), size);
        LeaveCriticalSection(&data->cs);

        lastTime = now;

        accumulator += dt.count();

        if (accumulator > 0.25f)
            accumulator = 0.25f;

        /////////////////////////
        // Get local particles //
        /////////////////////////
        localParticles = std::move(data->currentParticles);
        size = localParticles.size();
        data->currentParticles.clear();

        while (true)
        {
            auto ouput = data->queue.Pop();
            if (!ouput.has_value())
            {
                break;
            }
            localParticles.push_back(*ouput);
        }

        while (accumulator >= fixedDt)
        {
            std::vector<Particle*> remaining;

            ////////////////////////////
            // Update local particles //
            ////////////////////////////
            for (Particle* p : localParticles)
            {
                if (!p)
                {
                    continue;
                }

                p->Update(fixedDt, localParticles);

                BoxCollider* collider = p->GetOwner()->GetScript<BoxCollider>();
                if (collider != nullptr)
                {
                    collider->UpdateCollider();
                }
            }

            /////////////////
            // Zone change //
            /////////////////
            remaining.reserve(localParticles.size());

            for (Particle* p : localParticles)
            {
                if (!p)
                {
                    continue;
                }

                if (!data->zoneCollider->IsPointInside(p->GetPosition()))
                {
                    bool pushed = false;
                    for (int z = 0; z < data->zoneCount; ++z)
                    {
                        ZoneThreadData* other = &data->allZones[z];

                        if (other == data)
                        {
                            continue;
                        }

                        if (other->zoneCollider->IsPointInside(p->GetPosition()))
                        {
                            other->queue.Push(p);
                            pushed = true;
                            break;
                        }
                    }
                    if (!pushed)
                    {
                        if (p->GetOwner()->GetScript<BoxCollider>() != nullptr)
                        {
                            BoxCollider* ca = p->GetOwner()->GetScript<BoxCollider>();
                            BoxCollider* cb = data->zoneCollider;
                            Vector3 delta = p->GetPosition() - data->zoneCollider->GetOwner()->transform.GetPositionFLOAT();
                            Vector3 penetration;

                            penetration.data[0] = std::min(ca->m_max.data[0] - cb->m_min.data[0], cb->m_max.data[0] - ca->m_min.data[0]);
                            penetration.data[1] = std::min(ca->m_max.data[1] - cb->m_min.data[1], cb->m_max.data[1] - ca->m_min.data[1]);
                            penetration.data[2] = std::min(ca->m_max.data[2] - cb->m_min.data[2], cb->m_max.data[2] - ca->m_min.data[2]);


                            int axis = 0;
                            if (penetration.data[1] < penetration.data[0])
                            {
                                axis = 1;
                            }
                            if (penetration.data[2] < penetration.data[axis])
                            {
                                axis = 2;
                            }

                            float correction = penetration.data[axis] / 2.0f;
                            Vector3 corrA{ 0,0,0 };
                            if (delta.data[axis] > 0)
                            {
                                corrA.data[axis] = correction;
                            }
                            else
                            {
                                corrA.data[axis] = -correction;
                            }

                            Vector3 va = p->GetVelocity();
                            va.data[axis] = -va.data[axis];
                            p->SetVelocity(va);

                            p->GetOwner()->transform.SetPosition(p->GetPosition());
                        }

                        remaining.push_back(p);
                    }
                }
                else
                {
                    remaining.push_back(p);
                }
            }

            /////////////////////////////////////////
            // Collision check for local particles //
            /////////////////////////////////////////
            for (Particle* p : remaining)
            {
                if (p->GetOwner()->GetScript<BoxCollider>() != nullptr)
                {
                    for (Particle* neighbor : p->GetNearbyParticles())
                    {
                        BoxCollider* ca = p->GetOwner()->GetScript<BoxCollider>();
                        BoxCollider* cb = neighbor->GetOwner()->GetScript<BoxCollider>();

                        if (ca->IsColliding(*cb))
                        {
                            Vector3 delta = p->GetPosition() - neighbor->GetPosition();
                            Vector3 penetration;

                            //////////////////////////////////////////////
                            // Calcul of penetration depth on each axis //
                            //////////////////////////////////////////////
                            penetration.data[0] = std::min(ca->m_max.data[0] - cb->m_min.data[0], cb->m_max.data[0] - ca->m_min.data[0]);
                            penetration.data[1] = std::min(ca->m_max.data[1] - cb->m_min.data[1], cb->m_max.data[1] - ca->m_min.data[1]);
                            penetration.data[2] = std::min(ca->m_max.data[2] - cb->m_min.data[2], cb->m_max.data[2] - ca->m_min.data[2]);


                            int axis = 0;
                            if (penetration.data[1] < penetration.data[0])
                            {
                                axis = 1;
                            }
                            if (penetration.data[2] < penetration.data[axis])
                            {
                                axis = 2;
                            }

                            float correction = penetration.data[axis] / 2.0f;
                            Vector3 corrA{ 0,0,0 }, corrB{ 0,0,0 };
                            if (delta.data[axis] > 0)
                            {
                                corrA.data[axis] = correction;
                                corrB.data[axis] = -correction;
                            }
                            else
                            {
                                corrA.data[axis] = -correction;
                                corrB.data[axis] = correction;
                            }
                            p->SetPosition(p->GetPosition() + corrA);
                            neighbor->SetPosition(neighbor->GetPosition() + corrB);

                            Vector3 va = p->GetVelocity();
                            Vector3 vb = neighbor->GetVelocity();
                            va.data[axis] = -va.data[axis];
                            vb.data[axis] = -vb.data[axis];
                            p->SetVelocity(va);
                            neighbor->SetVelocity(vb);

                            p->GetOwner()->transform.SetPosition(p->GetPosition());
                            neighbor->GetOwner()->transform.SetPosition(neighbor->GetPosition());
                        }
                    }
                }
            }

            accumulator -= fixedDt;
            localParticles = std::move(remaining);
        }

        data->currentParticles = std::move(localParticles);
    }

    return 0;
}

Application::~Application()
{
    //////////////////
    // Clear memory //
    //////////////////
    delete m_pObjectColliders;
    delete m_pScene;
    delete[] m_pZData;
    delete[] m_pZThreads;

    m_pObjectColliders = nullptr;
    m_pZData = nullptr;
    m_pZThreads = nullptr;

    GameManager::Release();
}

void Application::StartUp(bool useObj, AttractorType attractorType)
{
	Initialize();
	CreateSpacePartition();
	CreateThreads();
	CreateParticles(useObj, attractorType);
}

void Application::Initialize()
{
    ////////////////////////////
    // Initialize Game engine //
    ////////////////////////////
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    srand(time(0));

    HINSTANCE hInstance = nullptr;
    GameManager::Initialize(hInstance);

    m_pScene = new Scene();
    *m_pScene = Scene::Create();
    m_pScene->Load();
    m_pScene->SetActive();

    GameObject* const pMainCamera = new GameObject(*m_pScene);
    pMainCamera->transform.SetPosition({ 0.0f, 0.0f, 0.0f });
    Camera& cameraComponent = pMainCamera->AddComponent<Camera>();
    pMainCamera->SetName("Camera");
    pMainCamera->AddScript<CameraMovement>();

    GameObject* pSkyBox = new GameObject(*m_pScene);
    pSkyBox->transform.SetPosition({ 0, 0, 0 });
    pSkyBox->transform.SetScaling(Vector3{ -10000, -10000, -10000 });
    MeshRenderer& mr = pSkyBox->AddComponent<MeshRenderer>();
    mr.SetSphere("sky.dds", Color::White);
    pSkyBox->SetName("SkyBox");
}

void Application::CreateSpacePartition()
{
    //////////////////////////////////
    // Create space partition zones //
    //////////////////////////////////
    m_zoneGridX = 2;
    m_zoneGridY = 2;
    m_zoneGridZ = 2;
    m_zoneCount = m_zoneGridX * m_zoneGridY * m_zoneGridZ;
    m_zoneScale = 120.0f;

    m_zoneGOs.reserve(m_zoneCount);

    m_zoneColliders.reserve(m_zoneCount);

    m_pObjectColliders = new std::vector<BoxCollider*>();
}

void Application::CreateThreads()
{
    

    //////////////////////////////////////////
    // Create space partition zones objects //
    //////////////////////////////////////////
    for (int x = 0; x < m_zoneGridX; ++x)
    {
        for (int y = 0; y < m_zoneGridY; ++y)
        {
            for (int z = 0; z < m_zoneGridZ; ++z)
            {
                GameObject* pZone = new GameObject(*m_pScene);
                pZone->transform.SetPosition({ x * m_zoneScale, y * m_zoneScale, z * m_zoneScale + 5.0f });
                pZone->transform.SetScaling(Vector3{ -m_zoneScale, -m_zoneScale, -m_zoneScale });

                MeshRenderer& mr = pZone->AddComponent<MeshRenderer>();
                mr.SetCube("Frame.dds", Color::White);

                pZone->SetName("Zone");

                BoxCollider* bc = pZone->AddScript<BoxCollider>(&m_zoneColliders);

                m_zoneGOs.push_back(pZone);
                m_zoneColliders.push_back(bc);
            }
        }
    }

    for (BoxCollider* zc : m_zoneColliders)
        if (zc) zc->UpdateCollider();

}

void Application::CreateParticles(bool useObj, AttractorType attractorType)
{
    /////////////////////////////////
    // Create thread data per zone //
    /////////////////////////////////
    m_pZData = new ZoneThreadData[m_zoneCount];
    m_pZThreads = new HANDLE[m_zoneCount];

    for (int i = 0; i < m_zoneCount; ++i)
    {
        m_pZData[i].zoneCollider = m_zoneColliders[i];
        m_pZData[i].allZones = m_pZData;
        m_pZData[i].zoneCount = m_zoneCount;
        m_pZData[i].zoneId = i;
        m_pZData[i].running = new bool(true);
        InitializeCriticalSection(&m_pZData[i].cs);
    }

    bool running = true;
    for (int i = 0; i < m_zoneCount; ++i)
        m_pZData[i].running = &running;

    ////////////////////////////////////////////////////
    // Creates particles and distribute them to zones //
    ////////////////////////////////////////////////////
    int totalParticles = 8000;
    int threadIndex = 0;

    for (int d = 0; d < totalParticles; ++d)
    {
        GameObject* pGO = new GameObject(*m_pScene);

        MeshRenderer& mr = pGO->AddComponent<MeshRenderer>();

        if (useObj)
        {
            mr.SetMeshFile("Horse.obj", "Horse.dds");
            pGO->transform.SetScaling(Vector3{ 0.005f, 0.005f, 0.005f });
            pGO->transform.RotateYPR({ 0.0f, 55.0f, 0.0f });
        }
        else
        {
            switch (d % 3)
            {
            case 0:
                mr.SetCube("red.dds", Color::White);
                break;
            case 1:
                mr.SetCube("orange.dds", Color::White);
                break;
            case 2:
                mr.SetCube("yellow.dds", Color::White);
                break;
            }
            pGO->transform.SetScaling(Vector3{ 1.f, 1.f, 1.f });
        }

        pGO->SetName("Particle");


        Particle* p = pGO->AddScript<Particle>();

        p->UseChaoticAttractor(attractorType);

        BoxCollider* zc = m_pZData[threadIndex].zoneCollider;
        Vector3 min = zc->m_min;
        Vector3 max = zc->m_max;

        Vector3 ppos
        {
            min.data[0] + (float)(((double)rand() / RAND_MAX) * (max.data[0] - min.data[0])),
            min.data[1] + (float)(((double)rand() / RAND_MAX) * (max.data[1] - min.data[1])),
            min.data[2] + (float)(((double)rand() / RAND_MAX) * (max.data[2] - min.data[2]))
        };

        p->SetPosition(ppos);

        m_pZData[threadIndex].queue.Push(p);

		if (attractorType == AttractorType::None)
        {
            pGO->AddScript<BoxCollider>(m_pObjectColliders);
        }
        
        threadIndex = (threadIndex + 1) % m_zoneCount;
    }

    //////////////////////
    // Starting threads //
    //////////////////////
    for (int i = 0; i < m_zoneCount; ++i)
    {
        m_pZThreads[i] = CreateThread(NULL, 0, ZonePhysicsThread, &m_pZData[i], 0, NULL);
    }

    ////////////////////////////
    // Start engine game loop //
    ////////////////////////////
    GameManager::Run();
}

void Application::ClearUp()
{

    //////////////////////////////////
    // Closing and clearing threads //
    //////////////////////////////////
    m_running = false;
    WaitForMultipleObjects(m_zoneCount, m_pZThreads, TRUE, INFINITE);

    for (int i = 0; i < m_zoneCount; ++i)
    {
        if (m_pZThreads[i] != nullptr)
        {
            CloseHandle(m_pZThreads[i]);
        }
    }

}