//
// Created by Darrow, Jackson on 12/2/17.
//

#ifndef INC_3DBILLIARDS_USERDATA_H
#define INC_3DBILLIARDS_USERDATA_H

#include "Program.h"
#include "Material.h"

#include <PxPhysicsAPI.h>
#include <foundation/PxVec2.h>
#include <foundation/PxVec3.h>

using namespace std;

class UserData {
public:
    int ballNum = 0;
    map<PxGeometryType::Enum, shared_ptr<Material>> materialMap;
    float time = -2.f;
};

#endif //INC_3DBILLIARDS_USERDATA_H
