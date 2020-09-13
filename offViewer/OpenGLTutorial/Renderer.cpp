#include "Renderer.h"
#include "Grid.h"
#include <GL/glut.h>										//GLUT library



void Renderer::draw(Grid& g_)								//Draw an unstructured grid, in the style indicated by 'drawing_style'
{
	glClearColor(1, 1, 1, 1);								//Clear the frame and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);									//Ask OpenGL to automatically normalize normal-vectors when we scale the model
															//This is required to get correct shading for various model scales.

	Grid& g = (Grid&)g_;

	switch (draw_style)										//Render the grid, based on the current drawing style
	{														//Currently only implemented the grid version
	case DRAW_GRID:
		glColor3f(1, 1, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		drawGrid(g);
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawGrid(g);
		break;
	}

	glutSwapBuffers();
}



void Renderer::drawGrid(Grid& g)							//Draw 'g' without shading
{
	glDisable(GL_LIGHTING);

	for (int i = 0; i < g.numCells(); ++i)					//Draw all cells in the grid
	{
		int   cell[10];										//

		int nvertices = g.getCell(i, cell);					//Get the 'nvertices' vertex-IDs of i-th cell
		if (nvertices != 3 && nvertices != 4)				//We only handle here drawing of triangle and quad cells.
			continue;										//It is quite simple to extend this to other cell types.

		if (nvertices == 3)									//Triangle cells:
			glBegin(GL_TRIANGLES);
		else //nvertices==4	
			glBegin(GL_QUADS);								//Quad cells:		

		for (int j = 0; j < nvertices; ++j)					//Render current cell
		{
			float x[3];
			g.getPoint(cell[j], x);							//Get vertex coordinates of j-th vertex of i-th cell		
			glVertex3f(x[0], x[1], x[2]);					//Pass this coordinate to OpenGL
		}
		glEnd();											//Finish rendering current cell
	}
}




