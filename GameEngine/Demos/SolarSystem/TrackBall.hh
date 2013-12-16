#ifndef		TRACKBALL_HH_
#define		TRACKBALL_HH_

#include "Core\ACamera.hh"
#include "Core/Engine.hh"
#include "glm/glm.hpp"

class TrackBall : public ACamera
{
private:
	float					_dist;
	float					_rotateSpeed, _zoomSpeed;
	glm::vec2				_angles;
	Handle              	_toLook;

public:
	TrackBall(Engine &engine, Handle toLook, float dist,
			  float rotatingSpeed, float zoomingSpeed);
	virtual ~TrackBall(void);

	virtual void      customUpdate();
};

#endif