#include <GL/glut.h>
#include "math.h"
#define DEG2RAD 3.14159/180.0
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

GLUquadric* quad;

float ballZ = 10.0f;
float ballSpeed = -0.05f;
bool goingForward = true;

void DrawEllipse(float radiusX, float radiusY, int posx, int posy) {
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 360; i++) {
        float rad = i * DEG2RAD;
        glVertex2f(posx + cos(rad) * radiusX,
                   posy + sin(rad) * radiusY);
    }
    glEnd();
}void humanBody() {
    // Set skin color once for all body parts except vessels
    glColor3f(1.0f, 0.87f, 0.77f); // Skin color

    // --- Legs ---
    glPushMatrix();
    glRotatef(-10, 0, 0, 1);
    DrawEllipse(10, 50, 0, 0); // Left leg
    glPopMatrix();

    glPushMatrix();
    glRotatef(10, 0, 0, 1);
    DrawEllipse(10, 50, 25, -7); // Right leg
    glPopMatrix();

    // --- Torso ---
    DrawEllipse(20, 50, 12, 70); // Center torso

    // --- Arms ---
    glPushMatrix();
    glRotatef(60, 0, 0, 1);
    DrawEllipse(40, 10, 60, 55); // Left arm
    glPopMatrix();

    glPushMatrix();
    glRotatef(-60, 0, 0, 1);
    DrawEllipse(40, 10, -50, 75); // Right arm
    glPopMatrix();

    // --- Head ---
    DrawEllipse(15, 25, 12, 135); // Head

    // --- Blood Vessels ---
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Red for vessels
    glBegin(GL_LINES);
    glVertex2f(12, 30); glVertex2f(12, 120);        // Center vertical
    glVertex2f(12, 118); glVertex2f(-8, 90);        // Left upper
    glVertex2f(-8, 90); glVertex2f(-30, 60);
    glVertex2f(12, 118); glVertex2f(50, 65);        // Right upper
    glVertex2f(12, 35); glVertex2f(25, 10);         // Right lower
    glVertex2f(25, 10); glVertex2f(30, -20);
    glVertex2f(12, 35); glVertex2f(2, 10);          // Left lower
    glVertex2f(2, 10); glVertex2f(-5, -30);
    glEnd();
    glPopMatrix();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    quad = gluNewQuadric();
}

void drawCylinder(float x, float y, float z, float height, bool vertical) {
    glPushMatrix();
    glTranslatef(x, y, z);
    if (vertical)
        glRotatef(-90, 1, 0, 0);
    else
        glRotatef(90, 0, 1, 0);
    gluCylinder(quad, 0.05, 0.05, height, 20, 5);
    glPopMatrix();
}

void drawGoal() {
    float halfWidth = 3.66f; // Half of 7.32m
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCylinder(-halfWidth, 0.0f, 0.0f, 4.0f, true);  // Left post
    drawCylinder(halfWidth, 0.0f, 0.0f, 4.0f, true);   // Right post
    drawCylinder(-halfWidth, 4.0f, 0.0f, 7.32f, false); // Crossbar
}

void drawGround() {
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -10);
    glVertex3f(20, 0, -10);
    glVertex3f(20, 0, 20);
    glVertex3f(-20, 0, 20);
    glEnd();
}

void drawPenaltyBox() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-6.0f, 0.01f, 0.0f);
    glVertex3f(6.0f, 0.01f, 0.0f);
    glVertex3f(-6.0f, 0.01f, 6.0f);
    glVertex3f(6.0f, 0.01f, 6.0f);
    glVertex3f(-6.0f, 0.01f, 0.0f);
    glVertex3f(-6.0f, 0.01f, 6.0f);
    glVertex3f(6.0f, 0.01f, 0.0f);
    glVertex3f(6.0f, 0.01f, 6.0f);
    glEnd();
}

void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, ballZ);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();
}

void update(int value) {
    if (goingForward) {
        ballZ += ballSpeed;
        if (ballZ <= 0.5f)
            goingForward = false;
    } else {
        ballZ -= ballSpeed * 0.5f;
        if (ballZ >= 10.0f)
            goingForward = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 3.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

    drawGround();
    drawPenaltyBox();
    drawGoal();

    // Draw human body as goalkeeper
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.3f);  // X=center, Y=on ground, Z=slightly inside goal
    glScalef(0.015f, 0.015f, 0.015f); // Scale to fit inside goal
    humanBody();
    glPopMatrix();

    drawBall();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Realistic 3D Soccer Goal - OpenGL");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
