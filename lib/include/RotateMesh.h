#ifndef ROTATE_MESH_INCLUDED__H
#define ROTATE_MESH_INCLUDED__H

#include <Engine/Header/IScript.h>

class RotateMesh : public IScript
{
public:
	RotateMesh();
	~RotateMesh();

	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;

private:
	float rotationSpeed;
};


#endif // !ROTATE_MESH_INCLUDED__H