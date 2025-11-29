#include "ChaoticAttractor.h"


void ChaoticAttractor::UpdateLorenz(Vector3& pos, float dt)
{
    float x = pos.data[0];
    float y = pos.data[1];
    float z = pos.data[2];

    float dx = sigma * (y - x);
    float dy = x * (rho - z) - y;
    float dz = x * y - beta * z;

    pos.data[0] += dx * dt / 3;
    pos.data[1] += dy * dt / 3;
    pos.data[2] += dz * dt / 3;
}

void ChaoticAttractor::UpdateThomas(Vector3& pos, float dt)
{
    float x = pos.data[0];
    float y = pos.data[1];
    float z = pos.data[2];

    float dx = std::sin(y) - b * x;
    float dy = std::sin(z) - b * y;
    float dz = std::sin(x) - b * z;

    pos.data[0] += dx * dt;
    pos.data[1] += dy * dt;
    pos.data[2] += dz * dt;
}

void ChaoticAttractor::UpdateRossler(Vector3& pos, float dt)
{
    float x = pos.data[0];
    float y = pos.data[1];
    float z = pos.data[2];

    float dx = -y - z;
    float dy = x + a * y;
    float dz = b + z * (x - c);

    pos.data[0] += dx * dt;
    pos.data[1] += dy * dt;
    pos.data[2] += dz * dt;
}

void ChaoticAttractor::UpdateAizawa(Vector3& pos, float dt)
{
    float x = pos.data[0];
    float y = pos.data[1];
    float z = pos.data[2];

    float dx = (z - betaA) * x - deltaA * y;
    float dy = deltaA * x + (z - betaA) * y;
    float dz = gammaA + alphaA * z - (z * z * z) / 3 - (x * x + y * y) * (1 + epsilonA * z) + zetaA * z * x * x * x;

    pos.data[0] += dx * dt;
    pos.data[1] += dy * dt;
    pos.data[2] += dz * dt;
}

void ChaoticAttractor::UpdateChen(Vector3& pos, float dt)
{
    float x = pos.data[0];
    float y = pos.data[1];
    float z = pos.data[2];

    float dx = aC * (y - x);
    float dy = (cC - aC) * x - x * z + cC * y;
    float dz = x * y - bC * z;

    pos.data[0] += dx * dt;
    pos.data[1] += dy * dt;
    pos.data[2] += dz * dt;
}