#ifndef		__COLLISION_BODY_HPP__
#define		__COLLISION_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>
#include <ResourceManager/SharedMesh.hh>
#include <Managers/BulletCollisionManager.hpp>
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Core/AScene.hh>


namespace Component
{
	ATTRIBUTE_ALIGNED16(class) CollisionBody : public Component::ComponentBase<CollisionBody>
	{
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();
		typedef enum
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		} CollisionShape;

		CollisionBody(AScene *scene, Entity &entity)
			: ComponentBase(scene, entity, "CollisionBody"),
			_manager(nullptr),
			_shapeType(UNDEFINED),
			_meshName(""),
			_collisionShape(nullptr),
			_body(nullptr)
		{
			_manager = dynamic_cast<BulletCollisionManager*>(&scene->getEngine().getInstance<BulletCollisionManager>());
			assert(_manager != nullptr);
		}

		virtual void reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
				_body = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
				_collisionShape = nullptr;
			}
			_shapeType = UNDEFINED;
		}

		bool init()
		{
			return true;
		}

		btCollisionShape &getShape()
		{
			assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
			return *_collisionShape;
		}

		btCollisionObject &getBody()
		{
			assert(_body != nullptr && "Collision body is NULL. Tips : Have you setAcollisionShape to Component ?.");
			return *_body;
		}

		void setCollisionShape(CollisionShape c, const std::string &meshName = "")
		{
			if (c == UNDEFINED)
				return;
			_meshName = meshName;
			_reset();
			_shapeType = c;
			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
			if (c == BOX)
			{
				_collisionShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));//new btBoxShape(halfScale);
			}
			else if (c == SPHERE)
			{
				_collisionShape = new btSphereShape(1);//new btSphereShape(scale.x);
			}
			else if (c == MESH)
			{
				SmartPointer<Resources::SharedMesh> mesh = _scene->getEngine().getInstance<Resources::ResourceManager>().getResource(meshName);
				const Resources::Geometry &geo = mesh->getGeometry()[0]; // DIRTY HACK TEMPORARY
				// NEED TO REPLACE MESH BY MESH GROUP !
				btScalar *t = new btScalar[geo.vertices.size() * 3]();
				for (unsigned int i = 0; i < geo.vertices.size(); ++i)
				{
					t[i * 3] = geo.vertices[i].x;
					t[i * 3 + 1] = geo.vertices[i].y;
					t[i * 3 + 2] = geo.vertices[i].z;
				}
				btConvexHullShape *tmp = new btConvexHullShape(t, geo.vertices.size(), 3 * sizeof(btScalar));
				btShapeHull *hull = new btShapeHull(tmp);
				btScalar margin = tmp->getMargin();
				hull->buildHull(margin);
				tmp->setUserPointer(hull);
				btConvexHullShape *s = new btConvexHullShape();
				for (int i = 0; i < hull->numVertices(); ++i)
				{
					s->addPoint(hull->getVertexPointer()[i], false);
				}
				s->recalcLocalAabb();
				_collisionShape = s;
				delete[] t;
				delete hull;
				delete tmp;
			}
			_collisionShape->setLocalScaling(convertGLMVectorToBullet(scale));
			_body = new btCollisionObject();
			_body->setUserPointer(&_entity);
			_body->setCollisionShape(_collisionShape);
			_body->setWorldTransform(transform);
			_manager->getWorld()->addCollisionObject(_body, btCollisionObject::CF_STATIC_OBJECT, btCollisionObject::CF_STATIC_OBJECT);
		}

		virtual ~CollisionBody(void)
		{
			if (_body)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
			}
			if (_collisionShape)
				delete _collisionShape;
		}

	private:
		BulletCollisionManager *_manager;
		CollisionShape _shapeType;
		std::string _meshName;
		btCollisionShape *_collisionShape;
		btCollisionObject *_body;
	private:
		CollisionBody(CollisionBody const &);
		CollisionBody &operator=(CollisionBody const &);

		void _reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
			}
		}
	};

}

#endif //!__COLLISION_BODY_HPP__