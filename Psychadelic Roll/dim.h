#pragma once
#include <iostream>
#include <complex>
#include <glut.h>
#include "stb_image.cpp"

/* Forward Declarations of this Header */
class Object;
class Camera;
class Stage;
class Vector;
/*      End Forward Declarations       */



/*           Loosies               */

GLfloat qaBlack[]		= {0.0, 0.0, 0.0, 1.0};
GLfloat qaGreen[]		= {0.0, 1.0, 0.0, 1.0};
GLfloat qaRed[]			= {1.0, 0.0, 0.0, 1.0};
GLfloat qaBlue[]		= {0.0, 0.0, 1.0, 1.0};
GLfloat qaWhite[]		= {1.0, 1.0, 1.0, 1.0};
GLfloat qaLowAmbient[]	= {0.2, 0.2, 0.2, 1.0};
GLfloat qaFullAmbient[]	= {1.0, 1.0, 1.0, 1.0};
GLfloat qaStdDiffuse[]	= {0.8, 0.8, 0.8, 1.0};


void nullFunc()	{}
void nullFunc2(Object * const self)	{}
GLfloat colornull[3] = { 1.0 };

void (*keyboardUpdateFunc)() = nullFunc;

/*        End Loosies           */

class Object	{
public:
	enum object_type { Blank=-1, Sphere, Rectangle };
	object_type type;
	int ID;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat color[4];
	GLfloat degreesRotate;
	void (*updateF)(Object * const self);
	// Rectangle
	GLfloat width;
	GLfloat height;
	GLfloat length;
	// Sphere
	GLfloat radius;
	int polygons;

	Object(object_type _type, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _width, GLfloat _height, GLfloat _length, GLfloat _color[]=colornull, void (*updateFunc)(Object * const self)=nullFunc2)	{
		// Rectangle
		type = _type;
		x = _x;
		y = _y;
		z = _z;
		degreesRotate = 0;
		for (int i=0; i<3; i++)	{ color[i] = _color[i]; }
		color[3] = 1.0;
		width = _width;
		height = _height;
		length = _length;
		updateF = updateFunc;
	}
	Object(object_type _type, GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _radius, int _polygons, GLfloat _color[]=colornull, void (*updateFunc)(Object * self)=nullFunc2)	{
		// Sphere
		type = _type;
		x = _x;
		y = _y;
		z = _z;
		degreesRotate = 0;
		for (int i=0; i<3; i++)	{ color[i] = _color[i]; }
		color[3] = 1.0;
		radius = _radius;
		polygons = _polygons;
		updateF = updateFunc;
	}
	Object()	{
		type = Blank;
		ID = 0;
		x = 0.0;
		y = 0.0;
		z = 0.0;
		degreesRotate = 0;
		for (int i=0; i<4; i++)	{ color[i] = 0.0; }
		width = 0.0;
		height = 0.0;
		radius = 0.0;
		polygons = 0;
	}
	void update()	{
		updateF(this);
	}
	bool collideWith(Object other)	{


		GLfloat Xleast;
		GLfloat Yleast;
		GLfloat Zleast;
		GLfloat Xextent;
		GLfloat Yextent;
		GLfloat Zextent;

		GLfloat OtherXleast;
		GLfloat OtherYleast;
		GLfloat OtherZleast;
		GLfloat OtherXextent;
		GLfloat OtherYextent;
		GLfloat OtherZextent;


		if (other.type == Rectangle)	{
			OtherXleast = other.x;
			OtherYleast = other.y;
			OtherZleast = other.z;
			OtherXextent = other.x + other.width;
			OtherYextent = other.y + other.height;
			OtherZextent = other.z + other.length;
		}
		if (other.type == Sphere)	{
			OtherXleast = other.x - other.radius;
			OtherYleast = other.y - other.radius;
			OtherZleast = other.z - other.radius;
			OtherXextent = other.x + other.radius;
			OtherYextent = other.y + other.radius;
			OtherZextent = other.z + other.radius;
		}

		if (type == Rectangle)	{
			Xleast = x;
			Yleast = y;
			Zleast = z;
			Xextent = x + width;
			Yextent = y + height;
			Zextent = z + length;
		}
		if (type == Sphere)	{
			Xleast = x - radius;
			Yleast = y - radius;
			Zleast = z - radius;
			Xextent = x + radius;
			Yextent = y + radius;
			Zextent = z + radius;
		}


		return (Xleast <= OtherXextent && Xextent >= OtherXleast && 
			Yleast <= OtherYextent && Yextent >= OtherYleast && 
			Zleast <= OtherZextent && Zextent >= OtherZleast);

	}
	bool collideWithMult(Object * other[], int numObjects)	{
		for (int a=0; a<numObjects; a++)	{
			if (other[a]->type != Blank)	{
				std::cout << "Other[" << a << "]->type != Blank. Happy face!" << "\n";
				if (collideWith(*other[a]))	{
					return true;
				}
			}
			else std::cout << "Other[" << a << "]->type == Blank. Sad face!" << "\n";
		}
		return false;

	}

};

class Camera	{
public:

	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat pointX;
	GLfloat pointY;
	GLfloat pointZ;
	//Vector3D cameraVector;

	Camera(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _pointX, GLfloat _pointY, GLfloat _pointZ)	{
		x = _x;
		y = _y;
		z = _z;
		pointX = _pointX;
		pointY = _pointY;
		pointZ = _pointZ;
	}
	Camera()	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		pointX = 0.0;
		pointY = 0.0;
		pointZ = 0.0;
	}

	void adjust()	{
		gluLookAt(x, y, z, pointX, pointY, pointZ, 0, 1, 0);
	}
	void lookAt(Object object, GLfloat Xdistance=0.0, GLfloat Ydistance=1.25, GLfloat Zdistance=1.25)	{
		x = object.x + Xdistance;
		y = object.y + Ydistance;
		z = object.z + Zdistance;
		pointX = object.x;
		pointY = object.y;
		pointZ = object.z;
		//adjust();
	}

} camera;

class Stage	{
private:
	Object * instances[20]; // = MAX_OBJECTS. Modify when changing that.
	int a;
	int b;
	static GLfloat * colornull;
	Camera * camera;
	friend void update(int);
	GLuint textureid;
public:
	static const int MAX_OBJECTS = 20;
	int numObjects;
	Stage()	{
		numObjects = 0;
		for (a=0; a < MAX_OBJECTS; a++)	{
			Object * object = new Object();
			instances[a] = object;
		}
		camera = &::camera;
	}
	void genTexture(char * fileName, int width, int height, bool alpha)	{
		int w = width;
		int h = height;
		if (alpha)	int a = 4;
		else	int a = 3;
		glGenTextures(1, &textureid);
		glBindTexture(GL_TEXTURE_2D, textureid);
		unsigned char * data = stbi_load(fileName, &w, &h, &a, 0);
		if (alpha)	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	void getAll(Object * outputTo[], int numInList=MAX_OBJECTS, int exceptID = -1)	{ // Modify numInList default when changing MAX_OBJECTS
		for (int a=0; a<numInList; a++)	{
			if (a != exceptID)	outputTo[a] = instances[a];
			else	outputTo[a] = new Object();
		}
	}
	int addInstance(Object::object_type type, GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat * color=(GLfloat *)&colornull)	{
		// Two-coord shape.
		if (numObjects < MAX_OBJECTS)	{
			int ID = numObjects;
			Object object;
			object.ID = ID;
			object.type = type;
			object.x = x;
			object.y = y;
			object.z = z;
			object.width = width;
			object.polygons = height;
			for (a=0; a<3; a++)	{
				object.color[a] = color[a];
			}
			instances[ID] = &object;
			numObjects += 1;
			return ID;
		}
		else	{
			for (a=0; a < MAX_OBJECTS; a++)	{
				if (instances[a]->type == Object::Blank)	{
					int ID = a;
					Object object;
					object.ID = ID;
					object.type = type;
					object.x = x;
					object.y = y;
					object.z = z;
					object.width = width;
					object.height = height;
					for (a=0; a<3; a++)	{
						object.color[a] = color[a];
					}
					instances[a] = &object;
					return a;
				}
			}
			return -1; // Not enough slots: simply fails to add object.
		}
	}
	int addInstance(Object::object_type type, GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int stacks, int slices, GLfloat * color=(GLfloat *)&colornull)	{
		// Sphere.
		if (numObjects < MAX_OBJECTS)	{
			int ID = numObjects;
			Object object;
			object.ID = ID;
			object.type = type;
			object.x = x;
			object.y = y;
			object.z = z;
			object.radius = radius;
			object.polygons = (stacks + slices) / 2;
			for (a=0; a<3; a++)	{
				object.color[a] = color[a];
			}
			instances[ID] = &object;
			numObjects += 1;
			return ID;
		}
		else	{
			for (a=0; a<MAX_OBJECTS; a++)	{
				if (instances[a]->type == Object::Blank)	{
					int ID = a;
					Object object;
					object.ID = ID;
					object.type = type;
					object.x = x;
					object.y = y;
					object.z = z;
					object.radius = radius;
					object.polygons = (stacks + slices) / 2;
					for (a=0; a<3; a++)	{
						object.color[a] = color[a];
					}
					instances[a] = &object;
					return a;
				}
			}
			return -1; // Not enough slots: simply fails to add object.
		}
	}
	int addInstance(Object &object)	{
		if (numObjects < MAX_OBJECTS)	{
			int ID = numObjects;
			object.ID = ID;
			instances[ID] = &object;
			numObjects += 1;
			return ID;
		}
		else	{
			for (a=0; a<MAX_OBJECTS; a++)	{
				if (instances[a]->type == Object::Blank)	{
					int ID = a;
					object.ID = ID;
					instances[ID] = &object;
					return ID;
				}
			}
			return -1;
		}
	}
	Object getObjectById(int id)	{
		return *(instances[a]);
	}
	void removeInstance(int id)	{
		instances[id]->type = Object::Blank;
	}
	void removeInstance(Object object)	{
		instances[object.ID]->type = Object::Blank;
	}
	void setCamera(Camera &newCamera)	{
		camera = &newCamera;
	}
	void draw()	{

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaWhite);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaWhite);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glLoadIdentity();

		camera->adjust();

		GLfloat qaLightPosition[] = {0.0, 1.0, 10.0, 1.0};
		glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (a=0; a<numObjects; a++)	{

			Object * object = instances[a];
			int type = object->type;
			GLfloat x = object->x;
			GLfloat y = object->y;
			GLfloat z = object->z;
			GLfloat * color = new GLfloat[4];
			for (b=0; b<4; b++)	{ color[b] = object->color[b]; }
			color[3] = 1.0;
			
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

			glPushMatrix();

			if (instances[a]->type == Object::Sphere)	{
				GLfloat radius = object->radius;
				if (1)	{
					int polygons = object->polygons;
					float degreesRotate = object->degreesRotate;
					glTranslatef(x, y, z);
					glRotatef(degreesRotate, 1.0, 0.0, 0.0);
					glutSolidSphere(radius, polygons, polygons);
				}
			}
			if (instances[a]->type == Object::Rectangle)	{
				GLfloat width = object->width;
				GLfloat height = object->height;
				GLfloat length = object->length;

				glBindTexture(GL_TEXTURE_2D, textureid);
				glBegin(GL_QUADS);

					glTexCoord2f(500, 500);

					/* Front */
					glVertex3f(x, y, z);
					glVertex3f(x+width, y, z);
					glVertex3f(x+width, y+height, z);
					glVertex3f(x, y+height, z);

					/* Back */
					glVertex3f(x, y, z+length);
					glVertex3f(x+width, y, z+length);
					glVertex3f(x+width, y+height, z+length);
					glVertex3f(x, y+height, z+length);

					/* Left */
					glVertex3f(x, y, z);
					glVertex3f(x, y, z+length);
					glVertex3f(x, y+height, z+length);
					glVertex3f(x, y+height, z);

					/* Right */
					glVertex3f(x+width, y, z);
					glVertex3f(x+width, y, z+length);
					glVertex3f(x+width, y+height, z+length);
					glVertex3f(x+width, y+height, z);

					/* Top */
					glVertex3f(x, y+height, z);
					glVertex3f(x+width, y+height, z);
					glVertex3f(x+width, y+height, z+length);
					glVertex3f(x, y+height, z+length);

					/* Bottom */
					glVertex3f(x, y, z);
					glVertex3f(x+width, y, z);
					glVertex3f(x+width, y, z+length);
					glVertex3f(x, y, z+length);
				glEnd();

			}

			glPopMatrix();

		}

		glutSwapBuffers();
	}

} stage;

void drawStage()	{
	stage.draw();
}

void setKeyboardUpdateFunc(void (*func)())	{
	::keyboardUpdateFunc = func;
}

void update(int totalFrames)	{
	::keyboardUpdateFunc();
	for (int b=0; b<stage.numObjects; b++)	{
		stage.instances[b]->update();
	}
	glutPostRedisplay();
	glutTimerFunc(10, update, totalFrames+1);
}

class Vector	{
public:
	double speed;
	double angle;
	double x;
	double z;
	enum types	{	plane, vector	};
	Vector(double _speed, int _angle)	{
		speed = _speed;
		angle = _angle;
		update(plane);
	}
	Vector(double _x, double _z)	{
		x = _x;
		z = _z;
		update(vector);
	}
	Vector()	{
		speed = 0;
		angle = 0;
		update(plane);
	}
	void update(types type)	{
		if (angle > 360)	angle = (int)angle % 360;
		if (type == vector)	{
			if (speed != 0)	{
				speed = sqrt(x*x + z*z);
				angle = acos(x / speed / 57);
			}
		}
		if (type == plane)	{
			z = -1 * cos((double)angle / 57) * speed;
			x = sin((double)angle / 57) * speed;
		}
	}
};

/*class Vector3D	{
public:
	double speed;
	double angleXY;
	double angleXZ;
	double x;
	double y;
	double z;
	enum types	{	plane, vector	};
	Vector3D(double _speed, int _angleXY, int _angleXZ)	{
		speed = _speed;
		angleXY = _angleXY;
		angleXZ = _angleXZ;
		update(plane);
	}
	Vector3D(double _x, double _y, double _z)	{
		x = _x;
		y = _y;
		z = _z;
		update(vector);
	}
	Vector3D()	{
		speed = 0;
		angleXY = 0;
		angleXZ = 0;
		update(plane);
	}
	void update(types type)	{
		if (angleXY > 360)	angleXY = (int)angleXY % 360;
		if (angleXZ > 360)	angleXZ = (int)angleXZ % 360;
		if (type == vector)	{
			if (speed != 0)	{
				double speedXZ = sqrt(x*x + z*z);
				speed = sqrt(speedXZ*speedXZ + y*y);
				angleXZ = acos(x / speed / 57);
				angleXY = acos(
			}
		}
		if (type == plane)	{
			z = -1 * cos((double)angle / 57) * speed;
			x = sin((double)angle / 57) * speed;
		}
	}
}*/