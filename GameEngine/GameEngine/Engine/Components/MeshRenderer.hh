#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include "AComponent.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"

namespace Components
{

class MeshRenderer : public AComponent
{
private:
	SmartPointer<Resources::SharedMesh>	_mesh;
	std::string							_shader;

	MeshRenderer();
	MeshRenderer(MeshRenderer const &);
	MeshRenderer	&operator=(MeshRenderer const &);

public:
	MeshRenderer(std::string const &name, std::string const &resource);
	virtual ~MeshRenderer(void);

	virtual void	start();
	virtual void	update();
	virtual void	stop();

	bool				setShader(std::string const &name);
	std::string const	&getShader() const;

	SmartPointer<Resources::SharedMesh>	const &getMesh() const;
};

}

#endif