#include "physics.h"

PhysicsWorld::PhysicsWorld() 
    : collisionConfiguration(nullptr)
    , dispatcher(nullptr)
    , overlappingPairCache(nullptr)
    , solver(nullptr)
    , dynamicsWorld(nullptr) {
}

PhysicsWorld::~PhysicsWorld() {
    cleanup();
}

void PhysicsWorld::init() {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    
    // Нормальная земная гравитация
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    
    // Улучшенные настройки симуляции
    dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
    dynamicsWorld->getSolverInfo().m_numIterations = 10;
    dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
    
    // Настройки стабильности
    dynamicsWorld->getSolverInfo().m_splitImpulse = true;
    dynamicsWorld->getSolverInfo().m_splitImpulsePenetrationThreshold = -0.001f;
    dynamicsWorld->getSolverInfo().m_erp = 0.4f;
    dynamicsWorld->getSolverInfo().m_erp2 = 0.8f;
}

void PhysicsWorld::cleanup() {
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;
    
    dynamicsWorld = nullptr;
    solver = nullptr;
    overlappingPairCache = nullptr;
    dispatcher = nullptr;
    collisionConfiguration = nullptr;
}

void PhysicsWorld::stepSimulation(float deltaTime) {
    for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && !body->isStaticObject()) {
            btVector3 velocity = body->getLinearVelocity();
            btVector3 angVel = body->getAngularVelocity();
            
            // Остановка при малых скоростях
            if (velocity.length() < 0.01f) {
                body->setLinearVelocity(btVector3(0,0,0));
            }
            if (angVel.length() < 0.01f) {
                body->setAngularVelocity(btVector3(0,0,0));
            }
            
            // Ограничение максимальных скоростей
            if (velocity.length() > 30.0f) {
                velocity = velocity.normalized() * 30.0f;
                body->setLinearVelocity(velocity);
            }
            if (angVel.length() > 15.0f) {
                angVel = angVel.normalized() * 15.0f;
                body->setAngularVelocity(angVel);
            }
        }
    }
    dynamicsWorld->stepSimulation(deltaTime, 10);
}

void PhysicsWorld::createBoundaryWalls() {
    float thickness = 0.2f;
    btVector3 sizes[6] = {
        btVector3(thickness, BOUNDARY_SIZE*2, BOUNDARY_SIZE*2),
        btVector3(thickness, BOUNDARY_SIZE*2, BOUNDARY_SIZE*2),
        btVector3(BOUNDARY_SIZE*2, thickness, BOUNDARY_SIZE*2),
        btVector3(BOUNDARY_SIZE*2, thickness, BOUNDARY_SIZE*2),
        btVector3(BOUNDARY_SIZE*2, BOUNDARY_SIZE*2, thickness),
        btVector3(BOUNDARY_SIZE*2, BOUNDARY_SIZE*2, thickness)
    };

    btVector3 positions[6] = {
        btVector3(BOUNDARY_SIZE, 0, 0),
        btVector3(-BOUNDARY_SIZE, 0, 0),
        btVector3(0, BOUNDARY_SIZE, 0),
        btVector3(0, -BOUNDARY_SIZE, 0),
        btVector3(0, 0, BOUNDARY_SIZE),
        btVector3(0, 0, -BOUNDARY_SIZE)
    };

    btCompoundShape* boundaryShape = new btCompoundShape();
    
    for(int i = 0; i < 6; ++i) {
        btBoxShape* wall = new btBoxShape(sizes[i] * 0.5f);
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(positions[i]);
        boundaryShape->addChildShape(transform, wall);
    }

    btDefaultMotionState* boundaryMotionState = new btDefaultMotionState();
    btRigidBody::btRigidBodyConstructionInfo boundaryInfo(0, boundaryMotionState, boundaryShape, btVector3(0,0,0));
    btRigidBody* boundaryBody = new btRigidBody(boundaryInfo);
    
    boundaryBody->setRestitution(0.8f);
    boundaryBody->setFriction(0.1f);
    boundaryBody->setCollisionFlags(boundaryBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

    dynamicsWorld->addRigidBody(boundaryBody);
}

PhysicsObject PhysicsWorld::createPhysicsObject(int type, const btVector3& position) {
    PhysicsObject obj;
    obj.type = type;
    
    float mass = 1.0f;
    btVector3 localInertia(0, 0, 0);
    
    switch(type) {
        case 0: // Куб
            obj.shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
            obj.color = glm::vec3(0.8f, 0.3f, 0.2f);
            mass = 1.0f;
            break;
        case 1: // Сфера
            obj.shape = new btSphereShape(0.5f);
            obj.color = glm::vec3(0.2f, 0.8f, 0.3f);
            mass = 0.8f;
            break;
        case 2: // Пирамида
            obj.shape = new btConeShape(0.4f, 1.0f);
            obj.color = glm::vec3(0.3f, 0.2f, 0.8f);
            mass = 0.8f;
            break;
    }

    obj.shape->calculateLocalInertia(mass, localInertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(position);
    
    obj.motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, obj.motionState, obj.shape, localInertia);

    // Настраиваем параметры в зависимости от типа
    if (type == 2) { // Пирамида
        rbInfo.m_restitution = 0.3f;
        rbInfo.m_friction = 0.9f;
        rbInfo.m_rollingFriction = 0.8f;
        rbInfo.m_spinningFriction = 0.8f;
    }
    
    obj.rigidBody = new btRigidBody(rbInfo);
    obj.rigidBody->setDamping(0.1f, 0.1f);  // Небольшое затухание движения
    obj.rigidBody->setActivationState(DISABLE_DEACTIVATION);
    
    // Включаем CCD для быстро движущихся объектов
    obj.rigidBody->setCcdMotionThreshold(1e-7f);
    obj.rigidBody->setCcdSweptSphereRadius(0.25f);
    
    return obj;
}

void PhysicsWorld::applyForceToObject(PhysicsObject& obj, const glm::vec2& windowVelocity, const PhysicsSettings& settings) {
    if (!obj.rigidBody) return;

    // Увеличиваем силу импульса
    btVector3 centralImpulse(
        windowVelocity.x * settings.horizontalScale * settings.forceScale * 300.0f, // Увеличили множитель
        -windowVelocity.y * settings.verticalScale * settings.forceScale * 300.0f,  // с 100 до 300
        windowVelocity.y * settings.zScale * settings.forceScale * 300.0f
    );

    obj.rigidBody->applyCentralImpulse(centralImpulse);

    btVector3 torque(
        windowVelocity.y * 0.5f,      // Увеличили вращение
        windowVelocity.x * 0.5f,
        (windowVelocity.x + windowVelocity.y) * 0.25f
    );

    float mass = obj.rigidBody->getMass();
    torque *= mass * 0.8f;  // Увеличили влияние массы

    float maxTorque = 8.0f;  // Увеличили максимальный момент
    if (torque.length() > maxTorque) {
        torque = torque.normalized() * maxTorque;
    }

    // Добавляем случайное возмущение для предотвращения стояния на грани
    btVector3 randomTorque(
        (rand() % 100 - 50) * 0.001f,
        (rand() % 100 - 50) * 0.001f,
        (rand() % 100 - 50) * 0.001f
    );
    
    obj.rigidBody->applyTorqueImpulse(torque + randomTorque);
}

void PhysicsWorld::removeObject(PhysicsObject& obj) {
    if (obj.rigidBody) {
        dynamicsWorld->removeRigidBody(obj.rigidBody);
        delete obj.rigidBody;
        delete obj.motionState;
        delete obj.shape;
        
        obj.rigidBody = nullptr;
        obj.motionState = nullptr;
        obj.shape = nullptr;
    }
}

void PhysicsWorld::removeAllObjects() {
    // Удаляем все объекты из мира
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }
}

void PhysicsWorld::addObject(PhysicsObject& obj) {
    dynamicsWorld->addRigidBody(obj.rigidBody);
}