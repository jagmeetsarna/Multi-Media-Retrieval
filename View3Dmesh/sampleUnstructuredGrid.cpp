#include <GL/glut.h>									//GLUT library
#include "UnstructuredGrid.h"
#include "SimpleRenderer.h"



float Xmin, Xmax;										//Range of variable x that we sample
float Ymin, Ymax;										//Range of variable y that we sample


SimpleRenderer		renderer;
UnstructuredGrid*	grid = 0;



void viewing(int W, int H)								//Window resize function, sets up viewing parameters (GLUT callback function)
{
	glMatrixMode(GL_MODELVIEW);							//1. Set the modelview matrix (including the camera position and view direction)
	glLoadIdentity ();										
	
	glMatrixMode (GL_PROJECTION);						//2. Set the projection matrix
	glLoadIdentity ();	
	glOrtho(Xmin,Xmax,Ymin,Ymax,-0.1,1);				//   Use an orthographic projection which nicely encompasses our entire grid

	glViewport(0,0,W,H);								//3. Set the viewport to the entire size of the rendering window
}



void draw()												//Render the height plot (GLUT callback function)
{
	renderer.draw(*grid);
}


UnstructuredGrid* createGrid()							//Create an unstructured grid. For illustration purposes, we read a triangle grid
{														//stored in an external text file. The text file has the following structure:
	FILE* fp = fopen("DATA/unstructuredGrid.txt","r");	//    number-of-points  number-of-triangles
	int P,C;											//    <point-id x y z> ...
	fscanf(fp,"%d %d",&P,&C);							//    <cell-id  vertex-id-1 vertex-id-2 vertex-id-3> ...
	
	UnstructuredGrid* grid = new UnstructuredGrid(P,C);	//After we read the number of points and cells, we can create the unstructured grid
	
	Xmin=1.0e6; Xmax=-1.0e6;		
	Ymin=1.0e6; Ymax=-1.0e6;
	for(int i=0;i<P;++i)								//Read all grid points. Also compute the range [Xmin,Xmax],[Ymin,Ymax] of the vertex coordinates.
	{													//We will use this range to set the projection so that the entire grid is shown in the window.
		int point_id;
		float p[2];
		
		fscanf(fp,"%d %f %f",&point_id,&p[0],&p[1]);
		grid->setPoint(i,p);
		
		Xmin = min(Xmin,p[0]); Xmax = max(Xmax,p[0]);
		Ymin = min(Ymin,p[1]); Ymax = max(Ymax,p[1]);
	}				

	for(int i=0;i<C;++i)								//Read all grid cells (triangles)
	{
		int cell_id,cell[3];
		
		fscanf(fp,"%d %d %d %d",&cell_id,&cell[0],&cell[1],&cell[2]);
		grid->setCell(i,cell);
	}				
	
	fclose(fp);
	
	return grid;
}


int main(int argc,char* argv[])							//Main program
{
   glutInit(&argc, argv);								//1. Initialize the GLUT toolkit
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);			//2. Ask GLUT to create next windows with a RGB framebuffer and a Z-buffer too
   glutInitWindowSize(500,500);							//3. Tell GLUT how large are the windows we want to create next
   glutCreateWindow("5. Unstructured grid");			//4. Create our window
   
   grid = createGrid();									//5. Create the unstructured grid
   
   glutDisplayFunc(draw);								//6. Add a drawing callback to the window	
   glutReshapeFunc(viewing);							//7. Add a resize callback to the window
   glutMainLoop();										//8. Start the event loop that displays the graph and handles window-resize events
   
   return 0;
}




