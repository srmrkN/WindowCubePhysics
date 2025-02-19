#pragma once

#include "types.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void init();
    void cleanup();
    void stepSimulation(float deltaTime);
    void createBoundaryWalls();
    PhysicsObject createPhysicsObject(int type, const btVector3& position);
    void applyForceToObject(PhysicsObject& obj, const glm::vec2& windowVelocity, const PhysicsSettings& settings);
    void removeObject(PhysicsObject& obj);
    void removeAllObjects();
    void addObject(PhysicsObject& obj);

private:
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};