#ifndef   __TRACKING_CAM_SYSTEM_HPP__
# define  __TRACKING_CAM_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/CameraComponent.hh>
#include <Components/TrackingCamera.hpp>
#include <Entities/EntityData.hh>
#include <Core/Input.hh>
#include <Core/Engine.hh>
#include <Context/SdlContext.hh>
#include <Utils/MatrixConversion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

class TrackingCameraSystem : public System
{
public:
	TrackingCameraSystem(AScene *scene)
		: System(scene)
	{}
	virtual ~TrackingCameraSystem(){}
protected:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		Input			&inputs = _scene->getEngine().getInstance<Input>();

		for (auto e : _collection)
		{
			auto c = e->getComponent<Component::CameraComponent>();
			auto t = e->getComponent<Component::TrackingCamera>();

			auto mat = t->toLook->getLocalTransform();
			mat = glm::translate(mat, t->dist);

			mat = glm::interpolate(e->getLocalTransform(), mat, 0.05f);

			glm::vec3 pos = posFromMat4(mat);
			e->setLocalTransform() = mat;
			c->setLookAtTransform() = glm::lookAt(pos,
				glm::vec3(t->toLook->getGlobalTransform()[3]),
				glm::vec3(0, 1, 0));//e->getLocalTransform();
		}
	}

	virtual void initialize()
	{
		require<Component::CameraComponent>();
		require<Component::TrackingCamera>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__TRACKING_CAM_SYSTEM_HPP__