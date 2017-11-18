//
// Created by Darrow, Jackson on 11/12/17.
//

#ifndef INC_3DBILLIARDS_PHYSICS_H
#define INC_3DBILLIARDS_PHYSICS_H

#include <ctype.h>
#include <iostream>

#include <PxPhysicsAPI.h>
#include <foundation/PxVec2.h>
#include <foundation/PxVec3.h>

#include "MemoryAllocator.h"
#include "ErrorCallback.h"


using namespace physx;
using namespace std;

// http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/API.html#introduction


class Physics {
public:
    Physics();
    ~Physics();

    void addSphere(PxVec3 position);
    void addGround();
    bool advance(PxReal dt);
    bool fetchResults();
    int getActors(PxActor** actors);


private:
    ErrorCallback errorCallback;
    MemoryAllocator memoryAllocator;

    double mAccumulator = 0.0f;
    double mStepSize;

    PxFoundation *mFoundation;
    PxPhysics *mPhysics;
    PxTolerancesScale scale;

    PxScene *mScene;

    PxPvd *mPvd;
    PxCooking *mCooking;
};


#endif //INC_3DBILLIARDS_PHYSICS_H
