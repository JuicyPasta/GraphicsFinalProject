//
// Created by Darrow, Jackson on 11/25/17.
//

#ifndef INC_3DBILLIARDS_TEXTURECUBE_H
#define INC_3DBILLIARDS_TEXTURECUBE_H

//#include <OpenGL/OpenGL.h>
#include <string>
#include "Texture.h"

using namespace std;

class TextureCube : public Texture {
public:
    TextureCube();

    bool load_cube_map_side(GLuint texture, GLenum side_target, string file_name);

    void init(string front, string back, string top, string bottom, string left, string right);
    void init();

//    virtual ~TextureCube();
//    void setDimensions(int w, int h) { width = w; height = h; }
//    void initFBO();
//    void setUnit(GLint u) { unit = u; }
//    GLint getUnit() const { return unit; }
//    GLuint getFBO() const { return fbo; }
    void bind(GLint handle);
//    void unbind();
//    void setWrapModes(GLint wrapS, GLint wrapT); // Must be called after init()
    GLint getID() const { return textureCube;}



private:
    // top bottom left right front back
    GLuint textureCube;
    GLuint fbo[6];
};


#endif //INC_3DBILLIARDS_TEXTURECUBE_H
