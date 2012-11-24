#include "physics.h"
#include "entity.h"
#include "collider.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "terrain.h"
#include <string>
#include <vector>
using std::vector;

void Physics::initPhysics()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
	m_dynamicsWorld->setGravity( btVector3(0.0,-10,0.0));

	//addStaticPlane();

}

void Physics::exitPhysics()
{


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
	//entity->getCollider()->setBody(body);

	//body->forceActivationState(DISABLE_DEACTIVATION); // Check this, it should be enabled when object is moving..

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
				mass = btScalar(1.0f);
				colShape = new btBoxShape(btVector3(0.2, 1, 0.2));//new btCapsuleShape(btScalar(entity->getCollider()->getRadius()), btScalar(entity->getCollider()->getRadius()));
				break;
			case PLAYER:
				mass = btScalar(1.0f);
				colShape = new btBoxShape(btVector3(0.2, 1, 0.2));//btCapsuleShape(btScalar(entity->getCollider()->getRadius()), btScalar(entity->getCollider()->getRadius()));// btBoxShape(btVector3(1,1,1));
				break;
			case TREE:
				mass = btScalar(0.0f);
				colShape = new btBoxShape(btVector3(0.2, 1, 0.2));
				break;
			case ROCKET:
				mass = btScalar(1.0f);
				colShape = new btSphereShape(btScalar(entity->getCollider()->getRadius()));
				break;
			case LANDSCAPE:
			{
				bool use_heightmap = false;
				mass = btScalar(0.0f);
				if (!use_heightmap) {
					//colShape = new btBoxShape(btVector3(50, 4, 50));//new btStaticPlaneShape(btVector3(0,1,0), 1000); // Check Constant
					colShape = new btStaticPlaneShape(btVector3(0,1,0), btScalar(0));
					//colShape = ((Terrain*) entity)->createShape();
					} else {

				
					std::string heightmap_str = "data/island.raw";

					std::ifstream fileIn(heightmap_str.c_str(), std::ios::binary);

					if (!fileIn.good())
					{
						//std::cout << "File does not exist" << std::endl;
						return;
					}

					//This line reads in the whole file into a string
					string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));

					fileIn.close();
					vector<float> heights;
					heights.reserve(65 * 65); //Reserve some space (faster)

					//Go through the string converting each character to a float and scale it
					for (int i = 0; i < (65* 65); ++i)
					{
						//Convert to floating value, the unsigned char cast is importantant otherwise the values wrap at 128
						float value = (float)(unsigned char)stringBuffer[i] / 256.0f;

						heights.push_back(value * 10.);
					}

					colShape = new btHeightfieldTerrainShape(65, 65, &heights[0],
						10., 0., 20.,
						1, PHY_FLOAT, false);
				}

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
			startTransform.setOrigin(btVector3(0,0,0));
		else if (entity->getType() == TREE)
			startTransform.setOrigin(btVector3(pos.x, 0,pos.z)); // y == 5.2
		else // Make entities fall from sky
			startTransform.setOrigin(btVector3(pos.x,pos.y + 5,pos.z));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape, btVector3(0, 0, 0)); // should be localInertia, but we dont want them to rotate
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
		entity->getCollider()->setBody(body);

		body->forceActivationState(DISABLE_DEACTIVATION); // Check this, it should be enabled when object is moving..
		
	}

}

void Physics::unregisterEntity(Entity* entity)
{

}

void Physics::update(float speed)
{
	m_dynamicsWorld->stepSimulation(btScalar(speed), 10);
}