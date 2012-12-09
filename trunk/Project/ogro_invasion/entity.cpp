#include "entity.h"
#include "collider.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

Entity::Entity(GameWorld* const gameWorld):
m_canBeRemoved(false),
m_world(gameWorld)
{
	m_node = NULL;
}

Entity::~Entity()
{
	m_node = NULL;
}

bool Entity::canBeRemoved() const
{
    return m_canBeRemoved;
}

void Entity::destroy()
{
	if(m_node != NULL)
		m_node->removeEntity(this);
    m_canBeRemoved = true;
}

void Entity::prepare(float dt)
{
	btRigidBody* body = getCollider()->getBody();
	// Update Position
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		//printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));

		btVector3 pos = trans.getOrigin();
		float x, y, z;
		x = pos.getX();
		y = pos.getY();
		z = pos.getZ();
		setPosition(Vector3(x, y, z));
	}

    onPrepare(dt);
}

void Entity::render() const
{
    onRender();
}

void Entity::postRender()
{
    onPostRender();
}

bool Entity::initialize()
{
    return onInitialize();
}

void Entity::shutdown()
{
    onShutdown();
}

void Entity::collide(Entity* collider)
{
    //Just call the virtual function (google Non-virtual interface)
    onCollision(collider);
}
