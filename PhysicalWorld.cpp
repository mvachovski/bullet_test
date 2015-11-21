/*
 * BulletSim.cpp, renamed to PhysicalWorld.cpp
 *
 *  Created on: 17 Oct 2015
 *      Author: martin
 */

#include "PhysicalWorld.h"

#include<cmath>

PhysicalWorld::PhysicalWorld()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_solver,m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0,-10,0));

	m_groundTransform.setIdentity();
	m_groundTransform.setOrigin(btVector3(0,-56,0));

	populatePhysicalWorld();

}

void PhysicalWorld::populatePhysicalWorld()
{

	{
		btScalar mass(0.);
		btCollisionShape *pStaticPlaneShape = new btStaticPlaneShape(btVector3(0, 1., 0), .0);
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(0,-15,0));

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			pStaticPlaneShape->calculateLocalInertia(mass,localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,pStaticPlaneShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		m_dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		startTransform.setOrigin(btVector3(-5,15,0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* colShape = new btSphereShape(btScalar(2.));
		m_collisionShapes.push_back(colShape);

		btScalar	mass(10.f);
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(5,5,0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* colShape = new btCylinderShape(btVector3(1, 3, 1));
		m_collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(10.f);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		double theta = M_PI * .93;
		btQuaternion q(cos(theta / 2), -sin(theta / 2), -sin(theta / 2), -sin(theta) / 2);

		startTransform.setRotation(q);
		startTransform.setOrigin(btVector3(0, 5, 0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* colShape = new btBoxShape(btVector3(3, 3, 3));
		m_collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(10.f);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		startTransform.setOrigin(btVector3(0, 10, -5));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
	}

}

void PhysicalWorld::integrate()
{
	m_dynamicsWorld->stepSimulation(1.f/60.f,10);
}

PhysicalWorld::~PhysicalWorld()
{

	//remove the rigidbodies from the dynamics world and delete them
	for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		m_collisionShapes[j] = 0;
		delete shape;
	}

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_overlappingPairCache;
	delete m_dispatcher;
	delete m_collisionConfiguration;
	m_collisionShapes.clear();

}

