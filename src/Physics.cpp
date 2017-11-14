//
// Created by Darrow, Jackson on 11/12/17.
//
#include "Physics.h"


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


void Physics::addSphere() {
    PxVec3 position = PxVec3(0, 1, -1);
    PxMaterial *material = mPhysics->createMaterial(.5, .5, .5);

    PxRigidDynamic *aSphereActor = mPhysics->createRigidDynamic(PxTransform(position));
    PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
    PxShape *aCapsuleShape = PxRigidActorExt::createExclusiveShape(*aSphereActor, PxSphereGeometry(3), *material);

    aCapsuleShape->setLocalPose(relativePose);
    PxRigidBodyExt::updateMassAndInertia(*aSphereActor, 5);

    // pass this in, connects to game
    int *data;
    aSphereActor->userData = data;

    mScene->addActor(*aSphereActor);
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
    // true means block until finished
    return mScene->fetchResults(true);
    // automatically calls simulation event callbacks
}


