// Psychadelic Roll.cpp : Currently in Development Phase

/*********************************** Include ********************************************/

#include "stdafx.h"
#include "dim.h"
#include "Psychadelic Roll.h"
//#include <random>

/******************************** End Include ********************************************/

void npcBall(Object * const self);

GLfloat BLUE[3] = { 0.1, 0.0, 0.6 };
GLfloat RED[3] = { 0.6, 0.0, 0.1 };
Object ball(Object::Sphere, 0.0, -0.7, 8.0, 0.2, 30, BLUE, ballUpdate);
Object platform(Object::Rectangle, -10.0, -1.0, -10.0, 20.0, 0.1, 20.0, RED);

Object npcBallObj(Object::Sphere, 0.0, -0.7, 2.0, 0.1, 20, BLUE, npcBall);

Vector ballVector;

bool debugMode;

bool grabMouse = true;

GLfloat gravity = 0;

GLfloat viewY = 1.25;

void npcBall(Object * const self)	{

	/*static Vector npcVector;

	npcVector.x = ball.x - npcBallObj.x;
	npcVector.z = ball.z - npcBallObj.z;
	npcVector.update(Vector::vector);
	int checkZero = std::rand() % 600 - 300;
	if (checkZero < 10 || checkZero > -10)	checkZero = 300;
	float changeSpeed = (float)1 / checkZero / 100000000000000;
	npcVector.speed += changeSpeed;
	npcVector.speed *= 0.01;
	int changeAngle = std::rand() % 5;
	npcVector.angle += changeAngle;
	
	if (debugMode)	std::cout << "X: " << (*self).x << ", Y: " << (*self).y << ", Z: " << (*self).z << ".\n";

	npcVector.update(Vector::plane);

	npcBallObj.x += npcVector.x;
	npcBallObj.z += npcVector.z;*/

	//if (!npcBallObj.collideWith(platform))	npcBallObj.y -= 0.02;

}

void ballUpdate(Object * const self)	{

	ballVector.speed *= 0.995;
	//ball.degreesRotate += (int)ballVector.speed * 288;
	ballVector.update(Vector::plane);
	if (ballVector.speed >= 0)	{
		ball.x += ballVector.x;
		ball.z += ballVector.z;
	}

	//if (debugMode)	std::cout << "X: " << ball.x << ", Y: " << ball.y << ", Z: " << ball.z << ".\n";

	//Object * all[stage.MAX_OBJECTS];
	//stage.getAll(all, stage.MAX_OBJECTS, ball.ID);

	gravity -= 0.008;
	//if (gravity < 0.01 && gravity > -0.01 && ball.y < -0.68)	gravity = 0;
	if (!ball.collideWith(platform) || gravity > 0)	{
		ball.y += gravity;
	}
	else	{
		ball.y = -0.7;
		//gravity = -0.8 * gravity;
	}

	if (ball.y < -10.0)	{
		Object ball(Object::Sphere, 0.0, -0.7, 8.0, 0.2, 30, BLUE, ballUpdate);
		ballVector = Vector(0.0, 0);
		gravity = 0;
	}

	Vector lookAtVector = ballVector;
	lookAtVector.speed = -1.75;
	lookAtVector.update(Vector::plane);
	camera.lookAt(ball, lookAtVector.x, viewY, lookAtVector.z);

}

void keyAct()	{
	if (keys['w'])	ballVector.speed += 0.001;
	if (keys['s'])	ballVector.speed *= 0.95;
	if (keys['a'])	ballVector.angle -= 2;
	if (keys['d'])	ballVector.angle += 2;
	if (keys['q'])	viewY -= 0.01;
	if (keys['e'])	viewY += 0.01;
	if (keys['r'])	{
		Object ball(Object::Sphere, 0.0, -0.7, 8.0, 0.2, 30, BLUE, ballUpdate);
		ballVector = Vector();
	}
	if (keys[' '])	{
		if (ball.collideWith(platform))	{
			gravity = 0.2;
		}
	}
}

void keyPressed(unsigned char key, int x, int y)	{
	keys[key] = true;
	if (key == '3')	debugMode = !debugMode;
}
void specKeyPressed(int key, int x, int y)	{
	if (key == GLUT_KEY_F11)	{
		if (glutGet(GLUT_SCREEN_WIDTH) != glutGet(GLUT_WINDOW_WIDTH) && glutGet(GLUT_SCREEN_HEIGHT) != glutGet(GLUT_WINDOW_HEIGHT))	glutFullScreen();
		else	{
			glutReshapeWindow(800, 600);
			glutPositionWindow(200, 200);
		}
	}
}
void keyReleased(unsigned char key, int x, int y)	{ keys[key] = false; }

void mouseMoved(int x, int y)	{
	
	static int counter = 0;

	if (counter > 2)	{
		ballVector.angle += (float)((float)x - glutGet(GLUT_WINDOW_WIDTH) / 2.0f) * MOUSE_SEN;
		viewY += (float)((float)y - glutGet(GLUT_WINDOW_HEIGHT) / 2.0f) / 20.0 * MOUSE_SEN;
		if (viewY < -0.2)	viewY = -0.9;
		glutWarpPointer((float)glutGet(GLUT_WINDOW_WIDTH) / 2.0f, (float)glutGet(GLUT_WINDOW_HEIGHT) / 2.0f);
		counter = 0;
	}

	counter++;

}

void frustumUpdate(int width, int height)	{
	glViewport(0, 0, width, height);
	//glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1 * (float) width / height, (float) width / height, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void initialize(int iArgc, char ** cppArgv)	{
	// Window
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Psychadelic Roll! - Cosine Gaming");

	// Standard
	glClearColor(0.12, 0.03, 0.26, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.33, 1.33, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);

	// Lighting
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	GLfloat qaDiffuseLight[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat qaLightPosition[] = {0.0, 1.0, 10.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaFullAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaWhite);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	stage.genTexture("Texture.tga", 500, 500, false);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	setKeyboardUpdateFunc(keyAct);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialFunc(specKeyPressed);
	glutReshapeFunc(frustumUpdate);
	glutPassiveMotionFunc(mouseMoved);
	glutTimerFunc(10, update, 0);
	glutDisplayFunc(drawStage);
}

int main(int iArgc, char ** cppArgv)	{
	stage.addInstance(ball);
	stage.addInstance(Object(Object::Sphere, 0.2, 0.2, 0.0, 0.2, 30, BLUE));
	
	stage.addInstance(platform);
	//stage.addInstance(Object(Object::Rectangle, -1.25, -1.0, -1.0, 3.0, 3.0, 20.0, BLUE));

	stage.addInstance(npcBallObj);

	initialize(iArgc, cppArgv);
	glutMainLoop();
	return 0;
}



// I was making the NPC correct.