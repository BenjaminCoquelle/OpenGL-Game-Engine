
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Core/Input.hh"
#include "Core/Timer.hh"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"
#include "Utils/Barcode.h"
#include "Utils/PubSub.hpp"
#include "glm/glm.hpp"
#include <Components/Component.hh>

class AScene;
class EntityManager;
class Entity;

class EntityData : public PubSub
{
private:
	static size_t 					_currentId;
	Entity                          _handle;
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

	typedef std::vector<SmartPointer<Component::Base> >	t_ComponentsList;

	Entity &getHandle();
	void setHandle(Entity &handle);

private:
	AScene              *_scene;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	glm::vec3           _localRotation;
	glm::vec3           _localScale;
	glm::vec3           _localTranslation;

	glm::vec3           _globalRotation;
	glm::vec3           _globalScale;
	glm::vec3           _globalTranslation;

	t_ComponentsList	_components;

	Barcode _code;
public:
	EntityData(AScene *scene);
	virtual ~EntityData();

	void                    translate(const glm::vec3 &v);
	void                    setTranslation(const glm::vec3 &v);
	glm::vec3 const         &getTranslation() const;
	void                    rotate(const glm::vec3 &v);
	void                    setRotation(const glm::vec3 &v);
	glm::vec3 const         &getRotation() const;
	void                    scale(const glm::vec3 &v);
	void                    setScale(const glm::vec3 &v);
	glm::vec3 const         &getScale() const;


	glm::mat4 const  		&getLocalTransform();
	// TO DELETE
	glm::mat4   			&setLocalTransform();  // <-- //

	glm::mat4 const			&getGlobalTransform() const;
	// TO DELETE
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);  // <-- //
	void                    computeGlobalTransform(const Entity &parent);

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	Barcode &getCode();
	void reset();

	////////////////////////////
	//
	//
	// COMPONENTS OPERATIONS
	//
	//
	//
	bool hasComponent(unsigned int componentId) const;

	template <typename T>
	bool hasComponent() const
	{
		return _code.isSet<T>();
	}

	template <typename T, typename... Args>
	SmartPointer<T> addComponent(Args ...args)
	{
		// get the component type ID
		unsigned int id = T::getTypeId();

		auto a = _code.isEmpty();

		// if entity already have component, return it
		if (hasComponent(id))
		{
			return static_cast<SmartPointer<T> >(_components[id]);
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 1);
		}
		// if component has never been created, create one
		if (!_components[id].get())
		{
			_components[id] = new T(_scene, getHandle());
			assert(_components[id].get() != nullptr && "Memory error : Component creation failed.");
		}
		//init component
		static_cast<SmartPointer<T> >(_components[id])->init(args...);
		_code.add(id);
		broadCast(std::string("componentAdded" + std::to_string(id)), _handle);
		return static_cast<SmartPointer<T> >(_components[id]);
	}

	template <typename T>
	SmartPointer<T> getComponent() const
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return nullptr;
		return static_cast<SmartPointer<T> >(_components[id]);
	}

	template <typename T>
	void removeComponent()
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return;
		_code.remove(id);
//		_components[id]	= nullptr;
		_components[id].get()->reset();
		broadCast(std::string("componentRemoved" + std::to_string(id)), _handle);
		// component remove -> signal to system
	}
};

#endif