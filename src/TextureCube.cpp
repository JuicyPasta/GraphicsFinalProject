//
// Created by Darrow, Jackson on 11/25/17.
//

#include <OpenGL/OpenGL.h>
#include <glad/glad.h>
#include <cstdlib>
#include "TextureCube.h"
#include "stb_image.h"

TextureCube::TextureCube() {

};

void TextureCube::init(string front, string back, string top, string bottom, string left, string right) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glGenTextures(1, &textureCube);

    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
    load_cube_map_side(textureCube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


bool TextureCube::load_cube_map_side(GLuint texture, GLenum side_target, string file_name) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load(
            file_name.c_str(), &x, &y, &n, force_channels);
    if (!image_data) {
        fprintf(stderr, "ERROR: could not load %s\n", file_name.c_str());
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        fprintf(stderr,
                "WARNING: image %s is not power-of-2 dimensions\n",
                file_name.c_str());
    }

    // copy image data into 'target' side of cube map
    glTexImage2D( side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    free(image_data);
    return true;
}

void TextureCube::init() {

}

void TextureCube::bind(GLint handle) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
    glUniform1i(handle, unit);
}
