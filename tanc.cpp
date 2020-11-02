/*
	George Baba
	Calculatoare, 223-2
	Proiect: Tanc
*/

#include <math.h>
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
void CALLBACK scene(void);
void CALLBACK tank(void);

// Lightning
GLfloat position[] = { 0.0, 2.0, -0.25, 1.0 };
boolean umbra = 0;

// Quadric
GLUquadricObj* quadObj;

// Load Texture
GLuint IDtexture, metalOneTexture, metalTwoTexture, rubberTexture, rustTexture, grassTexture, skyTexture;
void loadTexture(const char* s) {
	AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);
	if(pImagineTextura != NULL) {
		glGenTextures(1, &IDtexture);
		glBindTexture(GL_TEXTURE_2D, IDtexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}
	if(pImagineTextura) {
		if(pImagineTextura->data)
			free(pImagineTextura->data);
		free(pImagineTextura);
	}
}

// Animation
boolean animationStarted = false;
static GLfloat bulletX = -0.85;
void CALLBACK bulletAnimation(void) {
	if(animationStarted) {
		bulletX -= 0.05;
	}
	display();
	Sleep(25);
}
void CALLBACK toggleAnimation() {
	bulletX = -0.85;
	if(animationStarted) {
		animationStarted = false;
	} else {
		animationStarted = true;
	}
}

// Rotation
static GLfloat alfaX = 0;
void CALLBACK addAlfaX() {
	alfaX = alfaX + 10;
}
void CALLBACK subAlfaX() {
	alfaX = alfaX - 10;
}
static GLfloat alfaY = 0;
void CALLBACK addAlfaY() {
	alfaY = alfaY + 10;
}
void CALLBACK subAlfaY() {
	alfaY = alfaY - 10;
}
static GLfloat betaY = 0;
void CALLBACK addBetaY() {
	betaY = betaY + 2;
}
void CALLBACK subBetaY() {
	betaY = betaY - 2;
}
static GLfloat alfaSun = 0;
void CALLBACK rotSunRight() {
	alfaSun = alfaSun - 10;
	position[0] -= 0.5;
}
void CALLBACK rotSunLeft() {
	alfaSun = alfaSun + 10;
	position[0] += 0.5;
}

// Movement
static GLfloat coordX = 0;
void CALLBACK moveRightX() {
	coordX = coordX + 0.1;
}
void CALLBACK moveLeftX() {
	coordX = coordX - 0.1;
}

// Init
void myinit(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Texture
	loadTexture(".\\textures/metal-1.bmp");
	metalOneTexture = IDtexture;

	loadTexture(".\\textures/metal-2.bmp");
	metalTwoTexture = IDtexture;

	loadTexture(".\\textures/rubber.bmp");
	rubberTexture = IDtexture;

	loadTexture(".\\textures/rust.bmp");
	rustTexture = IDtexture;

	loadTexture(".\\textures/grass.bmp");
	grassTexture = IDtexture;

	loadTexture(".\\textures/sky.bmp");
	skyTexture = IDtexture;

	glEnable(GL_TEXTURE_2D);

	// Lightning
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 22.0f };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	//permite urmarirea culorilor
	//glEnable(GL_COLOR_MATERIAL);
	//seteaza proprietatile de material pt a urma valorile glColor
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//seteaza sursa
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//seteaza materialul
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_AUTO_NORMAL);

	// Depth Test
	glEnable(GL_DEPTH_TEST);
}

// Tank Body
void tankBody(void) {
	if(umbra == 0) {
		glColor3f(0.60f, 0.58f, 0.42f);
		glBindTexture(GL_TEXTURE_2D, metalOneTexture);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v1

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f); // v2

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.0f, 0.2f, 0.0f); // v3

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.1f, 0.0f, -0.1f); // v4

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.1f, 0.2f, -0.1f); // v5

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.1f, 0.0f, -0.5f); // v6

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.1f, 0.2f, -0.5f); // v7

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.0f, 0.0f, -0.6f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.2f, -0.6f); // v9

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, -0.33f);
		glVertex3f(0.0f, 0.0f, -0.6f); // v10

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.2f, -0.6f); // v11

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(-0.33f, 0.0f, -0.33f);
		glVertex3f(-0.1f, 0.0f, -0.5f); // v12

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.1f, 0.2f, -0.5f); // v13

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(-0.33f, 0.0f, -0.33f);
		glVertex3f(-0.1f, 0.0f, -0.1f); // v14

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.1f, 0.2f, -0.1f); // v15

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v16

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v17
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(-0.05f, 0.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v1

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.0f, 0.2f, 0.0f); // v3

		glTexCoord2f(0.05f, 0.15f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.1f, 0.2f, -0.1f); // v5

		glTexCoord2f(1.0f, -0.55f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.1f, 0.2f, -0.5f); // v7

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.2f, -0.6f); // v9

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.2f, -0.6f); // v11

		glTexCoord2f(-0.05f, -0.55f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.1f, 0.2f, -0.5f); // v13

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.1f, 0.2f, -0.1f); // v15

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v17
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f); // v2

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.1f, 0.0f, -0.1f); // v4

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.1f, 0.0f, -0.5f); // v6

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, -0.33f);
		glVertex3f(1.0f, 0.0f, -0.6f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, -0.33f);
		glVertex3f(0.0f, 0.0f, -0.6f); // v10

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(-0.33f, 0.0f, -0.33f);
		glVertex3f(-0.1f, 0.0f, -0.5f); // v12

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(-0.33f, 0.0f, -0.33f);
		glVertex3f(-0.1f, 0.0f, -0.1f); // v14

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v16
	glEnd();
}

// Tank Upper Body
void tankUpperBody(void) {
	if(umbra == 0) {
		glColor3f(0.60f, 0.58f, 0.42f);
		glBindTexture(GL_TEXTURE_2D, metalOneTexture);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.05f, 0.2f, -0.05f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.35f, -0.1f); // v1

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.05f, 0.2f, -0.05f); // v2

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.35f, -0.1f); // v3

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.05f, 0.2f, -0.55f); // v4

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.35f, -0.5f); // v5

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.05f, 0.2f, -0.55f); // v6

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.35f, -0.5f); // v7

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(-0.33f, 0.33f, -0.33f);
		glVertex3f(-0.05f, 0.2f, -0.05f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.35f, -0.1f); // v9
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.30f, -0.1f); // v1

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.30f, -0.1f); // v3

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, -0.33f);
		glVertex3f(1.0f, 0.30f, -0.5f); // v5

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, -0.33f);
		glVertex3f(0.0f, 0.30f, -0.5f); // v7
	glEnd();

	// Tank Reservoir
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rustTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(0.9f, 0.37f, -0.45f);
		gluCylinder(quadObj, 0.033, 0.033, 0.3, 12, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rustTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(0.9f, 0.37f, -0.45f);
		gluDisk(quadObj, 0, 0.033, 12, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rustTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(0.9f, 0.37f, -0.15f);
		gluDisk(quadObj, 0, 0.033, 12, 10);
	glPopMatrix();

}

// Tank Turret
void tankTurret(void) {
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.4f, 0.3f, 0.35f);
		gluCylinder(quadObj, 0.18, 0.13, 0.12, 12, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.4f, 0.3f, 0.47f);
		gluDisk(quadObj, 0.0, 0.13, 12, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.25f, 0.41f, -0.35f);
		gluCylinder(quadObj, 0.04, 0.04, 0.10, 15, 10);
	glPopMatrix();
	
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(0.25f, 0.41f, -0.35f);
		gluDisk(quadObj, 0.0, 0.04, 15, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(0.25f, 0.41f, -0.25f);
		gluDisk(quadObj, 0.0, 0.04, 15, 10);
	glPopMatrix();
}

// Tank Turret Pipe
void tankTurretPipe(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glRotatef(-2.5, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.3f, 0.42f, -0.3f);
		gluCylinder(quadObj, 0.013, 0.013, 1.1, 15, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glRotatef(-2.5, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.3f, 0.42f, 0.8f);
		gluCylinder(quadObj, 0.013, 0.026, 0.05, 15, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glRotatef(-2.5, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.3f, 0.42f, 0.85f);
		gluCylinder(quadObj, 0.026, 0.026, 0.05, 15, 10);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, metalOneTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glRotatef(-2.5, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.3f, 0.42f, 0.9f);
		gluCylinder(quadObj, 0.026, 0.013, 0.05, 15, 10);
	glPopMatrix();

	// Bullet
	glPushMatrix();
		if(umbra == 0) {
			glColor3f(1.0, 0.0, 0.0);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glTranslatef(bulletX, 0.46, -0.3);
		auxSolidSphere(0.017);
	glPopMatrix();
}

// Tank Rails Left
void tankRailsLeft(void) {
	if(umbra == 0) {
		glColor3f(0.443f, 0.431f, 0.313f);
		glBindTexture(GL_TEXTURE_2D, metalTwoTexture);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.25f); // v1

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, 0.0f);
		glVertex3f(-0.2f, 0.15f, 0.0f); // v2

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(-0.33f, 0.33f, 0.33f);
		glVertex3f(-0.2f, 0.15f, 0.25f); // v3

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v4

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.33f);
		glVertex3f(0.0f, 0.2f, 0.25f); // v5

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.1f, 0.2f, 0.0f); // v6

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.1f, 0.2f, 0.25f); // v7

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.2f, 0.15f, 0.0f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.2f, 0.15f, 0.25f); // v9

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, 0.0f);
		glVertex3f(1.1f, 0.0f, 0.0f); // v10

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, 0.33f);
		glVertex3f(1.1f, 0.0f, 0.25f); // v11

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v12

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.25f); // v13
	glEnd();

	
	if(umbra == 0) {
		glColor3f(0.360f, 0.349f, 0.258f);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.125f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, 0.33f);
		glVertex3f(-0.2f, 0.15f, 0.125f); // v2

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.33f);
		glVertex3f(0.0f, 0.2f, 0.125f); // v4
									  
		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.1f, 0.2f, 0.125f); // v6

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.2f, 0.15f, 0.125f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, 0.33f);
		glVertex3f(1.1f, 0.0f, 0.125f); // v10

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.125f); // v12
	glEnd();

	// Tires
	if(umbra == 0) {
		glColor3f(0.203f, 0.2f, 0.152f);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.12f, 0.135f, 0.1f);
		gluCylinder(quadObj, 0.03, 0.03, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.12f, 0.135f, 0.24f);
		auxSolidTorus(0.007, 0.02);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.03f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.03f, 0.1f, 0.24f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.03f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.03f, 0.1f, 0.24f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.24f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.24f, 0.1f, 0.24f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.24f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.24f, 0.1f, 0.24f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(1.1f, 0.13f, 0.1f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.1f, 0.13f, 0.24f);
		auxSolidTorus(0.01, 0.05);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(1.1f, 0.13f, 0.1f);
		gluCylinder(quadObj, 0.025, 0.025, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.1f, 0.13f, 0.24f);
		auxSolidTorus(0.015, 0.015);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.93f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.93f, 0.1f, 0.24f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.93f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.93f, 0.1f, 0.24f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.72f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.72f, 0.1f, 0.24f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.72f, 0.1f, 0.1f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.72f, 0.1f, 0.24f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();
}

// Tank Rails Right
void tankRailsRight(void) {
	if(umbra == 0) {
		glColor3f(0.443f, 0.431f, 0.313f);
		glBindTexture(GL_TEXTURE_2D, metalTwoTexture);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.25f); // v1

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, 0.0f);
		glVertex3f(-0.2f, 0.15f, 0.0f); // v2

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(-0.33f, 0.33f, 0.33f);
		glVertex3f(-0.2f, 0.15f, 0.25f); // v3

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.33f, 0.0f);
		glVertex3f(0.0f, 0.2f, 0.0f); // v4

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.33f);
		glVertex3f(0.0f, 0.2f, 0.25f); // v5

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.1f, 0.2f, 0.0f); // v6

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.1f, 0.2f, 0.25f); // v7

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.0f);
		glVertex3f(1.2f, 0.15f, 0.0f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.2f, 0.15f, 0.25f); // v9

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, 0.0f);
		glVertex3f(1.1f, 0.0f, 0.0f); // v10

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.0f, 0.33f);
		glVertex3f(1.1f, 0.0f, 0.25f); // v11

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f); // v12

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.25f); // v13
	glEnd();

	if(umbra == 0) {
		glColor3f(0.360f, 0.349f, 0.258f);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.125f); // v0

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(-0.33f, 0.33f, 0.33f);
		glVertex3f(-0.2f, 0.15f, 0.125f); // v2

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.33f, 0.33f);
		glVertex3f(0.0f, 0.2f, 0.125f); // v4

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.1f, 0.2f, 0.125f); // v6

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.33f, 0.33f, 0.33f);
		glVertex3f(1.2f, 0.15f, 0.125f); // v8

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.33f, 0.0f, 0.33f);
		glVertex3f(1.1f, 0.0f, 0.125f); // v10

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 0.33f);
		glVertex3f(0.0f, 0.0f, 0.125f); // v12
	glEnd();

	// Tires
	if(umbra == 0) {
		glColor3f(0.203f, 0.2f, 0.152f);
	} else {
		glColor3f(0.1, 0.1, 0.1);
	}
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(-0.12f, 0.135f, 0.0f);
		gluCylinder(quadObj, 0.03, 0.03, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.12f, 0.135f, 0.01f);
		auxSolidTorus(0.007, 0.02);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.03f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.03f, 0.1f, 0.01f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.03f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.03f, 0.1f, 0.01f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.24f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.24f, 0.1f, 0.01f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.24f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.24f, 0.1f, 0.01f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(1.1f, 0.13f, 0.0f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.1f, 0.13f, 0.01f);
		auxSolidTorus(0.01, 0.05);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(1.1f, 0.13f, 0.0f);
		gluCylinder(quadObj, 0.025, 0.025, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.1f, 0.13f, 0.01f);
		auxSolidTorus(0.015, 0.015);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.93f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.93f, 0.1f, 0.01f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.93f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.93f, 0.1f, 0.01f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();

	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.72f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.095, 0.095, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.72f, 0.1f, 0.01f);
		auxSolidTorus(0.01, 0.09);
	glPopMatrix();
	glPushMatrix();
		quadObj = gluNewQuadric();
		if(umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, rubberTexture);
			gluQuadricTexture(quadObj, true);
		} else {
			glColor3f(0.1, 0.1, 0.1);
		}
		glRotatef(0.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.72f, 0.1f, 0.0f);
		gluCylinder(quadObj, 0.06, 0.06, 0.15, 15, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.72f, 0.1f, 0.01f);
		auxSolidTorus(0.03, 0.03);
	glPopMatrix();
}

// Tank Rails
void tankRails(void) {
	glPushMatrix();
		glTranslatef(-0.025f, -0.07f, -0.124f);
		tankRailsLeft();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.025f, -0.07f, -0.726f);
		tankRailsRight();
	glPopMatrix();
}

// Tank Parts
void CALLBACK tank(void) {
	glPushMatrix();
		tankBody();
		tankUpperBody();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.4f, 0.0f, -0.3f);
		glRotatef(betaY, 0.0, 1.0, 0.0);
		glTranslatef(-0.4f, 0.0f, 0.3f);
		tankTurret();
		tankTurretPipe();
	glPopMatrix();
	
	glPushMatrix();
		tankRails();
	glPopMatrix();
}

// Shadow
void calcCoeficientiPlan(float P[3][3], float coef[4]) {
	float v1[3], v2[3];
	float length;
	static const int x = 0, y = 1, z = 2;

	// calculeaza 2 vectori din 3 pct
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];
	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se calc produsul vectorial al celor 2 vectori => al3lea vector cu componentele A,B,C chiar coef din ec. planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] - v2[x];

	//normalizare vector
	length = (float)sqrt(coef[x] * coef[x] + coef[y] * coef[y] + coef[z] * coef[z]);
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}
void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]) {
	// creeaza matricea care da umbra cunoscandu-se coef planului + poz sursei
	// IN mat SE SALVEAZA MATRICEA

	GLfloat coefPlan[4], temp;

	//determina coef planului
	calcCoeficientiPlan(puncte, coefPlan);

	// determinam D
	coefPlan[3] = -(coefPlan[0] * puncte[2][0] + coefPlan[1] * puncte[2][1] + coefPlan[2] * puncte[2][2]);

	// temp= AxL + ByL + CzL + Dw
	temp = coefPlan[0] * pozSursa[0] + coefPlan[1] * pozSursa[1] + coefPlan[2] * pozSursa[2] + coefPlan[3] * pozSursa[3];

	//prima coloana
	mat[0][0] = temp - coefPlan[0] * pozSursa[0];
	mat[1][0] = 0.0f - coefPlan[1] * pozSursa[0];
	mat[2][0] = 0.0f - coefPlan[2] * pozSursa[0];
	mat[3][0] = 0.0f - coefPlan[3] * pozSursa[0];
	//a 2a coloana
	mat[0][1] = 0.0f - coefPlan[0] * pozSursa[1];
	mat[1][1] = temp - coefPlan[1] * pozSursa[1];
	mat[2][1] = 0.0f - coefPlan[2] * pozSursa[1];
	mat[3][1] = 0.0f - coefPlan[3] * pozSursa[1];
	//a 3a coloana
	mat[0][2] = 0.0f - coefPlan[0] * pozSursa[2];
	mat[1][2] = 0.0f - coefPlan[1] * pozSursa[2];
	mat[2][2] = temp - coefPlan[2] * pozSursa[2];
	mat[3][2] = 0.0f - coefPlan[3] * pozSursa[2];
	//a4a coloana
	mat[0][3] = 0.0f - coefPlan[0] * pozSursa[3];
	mat[1][3] = 0.0f - coefPlan[1] * pozSursa[3];
	mat[2][3] = 0.0f - coefPlan[2] * pozSursa[3];
	mat[3][3] = temp - coefPlan[3] * pozSursa[3];
}

// Grass
void grass(void) {
	// Grass
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		//glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glNormal3f(-0.25, 0.0, 0.0);
			glVertex3f(-6.0f, -2.0f, 1.0f); // v0

			glTexCoord2f(0.0f, 1.0f);
			glNormal3f(-0.25, 0.0, -0.25);
			glVertex3f(-6.0f, 1.0f, -3.0f); // v1

			glTexCoord2f(1.0f, 1.0f);
			glNormal3f(0.25, 0.0, 0.25);
			glVertex3f(6.0f, 1.0f, -3.0f); // v2

			glTexCoord2f(1.0f, 0.0f);
			glNormal3f(0.25, 0.0, 0.0);
			glVertex3f(6.0f, -2.0f, 1.0f); // v2
		glEnd();
	glPopMatrix();
}

// Sky
void sky(void) {
	// Sky
	glPushMatrix();
		quadObj = gluNewQuadric();
		if (umbra == 0) {
			glBindTexture(GL_TEXTURE_2D, skyTexture);
			gluQuadricTexture(quadObj, true);
		}
		else {
			glColor3f(0.1, 0.1, 0.1);
		}

		glTranslatef(0.0f, 1.0f, -12.0f);
		glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
		gluSphere(quadObj, 9.0, 15, 10);
	glPopMatrix();
}

// Scene
void CALLBACK scene(void) {
	// Tank
	glPushMatrix();
		glRotatef(alfaX, 1.0, 0.0, 0.0);
		glRotatef(alfaY, 0.0, 1.0, 0.0);
		glTranslatef(coordX, 0.0, 0.0);

		glEnable(GL_COLOR_MATERIAL);
		tank();
		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	gluDeleteQuadric(quadObj);
}

// Display
void CALLBACK display(void) {
	glClearColor(0.721, 1, 0.996, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	// Shadow
	GLfloat matUmbra[4][4];
	GLfloat puncte[3][3] = {
		{ -2.0f, -1.0f, -1.0f },
		{ -2.0f, -1.0f, 1.0f },
		{ 2.0f, -1.0f, 1.0f }
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MatriceUmbra(puncte, position, matUmbra);

	glPushMatrix();
		//glDisable(GL_COLOR_MATERIAL);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_TEXTURE_2D);
		grass();
		umbra = 0;
		scene();
		sky();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//deseneaza umbra
	//mai intai se dezactiveaza iluminarea si se salveaza starea matricei de proiectie
	glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glPushMatrix();
				glMultMatrixf((GLfloat*)matUmbra);// se inmulteste matricea curenta cu matricea de umbrire
				glTranslatef(0, 0, -0.35);
				umbra = 1;
				scene();
			glPopMatrix();
			//deseneaza sursa de lumina ca o sfera mica galbena in pozitia reala
			glPushMatrix();
				glTranslatef(position[0], position[1], position[2]);
				glColor3f(1.0, 1.0, 0.0);
				auxSolidSphere(0.1);
			glPopMatrix();
		//reseteaza starea variabilelor de iluminare
		glPopAttrib();
	glPopMatrix();

	auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -4.0f);
}

/*  Main Loop */
int main(int argc, char** argv) {
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);
	auxInitPosition(0, 0, 1200, 600);
	auxInitWindow("Tanc");
	myinit();

	// Animation
	auxIdleFunc(bulletAnimation);
	auxKeyFunc(AUX_SPACE, toggleAnimation);

	// Movement
	auxKeyFunc(AUX_w, moveLeftX);
	auxKeyFunc(AUX_s, moveRightX);

	// Rotation
	auxKeyFunc(AUX_UP, addAlfaX);
	auxKeyFunc(AUX_DOWN, subAlfaX);
	auxKeyFunc(AUX_LEFT, addAlfaY);
	auxKeyFunc(AUX_RIGHT, subAlfaY);

	auxKeyFunc(AUX_1, addBetaY);
	auxKeyFunc(AUX_2, subBetaY);

	auxKeyFunc(AUX_3, rotSunRight);
	auxKeyFunc(AUX_4, rotSunLeft);


	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}