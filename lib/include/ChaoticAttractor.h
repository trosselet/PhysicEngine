#ifndef CHAOTIC_ATTRACTOR_INCLUDED__H
#define CHAOTIC_ATTRACTOR_INCLUDED__H

#include <Tools/Header/PrimitiveTypes.h>

enum class AttractorType
{
	None,
    Lorenz,
    Thomas,
    Rossler,
    Aizawa,
    Chen
};

class ChaoticAttractor
{
public:
	// Lorenz attractor parameters
    float sigma = 10.0f;
    float rho = 28.0f;
    float beta = 8.0f / 3.0f;

	// Thomas attractor parameter
    float a = 0.2f;
    float b = 0.2f;
    float c = 5.7f;

	// Aizawa attractor parameters
    float alphaA = 0.95f;
    float betaA = 0.7f;
    float gammaA = 0.6f;
    float deltaA = 3.5f;
    float epsilonA = 0.25f;
    float zetaA = 0.1f;

	// Chen attractor parameters
    float aC = 35.0f;
    float bC = 3.0f;
    float cC = 28.0f;

    void UpdateLorenz(Vector3& pos, float dt);
    void UpdateThomas(Vector3& pos, float dt);
    void UpdateRossler(Vector3& pos, float dt);
    void UpdateAizawa(Vector3& pos, float dt);
    void UpdateChen(Vector3& pos, float dt);
};

#endif // !CHAOTIC_ATTRACTOR_INCLUDED__H