#include <GL/glut.h>
#include <cmath>

GLUquadric* quad;

float ballZ = 10.0f;
float ballSpeed = -0.05f;
bool goingForward = true;

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glClearColor(0.2f, 0.7f, 0.3f, 1.0f); // Green field background

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
    glColor3f(0.1f, 0.5f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -10);
    glVertex3f(20, 0, -10);
    glVertex3f(20, 0, 20);
    glVertex3f(-20, 0, 20);
    glEnd();
}
//
//void drawNet() {
//    glColor3f(1.0f, 1.0f, 1.0f);
//    glBegin(GL_LINES);
//
//    float width = 7.32f;
//    float halfWidth = width / 2.0f;
//    float height = 4.0f;
//    float depth = 2.5f;
//    float spacing = 0.15f;
//
//    // Vertical strings (X-Z)
//    for (float x = -halfWidth; x <= halfWidth; x += spacing) {
//        for (float y = 0.0f; y <= height; y += spacing) {
//            float zFront = 0.0f;
//            float curve = (1.0f - powf(x / halfWidth, 2)) * depth; // Parabolic curve
//            glVertex3f(x, y, zFront);
//            glVertex3f(x, y, curve);
//        }
//    }
//
//    // Horizontal strings (X across Z)
//    for (float z = 0.0f; z <= depth; z += spacing) {
//        for (float y = 0.0f; y <= height; y += spacing) {
//            float xStart = -halfWidth * (1.0f - z / depth);
//            float xEnd = halfWidth * (1.0f - z / depth);
//            glVertex3f(xStart, y, z);
//            glVertex3f(xEnd, y, z);
//        }
//    }
//
//    // Depth strings (Y-Z)
//    for (float x = -halfWidth; x <= halfWidth; x += spacing) {
//        for (float z = 0.0f; z <= depth; z += spacing) {
//            float curve = (1.0f - powf(x / halfWidth, 2)) * z;
//            glVertex3f(x, 0.0f, curve);
//            glVertex3f(x, height, curve);
//        }
//    }
//
//    glEnd();
//}

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

    // Front view camera
    gluLookAt(0.0, 3.0, 10.0,
              0.0, 2.0, 0.0,
              0.0, 1.0, 0.0);

    drawGround();
    drawPenaltyBox();
    drawGoal();

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
