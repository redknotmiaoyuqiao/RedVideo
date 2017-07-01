#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RedGL/RedGL.hpp"
#include "RedGL/File.hpp"
#include "Camera/Camera.hpp"
#include "H264/H264encode.hpp"
#include "H264/H264decoder.hpp"

GLFWwindow* window;

int width = 640;
int height = 480;

int main( void )
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( width, height, "RedVideo", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    TextFile * ReadFile = new TextFile();
    char * v_shader_code = ReadFile->ReadFile("/home/redknot/RedVideo/RedGL/GLSL/v_shader.vert");
    char * f_shader_code = ReadFile->ReadFile("/home/redknot/RedVideo/RedGL/GLSL/f_shader.frag");

    GLShader * v_shader = new GLShader(GL_VERTEX_SHADER,v_shader_code);
    GLShader * f_shader = new GLShader(GL_FRAGMENT_SHADER,f_shader_code);

    GLProgram * program = new GLProgram();
    program->AddShader(v_shader);
    program->AddShader(f_shader);
    program->LinkProgram();

    GLfloat vertices[] = {
        1.0f, 1.0, 0.0f,   // 右上角
        1.0f, -1.0f, 0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f, // 左下角
        -1.0f, 1.0f, 0.0f   // 左上角
    };

    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    GLfloat txtcoor[] = {
        1.0f, 1.0, 0.0f,   // 右上角
        1.0f, 0.0f, 0.0f,  // 右下角
        0.0f, 0.0f, 0.0f, // 左下角
        0.0f, 1.0f, 0.0f   // 左上角
    };

    GLVAO * vao = new GLVAO();

    printf("Size:%d\n",sizeof(indices));
    vao->AddVBO(vertices,sizeof(vertices),0,3);
    vao->AddVBO(txtcoor,sizeof(txtcoor),1,3);
    vao->SetEBO(indices,sizeof(indices));


    GLTexture * y = new GLTexture();
    GLTexture * u = new GLTexture();
    GLTexture * v = new GLTexture();
    GLTexture * rgb = new GLTexture();

    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",width,height);

    H264encode * h264 = new H264encode();
    h264->h264_encoder_init(width,height);
\
    H264Decoder * h264decode = new H264Decoder();
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 )
    {
        glClear( GL_COLOR_BUFFER_BIT );

        unsigned char * yuv420 = camera->read_yuv420_frame();

        /****H264****/
        unsigned char * out;
        int frame_size = h264->h264_compress_frame(10,yuv420,out);
        //printf("%d\n",frame_size);
        RGBData_Define rgbData;
        h264decode->DecodeH264Frames(out,frame_size,&rgbData);

        rgb->SetData(rgbData.databuffer,rgbData.width,rgbData.height,GL_RGB,GL_RGB);

        free(out);
        /****H264****/

        y->SetData(yuv420 , width , height , GL_RED , GL_RED);
        u->SetData(yuv420 + width*height , width / 2,height / 2 , GL_RED,GL_RED);
        v->SetData(yuv420 + width*height + width*height/4,width / 2,height / 2,GL_RED,GL_RED);

        glUseProgram(program->ProgramId);

        glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, y->TextureId);
        glUniform1i(program->GetUniformLocation("y"), 0);

        glActiveTexture(GL_TEXTURE1); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, u->TextureId);
        glUniform1i(program->GetUniformLocation("u"), 1);

        glActiveTexture(GL_TEXTURE2); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, v->TextureId);
        glUniform1i(program->GetUniformLocation("v"), 2);

        glActiveTexture(GL_TEXTURE3); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, rgb->TextureId);
        glUniform1i(program->GetUniformLocation("rgb"), 3);

        vao->DrawVAO();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteVertexArrays(1, &VertexArrayID);
    //glDeleteProgram(programID);

    delete ReadFile;
    delete v_shader;
    delete f_shader;
    delete program;
    delete vao;

    //h264->h264_encoder_uninit();
    delete h264;

    glfwTerminate();

    return 0;
}
