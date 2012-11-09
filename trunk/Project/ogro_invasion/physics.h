#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

//#include "GlutStuff.h"
#include "btBulletCollisionCommon.h"
#include "LinearMath/btHashMap.h"

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class	btDynamicsWorld;

class Physics {

private:
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

	btBroadphaseInterface*	m_broadphase;

	btCollisionDispatcher*	m_dispatcher;

	btConstraintSolver*	m_solver;

	btDefaultCollisionConfiguration* m_collisionConfiguration;

	btDynamicsWorld*		m_dynamicsWorld;


public:
	void	initPhysics();
	void	exitPhysics();

};

#endif