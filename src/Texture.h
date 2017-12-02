#pragma once
#ifndef __Texture__
#define __Texture__

#include <glad/glad.h>
#include <string>

class Texture {
public:
	Texture();
	~Texture();
	virtual void setFilename(const std::string &f) { filename = f; }
    virtual void setDimensions(int w, int h) { width = w; height = h; }
	virtual void init();
    virtual void initFBO();
	virtual bool initDepth();
	virtual void setUnit(GLint u) { unit = u; }
	virtual GLint getUnit() const { return unit; }
    virtual GLuint getFBO() const { return fbo; }
	virtual void bind(GLint handle);
	virtual void unbind();
	virtual void setWrapModes(GLint wrapS, GLint wrapT); // Must be called after init()
	virtual GLint getID() const { return tid;}

protected:
	std::string filename;
	int width;
	int height;
	GLuint tid;
    GLuint fbo;
	GLint unit;


};

#endif
