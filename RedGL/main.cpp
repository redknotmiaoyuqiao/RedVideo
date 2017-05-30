#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

using namespace std;

int main(int argc, char *argv[])
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    cout << "Hello World!" << endl;
    return 0;
}
