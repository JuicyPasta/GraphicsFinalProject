//
// Created by Darrow, Jackson on 11/12/17.
//

#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxShapeExt.h"
#include "foundation/PxMat33.h"
#include "extensions/PxSimpleFactory.h"

using namespace std;
#include <iostream>

using namespace physx;
using namespace std;

//const int WINDOW_WIDTH=1024,
//WINDOW_HEIGHT=768;
//
//static PxPhysics* gPhysicsSDK = NULL;
//static PxDefaultErrorCallback gDefaultErrorCallback;
//static PxDefaultAllocator gDefaultAllocatorCallback;
//static PxSimulationFilterShader gDefaultFilterShader=PxDefaultSimulationFilterShader;
//
//PxScene* gScene = NULL;
//PxReal myTimestep = 1.0f/60.0f;
//PxRigidActor *box;

//void createPhysics () {
//    gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback, PxTolerancesScale() );
//    if(gPhysicsSDK == NULL) {
//        cerr<<"Error creating PhysX3 device."<<endl;
//        cerr<<"Exiting..."<<endl;
//        exit(1);
//    }
//
//    if(!PxInitExtensions(*gPhysicsSDK))
//        cerr<< "PxInitExtensions failed!" <<endl;
//
//}
//PxReal stackZ = 10.0f;
//
//
//void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
//{
//	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
//	for(PxU32 i=0; i<size;i++)
//	{
//		for(PxU32 j=0;j<size-i;j++)
//		{
//			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
//			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
//			body->attachShape(*shape);
//			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
//			gScene->addActor(*body);
//		}
//	}
//	shape->release();
//}
//
//void initPhysics(bool interactive)
//{
//
//	gPvd = PxCreatePvd(*gFoundation);
//	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(0, 5425, 10);
//	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);
//
//	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);
//
//	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
//	gDispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.cpuDispatcher	= gDispatcher;
//	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
//	gScene = gPhysics->createScene(sceneDesc);
//
//	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
//	if(pvdClient)
//	{
//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//	}
//	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//
//	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
//	gScene->addActor(*groundPlane);
//
//}
//
//void stepPhysics(bool interactive)
//{
//	PX_UNUSED(interactive);
//	gScene->simulate(1.0f/60.0f);
//	gScene->fetchResults(true);
//}
//
//void cleanupPhysics(bool interactive)
//{
//	PX_UNUSED(interactive);
//	gScene->release();
//	gDispatcher->release();
//	gPhysics->release();
//	PxPvdTransport* transport = gPvd->getTransport();
//	gPvd->release();
//	transport->release();
//
//	gFoundation->release();
//
//	printf("SnippetHelloWorld done.\n");
//}
//
//void keyPress(unsigned char key, const PxTransform& camera)
//{
//	switch(toupper(key))
//	{
//	case 'B':	createStack(PxTransform(PxVec3(0,0,stackZ-=10.0f)), 10, 2.0f);						break;
//	case ' ':	createDynamic(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0,0,-1))*200);	break;
//	}
//}
//
//int snippetMain(int, const char*const*)
//{
//#ifdef RENDER_SNIPPET
//	extern void renderLoop();
//	renderLoop();
//#else
//	static const PxU32 frameCount = 100;
//	initPhysics(false);
//	for(PxU32 i=0; i<frameCount; i++)
//		stepPhysics(false);
//	cleanupPhysics(false);
//#endif
//
//	return 0;
//}

