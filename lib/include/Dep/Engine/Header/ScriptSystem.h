#ifndef SCRIPTSYSTEM_INCLUDE__H
#define SCRIPTSYSTEM_INCLUDE__H

#include <unordered_map>
#include <vector>

class IScript;

class ScriptSystem
{
public:
	ScriptSystem() = default;
	~ScriptSystem();

	void OnUpdate();
	void OnFixedUpdate();

	void RemoveGameObject(unsigned int gameObjectId);
	void HandleDeletion();

	std::unordered_map<unsigned int, std::vector<IScript*>> m_scriptsByEntity;

	std::unordered_map<unsigned int, bool> m_entityToRemove;
private:

private:
	friend class GameManager;
};


#endif // !SCRIPTSYSTEM_INCLUDE__H