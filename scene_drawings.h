/*
 * scene_drawings.h
 *
 *  Created on: 11.10.2015 г.
 *      Author: martin
 */

#ifndef SCENE_DRAWINGS_H_
#define SCENE_DRAWINGS_H_

#include "PhysicalWorld.h"

extern PhysicalWorld world;

namespace opengl_scene {

void initGLScene();
void drawScene();
void resize	(int w, int h);
void keyInput(unsigned char key, int x, int y);
void specialKeyInput(int key, int x, int y);
void timer_function(int value);
void idle_function();
void drawPrimitive(btCollisionObject *p);

}



#endif /* SCENE_DRAWINGS_H_ */
