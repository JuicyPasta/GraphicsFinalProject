//
// Created by Darrow, Jackson on 12/5/17.
//

#ifndef INC_3DBILLIARDS_MATERIAL_H
#define INC_3DBILLIARDS_MATERIAL_H

#include <memory>
#include "Program.h"
#include "Texture.h"

using namespace std;

class Material {
public:
    shared_ptr<Program> shader;
    shared_ptr<Texture> diffuseTex;
    shared_ptr<Texture> specularTex;
    int material;
};


#endif //INC_3DBILLIARDS_MATERIAL_H
