//
// Created by Darrow, Jackson on 11/12/17.
//
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "Physics.h"
#include "UserData.h"


Physics::Physics() {
    mStepSize = 1.0f / 60.0f;

    this->mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, memoryAllocator, errorCallback);
    if (!mFoundation)
        cerr << "foundation creation failed";

    bool recordMemoryAllocations = true;

    // units
    scale = PxTolerancesScale();

    // debugger
    mPvd = PxCreatePvd(*mFoundation);
    PxPvdTransport *transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
                               scale, recordMemoryAllocations, mPvd);
    if (!mPhysics)
        cerr << ("PxCreatePhysics failed!");

    // serialization
    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
    if (!mCooking)
        cerr << ("PxCreateCooking failed!");

    //Create the scene
    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    if (!sceneDesc.cpuDispatcher) {
        PxDefaultCpuDispatcher *mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
        if (!mCpuDispatcher)
            cerr << "PxDefaultCpuDispatcherCreate failed!" << endl;
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }
    if(!sceneDesc.filterShader)
        sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
        cerr << "createScene failed!" << endl;
    mScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
    mScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
}

Physics::~Physics() {
    mFoundation->release();
    mPhysics->release();
}


void Physics::addSphere(PxVec3 position, UserData *userData) {
    PxMaterial *material = mPhysics->createMaterial(.5, .5, .5);

    PxRigidDynamic *aSphereActor = mPhysics->createRigidDynamic(PxTransform(position));
    PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
    PxShape *aCapsuleShape = PxRigidActorExt::createExclusiveShape(*aSphereActor, PxSphereGeometry(1), *material);

    aCapsuleShape->setLocalPose(relativePose);

    aSphereActor->userData = userData;

    mScene->addActor(*aSphereActor);
}

void Physics::addGround() {
    PxMaterial *material = mPhysics->createMaterial(.5, .5, .5);

    PxVec3 position = PxVec3(0, -1, 0);
    PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
    PxRigidStatic *ground = mPhysics->createRigidStatic(PxTransform(position));
    PxShape *aCapsuleShape = PxRigidActorExt::createExclusiveShape(*ground, PxPlaneGeometry(), *material);
    aCapsuleShape->setLocalPose(PxTransform(relativePose));
    mScene->addActor(*ground);

    int size = 60;

    float corner = sqrt((size/2) * (size/2) * 2) + 5;

    position = PxVec3(0, -1, size);
    PxRigidStatic *left = mPhysics->createRigidStatic(PxTransform(position));
    PxRigidActorExt::createExclusiveShape(*left, PxBoxGeometry(size, 10, 10), *material);
    mScene->addActor(*left);

    position = PxVec3(0, -1, -size);
    PxRigidStatic *right = mPhysics->createRigidStatic(PxTransform(position));
    PxRigidActorExt::createExclusiveShape(*right, PxBoxGeometry(size, 10, 10), *material);
    mScene->addActor(*right);

    position = PxVec3(size, -1, 0);
    PxRigidStatic *front = mPhysics->createRigidStatic(PxTransform(position));
    PxRigidActorExt::createExclusiveShape(*front, PxBoxGeometry(10, 10, size), *material);
    mScene->addActor(*front);

    position = PxVec3(-size, -1, 0);
    PxRigidStatic *back = mPhysics->createRigidStatic(PxTransform(position));
    PxRigidActorExt::createExclusiveShape(*back, PxBoxGeometry(10, 10, size), *material);
    mScene->addActor(*back);

    position = PxVec3(corner, -1, corner);
    PxRigidStatic *goal1 = mPhysics->createRigidStatic(PxTransform(position));
    PxRigidActorExt::createExclusiveShape(*goal1, PxSphereGeometry(5), *material);
    mScene->addActor(*goal1);
}

// rigid body docs http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/RigidBodyOverview.html
bool Physics::advance(PxReal dt) {
    mAccumulator += dt;
    if (mAccumulator < mStepSize)
        return false;

    mAccumulator -= mStepSize;

    mScene->simulate(mStepSize);
    return true;
}

bool Physics::fetchResults() {
    return mScene->fetchResults(true);
}

int Physics::getActors(PxActor **actors) {
    return mScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, actors, 400);
}


