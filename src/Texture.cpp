#include "Texture.h"
#include "GLSL.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

using namespace std;

Texture::Texture() : filename(""), tid(0), fbo(0) {
}

Texture::~Texture() {
}

void Texture::initFBO() {
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &tid);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE_2D, tid);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tid, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Error setting up frame buffer - exiting" << endl;
        exit(0);
    }
}

void Texture::init() {
    unsigned char *data = NULL;
    if (filename.length() > 0) {
        // Load texture
        int w, h, ncomps;
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(filename.c_str(), &w, &h, &ncomps, 0);
        if (!data) {
            cerr << filename << " not found" << endl;
        }
        if (ncomps != 3) {
            cerr << filename << " must have 3 components (RGB)" << endl;
        }
        if ((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
            cerr << filename << " must be a power of 2" << endl;
        }
        width = w;
        height = h;
    }

    if (width == 0 || height == 0)
        cerr << "width or height not set" << endl;

    // Generate a texture buffer object
    glGenTextures(1, &tid);
    // Bind the current texture to be the newly generated texture object
    glBindTexture(GL_TEXTURE_2D, tid);
    // Load the actual texture data
    // Base level is 0, number of channels is 3, and border is 0.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // Generate image pyramid
    glGenerateMipmap(GL_TEXTURE_2D);
    // Set texture wrap modes for the S and T directions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set filtering mode for magnification and minimification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free image, since the data is now on the GPU

    if (data)
        stbi_image_free(data);
}

bool Texture::initDepth() {
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tid, 0);

    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
}


void Texture::setWrapModes(GLint wrapS, GLint wrapT) {
    // Must be called after init()
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::bind(GLint handle) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
    glUniform1i(handle, unit);
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

