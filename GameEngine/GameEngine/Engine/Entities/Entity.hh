
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Components/AComponent.hh"
#include "Core/Input.hh"
#include "Core/Timer.hh"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"

#include "glm/glm.hpp"

class Entity
{
private:
	static size_t 					_currentId;

public:
	enum	GetFlags
	{
		ONLY_THIS_FLAGS,
		THIS_FLAGS,
		NOT_THIS_FLAGS
	};

	enum	EntityFlags
	{
		HAS_MOVED = 1
	};

	typedef std::map<size_t, SmartPointer<Entity> >						t_sonsList;
	typedef std::list<SmartPointer<Entity> >								t_EntityList;
	typedef std::map<std::string, SmartPointer<Components::AComponent> >	t_ComponentsList;

private:
	size_t 				_id;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	Entity 			*_father;
	t_sonsList 			_sons;
	t_ComponentsList	_components;

	Entity(Entity const &oth);
	Entity 			&operator=(Entity const &oth);

public:
	Entity();
	virtual ~Entity();

	glm::mat4 const  		&getLocalTransform();
	glm::mat4   			&setLocalTransform();

	glm::mat4 const			&getGlobalTransform() const;
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);

	size_t 					getId() const;

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	Entity 				*getFather() const;
	void 					setFather(Entity *father);

	void 					addSon(SmartPointer<Entity> const &son);
	void 					removeSon(size_t sonId);
	SmartPointer<Entity> 	getSon(size_t sonId);
	SmartPointer<Entity> 	getSonRec(size_t sonId);

	SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, GetFlags op);

	void					addComponent(SmartPointer<Components::AComponent> const &component);
	bool					removeComponent(std::string const &name);

	t_sonsList::iterator 		getSonsBegin();
	t_sonsList::iterator 		getSonsEnd();
	t_ComponentsList::iterator 	getComponentsBegin();
	t_ComponentsList::iterator 	getComponentsEnd();
};

#endif