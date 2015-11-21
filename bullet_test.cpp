/*
 * bullet_test.cpp
 *
 *  Created on: 17.09.2015 г.
 *      Author: martin
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include "btBulletDynamicsCommon.h"
#include <stdio.h>

#include "scene_drawings.h"
#include "PhysicalWorld.h"

PhysicalWorld world;

int main(int argc, char** argv)
{

	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glewInit();


	opengl_scene::initGLScene();
//	opengl_scene::drawScene();

	glutMainLoop();

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	///-----cleanup_end-----
	printf("Press a key to exit\n");
	getchar();
}

