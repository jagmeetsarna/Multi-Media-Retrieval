#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OFFReader.h"
#include "Renderer.h"
using namespace std;

const char* fileName;

Grid* grid = 0;
Renderer renderer;
GLFWwindow* window;

float fov = 60;										                    //Perspective projection parameters
float z_near = -100;
float z_far = 100;

void draw()												                //Render the 3D mesh (GLUT callback function)
{
    renderer.draw(*grid);
}

void viewing(int W, int H)								                //Window resize function, sets up viewing parameters (GLUT callback function)
{                                                                       // TODO: THIS DOES NOT REALLY WORK RN
    glMatrixMode(GL_PROJECTION);						                //1. Set the projection matrix
    glLoadIdentity();
    gluPerspective(fov, float(W) / H, z_near, z_far);

    glViewport(0, 0, W, H);								                //2. Set the viewport to the entire size of the rendering window
}

int main(int argc, char* argv[])
{

    fileName = "m0.off";                                                //Grab the file, TODO: implement in a better way
    grid = openFile(fileName);


    glutInit(&argc, argv);								                //Initialize the GLUT toolkit
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


                                                                        //Ask GLUT to create next windows with a RGB framebuffer and a Z-buffer too
    glutInitWindowSize(500, 500);							            //Tell GLUT how large are the windows we want to create next
    glutCreateWindow(fileName);	                                        //Create our window

    glutDisplayFunc(draw);								                //Add a drawing callback to the window
    //glutReshapeFunc(viewing);
    glutMainLoop();										                //Start the event loop that displays the graph and handles window-resize events

    return 0;
}