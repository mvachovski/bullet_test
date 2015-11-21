/*
 * BulletSim.h
 *
 *  Created on: 17 Oct 2015
 *      Author: martin
 */

#ifndef PHYSICALWORLD_H_
#define PHYSICALWORLD_H_

#include "btBulletDynamicsCommon.h"

class PhysicalWorld {
public:
	PhysicalWorld();
	void populatePhysicalWorld();
	void integrate();

	virtual ~PhysicalWorld();

public:
	btDiscreteDynamicsWorld* m_dynamicsWorld;

private:
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;

	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	btTransform m_groundTransform;
};

#endif /* PHYSICALWORLD_H_ */
