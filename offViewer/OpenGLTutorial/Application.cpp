#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <filesystem>

#include <fstream>
#include <tuple>
#include <string>
#include "OFFReader.h"
#include "Renderer.h"
#include "zpr.h"
using namespace std;

string fileName;

int drawing_style = 0;


Grid* grid = 0;
Renderer renderer;

FilterItem* fis;

int index = 0;

float fov = 60;										                    //Perspective projection parameters
float z_near = -100;
float z_far = 100;

void draw()												                //Render the 3D mesh (GLUT callback function)
{
    renderer.draw(*grid);
}

void viewing(int W, int H)								//Window resize function, sets up viewing parameters (GLUT callback function)
{

    if (H == 0)
        H = 1;

    float ratio = W * 1.0 / W;

    glMatrixMode(GL_PROJECTION);						//1. Set the projection matrix
    glLoadIdentity();
    //gluPerspective(fov,float(W)/H,z_near,z_far);
    gluPerspective(45, ratio, 1, 100);
    glViewport(0, 0, W, H);
    glMatrixMode(GL_MODELVIEW);

    //glViewport(0,0,W,H);								//2. Set the viewport to the entire size of the rendering window
}

string getFileName(int index) {

    return ("m" + to_string(index));

}

void mouseclick(int button, int state, int x, int y)	//Callback for mouse click events. We use these to control the viewpoint interactively.
{
    int keys = glutGetModifiers();						//The mouse events are interpreted as follows:	
    if (keys & GLUT_ACTIVE_CTRL)							// 
        button = GLUT_MIDDLE_BUTTON;						// -left button + move:                         rotate viewpoint
    if (keys & GLUT_ACTIVE_SHIFT)							// -middle button (or left button+Control key): zoom in / out
        button = GLUT_RIGHT_BUTTON;							// -right buttom (or left button+Shift key):    translate viewpoint

    zprMouse(button, state, x, y);							//Use the ZPR library to manipulate the viewpoint. The library sets the modelview  
                                                          //OpenGL matrix based on the mouse events, thereby changing the current viewpoint.
}

void mousemotion(int x, int y)							//Callback for mouse move events. We use these to control the viewpoint interactively.
{
    zprMotion(x, y);										//Pass the current location of the mouse to the ZPR library, to change the viewpoint.

    glutPostRedisplay();									//After each mouse move event, ask GLUT to redraw our window, so we see the viewpoint change.
}

void loadFilter()
{
    fstream filtin;
    string line;
    filtin.open("FilterOutput.csv", ios::in);
    if (filtin)
    {
        getline(filtin, line);
        getline(filtin, line);
        while (!filtin.eof())
        {
            getline(filtin, line);
            vector<string> vec = split(line, ',');
            if (size(vec) == 0)
                continue;
            FilterItem fi;
            fi.cls = vec[1];
            fi.numFaces = stoi(vec[2]);
            fi.numVertices = stoi(vec[3]);
            fi.typeOfFace = vec[4];
            fi.minX = stof(vec[5]);
            fi.maxX = stof(vec[6]);
            fi.minY = stof(vec[7]);
            fi.maxY = stof(vec[8]);
            fi.minZ = stof(vec[9]);
            fi.maxZ = stof(vec[10]);
            fis[stoi(vec[0])] = fi;
        }
        filtin.close();
    }
    else
    {
        std::cout << "No previous filter output found" << endl;
    }
}

void keyboard(unsigned char c, int, int)					//Callback for keyboard events:
{
    switch (c)
    {
    case ' ':											    // space:   Toggle through the various drawing styles of the mesh renderer

        /*index += 1;
        fileName = getFileName(index);
        cout << fileName << endl;//Grab the file, TODO: implement in a better way
        grid = openFile(fileName);*/

        break;

    case 'R':
    {                                           // 'r','R': Reset the viewpoint
        tuple<Grid*, FilterItem> tup;
        tup = openFile("0/" + fileName + "/" + fileName + ".off");
        grid = std::get<0>(tup);
        fis[index] = std::get<1>(tup);

        /*glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        zprInit(0, 0, 0);*/

        break;
    }
    case 'D':
    case 'd':

        drawing_style = (++drawing_style) % 4;
        renderer.setDrawingStyle((Renderer::DRAW_STYLE)drawing_style);
        break;

    case 'o':
    {
    fileName = "FilterOutput.csv";
    fstream filtout;
    filtout.open(fileName, ios::out);
    filtout << "sep=," << endl;
    filtout << "index,class,number of faces, number of vertices, type of faces, minimun X value, maximum X value, minimun Y value, maximum Y value, minimum Z value, maximum Z value" << endl;
    for (int i = 0; i < 100; i++)
    {
        FilterItem fi = fis[i];

        if (fi.typeOfFace == "")
            continue;

        filtout << i;
        filtout << ",";
        filtout << fi.cls;
        filtout << ",";
        filtout << fi.numFaces;
        filtout << ",";
        filtout << fi.numVertices;
        filtout << ",";
        filtout << fi.typeOfFace;
        filtout << ",";
        filtout << fi.minX;
        filtout << ",";
        filtout << fi.maxX;
        filtout << ",";
        filtout << fi.minY;
        filtout << ",";
        filtout << fi.maxY;
        filtout << ",";
        filtout << fi.minZ;
        filtout << ",";
        filtout << fi.maxZ;
        filtout << endl;
    }

    std::cout << "Outputted!" << endl;
    filtout.close();
    break;
    }
    case 's':
    {
        std::cout << "Scanning..." << endl;
        for (int i = 0; i < 100; i++) //TODO scan entire database
        {
            string fln = getFileName(i);
            std::tuple<Grid*, FilterItem> tup = openFile("0/" + fln + "/" + fln + ".off");
            fis[i] = std::get<1>(tup);
        }
        std::cout << "Scanning complete!" << endl;
        break;
    }
    case 'l':
    {
        std::cout << "Loading from output file" << endl;
        loadFilter();
        break;
    }
    }

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    fis = new FilterItem[100];
    loadFilter();

    string input;
    cout << "Please specify the file you want to view:" << endl;
    cin >> input;

    std::tuple<Grid*, FilterItem> tup = openFile(input);
    grid = std::get<0>(tup);
    fis[index] = std::get<1>(tup);

    grid->normalize();									                //7.  Normalize the mesh in the [-1,1] cube. This makes setting the OpenGL projection easier.
    grid->computeFaceNormals();							                //8.  Compute face and vertex normals for the mesh. This allows us to shade the mesh next.
    grid->computeVertexNormals();

    glutInit(&argc, argv);								                //Initialize the GLUT toolkit
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
                                                                        //Ask GLUT to create next windows with a RGB framebuffer and a Z-buffer too
    glutInitWindowSize(500, 500);							            //Tell GLUT how large are the windows we want to create next
    glutCreateWindow(fileName.c_str());	                                //Create our window

    glutMouseFunc(mouseclick);							                //Bind the mouse click and mouse drag (click-and-move) events to callbacks. This allows us
    glutMotionFunc(mousemotion);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(draw);								                //Add a drawing callback to the window
    //glutReshapeFunc(viewing);
    glutMainLoop();										                //Start the event loop that displays the graph and handles window-resize events


    return 0;
}