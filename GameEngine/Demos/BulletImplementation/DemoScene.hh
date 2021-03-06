#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"
#include "Core/Engine.hh"

class DemoScene : public AScene
{
public:
	DemoScene(Engine &engine);
	virtual ~DemoScene(void);

	Entity  DemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  DemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  DemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
};

#endif