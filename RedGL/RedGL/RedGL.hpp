#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include "stdio.h"

class GLShader
{
private:
public:
    GLuint ShaderId;

    GLShader(GLenum type, const GLchar* src);
};

class GLProgram
{
private:
public:
    GLuint ProgramId;
    std::vector<GLShader *> ShaderList;

    GLProgram();
    void AddShader(GLShader * shader);
    void LinkProgram();
};


