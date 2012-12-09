#include "physics.h"
#include "entity.h"
#include "collider.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "terrain.h"
#include "landscape.h"
#include <string>
#include <cassert>
#include <sys/stat.h>
#include <vector>
using std::vector;
struct stat results;

static btVector3
getUpVector
(
int upAxis,
btScalar regularValue,
btScalar upValue
)
{
	btAssert(upAxis >= 0 && upAxis <= 2 && "bad up axis");

	btVector3 v(regularValue, regularValue, regularValue);
	v[upAxis] = upValue;

	return v;
}

Physics::Physics(GameWorld* gworld)
{
	world = gworld;
}

Physics::~Physics()
{
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;
	delete m_solver;
	delete m_dynamicsWorld;
}

void Physics::initPhysics()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
	m_dynamicsWorld->setGravity( btVector3(0.0,-15,0.0));

	addStaticPlane();

}

void Physics::exitPhysics()
{
	// Clean anything left

}

void Physics::addStaticPlane()
{
	btScalar mass = btScalar(0);
	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0,1,0), btScalar(0));

	m_collisionShapes.push_back(colShape);
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	//Vector3 pos = entity->getPosition();
	startTransform.setOrigin(btVector3(0,0,0));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape, btVector3(0, 0, 0));
	btRigidBody* body = new btRigidBody(rbInfo);

	m_dynamicsWorld->addRigidBody(body);

}

void Physics::registerEntity(Entity* entity)
{

	{
		//create a dynamic rigidbody

		
		//btCollisionShape* colShape = new btSphereShape(btScalar(entity->getCollider()->getRadius())); // Check Ratio (1.)


		/*

		    OGRO,
    PLAYER,
    ROCKET,
    EXPLOSION,
    LANDSCAPE,
    TREE
	*/

		btScalar mass;
		btCollisionShape* colShape;
		switch (entity->getType()) {
			case OGRO:
				mass = btScalar(0.5f);
				colShape = new btCapsuleShape(0.35, 0.5);
				break;
			case PLAYER:
				mass = btScalar(10.0f);
				colShape = new btCapsuleShape(0.3, 1);
				break;
			case TREE:
				mass = btScalar(0.0f);
				colShape = new btBoxShape(btVector3(0.3, 4, 0.3));
				break;
			case LOG:
				mass = btScalar(100.0f);
				colShape = new btCylinderShape(btVector3(0.3, 0.75, 0.3)); // No less than 0.3
				break;
			case ROCKET:
				mass = btScalar(1.0f);
				colShape = new btSphereShape(btScalar(entity->getCollider()->getRadius()));
				break;
			case HOUSE:
				mass = btScalar(0.0f);
				colShape = new btBoxShape(btVector3(1.2f, 1.5f, 0.9f));
				break;
			case LANDSCAPE:
			{
				mass = btScalar(0.0f);
				colShape = dynamic_cast<Landscape*>(entity)->getTerrain()->createShape();

				break;
			}
			default:
				return;

		}
		m_collisionShapes.push_back(colShape);
		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		Vector3 pos = entity->getPosition();
		if (entity->getType() == LANDSCAPE)
			startTransform.setOrigin(btVector3(0, 0,0));
		else if (entity->getType() == TREE || entity->getType() == LOG) {
			float height = world->getLandscape()->getTerrain()->getHeightAt(pos.x, pos.z);
			startTransform.setOrigin(btVector3(pos.x, height + 1,pos.z));
		} else if (entity->getType() == LOG) {
			float height = world->getLandscape()->getTerrain()->getHeightAt(pos.x, pos.z);
			startTransform.setOrigin(btVector3(pos.x, height + 1,pos.z));
		} else if (entity->getType() == HOUSE) {
			float height = world->getLandscape()->getTerrain()->getHeightAt(pos.x, pos.z);
			startTransform.setOrigin(btVector3(pos.x, height + 1,pos.z));
		} else if (entity->getType() == ROCKET) {
			startTransform.setOrigin(btVector3(pos.x, pos.y,pos.z));
		} else // Make entities fall from sky
			startTransform.setOrigin(btVector3(pos.x,pos.y + 10,pos.z));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic && entity->getType() == LOG) { // The Only Element that rotates for now...
			colShape->calculateLocalInertia(mass,localInertia);
		}

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		//body->setContactProcessingThreshold(BT_LARGE_FLOAT);
		body->setUserPointer(entity);
		m_dynamicsWorld->addRigidBody(body);
		entity->getCollider()->setBody(body);

		if (mass == 0.f) { // Disable the "Disabled" State
		  body->setCollisionFlags( body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		  body->setActivationState(DISABLE_DEACTIVATION);
	   }

		if (entity->getType() == LANDSCAPE) { // Add Friction to Landscape
			body->setFriction(1);
			body->setRollingFriction(0.35); // 0.35
		}
		if (entity->getType() == LOG)
			body->setRollingFriction(0.2); // 0.2
		if (entity->getType() == PLAYER)
			body->setFriction(0.5);
	}
}

void Physics::unregisterEntity(Entity* entity)
{
	m_dynamicsWorld->removeRigidBody(entity->getCollider()->getBody());
}

void Physics::update(float speed)
{
	m_dynamicsWorld->stepSimulation(btScalar(speed), 10);

	// Detect Collision
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				Entity* enA = (Entity*) obA->getUserPointer();
				Entity* enB = (Entity*) obB->getUserPointer();
				if (enA != NULL && enB != NULL && !enA->canBeRemoved() && !enB->canBeRemoved()) {

					enA->collide(enB);
					enB->collide(enA);

				}
			}
		}
	}

}