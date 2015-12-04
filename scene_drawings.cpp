/*
 * scene_drawings.cpp
 *
 *  Created on: 11.10.2015 г.
 *      Author: martin
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include "scene_drawings.h"

#include <cstdio>

namespace opengl_scene {

float vert = 1.;

void initGLScene()
{
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("Bullet simulation");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
//	glutTimerFunc(500, timer_function, 1);
	glutIdleFunc(idle_function);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_TRUE;
	glewInit();

//	glDisable(GL_CULL_FACE_MODE);

	glClearColor(1.0, 1.0, 1.0, 0.0);
//	glDepthFunc(GL_NEVER);
	//		setup();
}

void drawScene()
{
	printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(.0, vert, 20.0, 0. , 0.0, 0. , 0.0, 1.0, 0.0);

	for (int i=0; i < world.m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		btCollisionObject* obj = world.m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		btScalar m[16];
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}

		printf("Object[%d]: X=%f Y=%f Z=%f\n", i,
				trans.getOrigin().getX(),
				trans.getOrigin().getY(),
				trans.getOrigin().getZ());

		if(i == 0)		// base
		{
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(	trans.getOrigin().getX(),
							trans.getOrigin().getY(),
							trans.getOrigin().getZ());
			glBegin(GL_TRIANGLES);
			glVertex3f(0, 0, 0);
			glVertex3f(50, 0, -50);
			glVertex3f(-50, 0, -50);
			glVertex3f(0, 0, 0);
			glVertex3f(50, 0, 50);
			glVertex3f(-50, 0, 50);
			glVertex3f(0, 0, 0);
			glVertex3f(50, 0, -50);
			glVertex3f(50, 0, 50);
			glVertex3f(0, 0, 0);
			glVertex3f(-50, 0, -50);
			glVertex3f(-50, 0, 50);
			glEnd();

			glTranslatef(	-trans.getOrigin().getX(),
							-trans.getOrigin().getY(),
							-trans.getOrigin().getZ());
			continue;
		}

		glTranslatef(	trans.getOrigin().getX(),
						trans.getOrigin().getY(),
						trans.getOrigin().getZ());
		trans.getOpenGLMatrix(m);
		glPushMatrix();
		glMultMatrixf((GLfloat*)m);

		drawPrimitive(obj);

		glPopMatrix();
		glTranslatef(	-trans.getOrigin().getX(),
						-trans.getOrigin().getY(),
						-trans.getOrigin().getZ());

	}

	glutSwapBuffers();
}

void drawPrimitive(btCollisionObject *p)
{
	btRigidBody* body = btRigidBody::upcast(p);

	// the shape is a sphere
	btSphereShape *pSphere = dynamic_cast<btSphereShape *>(body->getCollisionShape());
	if(pSphere)
	{
		glColor3f(1.0, 0.0, 0.0);
		glutWireSphere(pSphere->getRadius(), 20, 10);
		return;
	}

	// the shape is a cylinder
	btCylinderShape *pCyl = dynamic_cast<btCylinderShape *>(body->getCollisionShape());
	static GLUquadric *quadric = NULL;
	if(pCyl)
	{
		if(!quadric)
		{
			quadric = gluNewQuadric();
		}

		glColor3f(.0, .0, 1.0);
		gluCylinder(quadric, 1, 1, 6, 10, 10);
		return;
	}

	btBoxShape *pBox= dynamic_cast<btBoxShape *>(body->getCollisionShape());
	if(pBox)
	{
		btVector3 extensions = pBox->getHalfExtentsWithMargin();
//		glutWireCube(extensions.getX() * 2);

		glBegin(GL_QUADS);
		// upper and lower walls

//		// left and right walls
		for(int i = -1; i < 2; i+=2)
		{
			glColor3f(0, i > 0 ? 1 : 0.5, 0);
//			glBegin(GL_LINE_LOOP);
			glVertex3f(i * extensions.getX(), -extensions.getY(), -extensions.getZ());
			glVertex3f(i * extensions.getX(), -extensions.getY(), extensions.getZ());
			glVertex3f(i * extensions.getX(), extensions.getY(), extensions.getZ());
			glVertex3f(i * extensions.getX(), extensions.getY(), -extensions.getZ());
//			glEnd();
		}

		for(int i = -1; i < 2; i+=2)
		{
			glColor3f(i > 0 ? 1 : 0.5, 0, 0);
//			glBegin(GL_LINE_LOOP);
			glVertex3f(-extensions.getX(), i * extensions.getY(), -extensions.getZ());
			glVertex3f(extensions.getX(), i * extensions.getY(), -extensions.getZ());
			glVertex3f(extensions.getX(), i * extensions.getY(), extensions.getZ());
			glVertex3f(-extensions.getX(), i * extensions.getY(), extensions.getZ());
//			glEnd();
		}
//
//		// back and forth walls
//		for(int i = -1; i < 2; i+=2)
//		{
////			glBegin(GL_LINE_LOOP);
//			glColor3f(0, 0, i > 0 ? 1 : 0.5);
//			glVertex3f(-extensions.getX(), extensions.getY(), i * extensions.getZ());
//			glVertex3f(extensions.getX(), extensions.getY(), i * extensions.getZ());
//			glVertex3f(extensions.getX(), -extensions.getY(), i * extensions.getZ());
//			glVertex3f(-extensions.getX(), -extensions.getY(), i * extensions.getZ());
////			glEnd();
//		}
		glEnd();

		return;
	}

}

void resize (int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, (float)w/(float)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'W':
	case 'w':
		vert += 0.5;
		break;
	case 'S':
	case 's':
		vert -= 0.5;
		break;
	default:
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
//   if (key == GLUT_KEY_RIGHT);
//   if (key == GLUT_KEY_LEFT);

   glutPostRedisplay();
}

void timer_function(int value)
{
	printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	glutTimerFunc(1000, timer_function, 1);
}

void idle_function()
{
	static int count = 0;
	printf("%s %s %d count=%d\n", __FILE__, __FUNCTION__, __LINE__, count++);

	world.integrate();
	drawScene();

}

}

