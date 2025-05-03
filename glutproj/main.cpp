#include <GL/glut.h>
#include <math.h>
#define DEG2RAD 3.14159/180.0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GLUquadric* quad;

float ballZ = 10.0f;
float ballSpeed = -0.1f;
float ballDirectionX = 0.0f;  // Ball's X position (aim)
bool ballKicked = false;

float goalkeeperX = 0.0f;
bool goalkeeperMovingLeft = true;

// Draws an ellipse for body parts
void DrawEllipse(float radiusX, float radiusY, int posx, int posy) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float rad = i * DEG2RAD;
        glVertex2f(posx + cos(rad) * radiusX, posy + sin(rad) * radiusY);
    }
    glEnd();
}

// Draws a simple human body
void humanBody() {
    // Skin color
    glColor3f(1.0f, 0.87f, 0.77f);

    // Legs
    glPushMatrix();
    glRotatef(-10, 0, 0, 1);
    DrawEllipse(10, 50, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glRotatef(10, 0, 0, 1);
    DrawEllipse(10, 50, 25, -7);
    glPopMatrix();

    // Torso
    glColor3f(0.0f, 0.0f, 1.0f);
    DrawEllipse(20, 50, 12, 70);

    // Arms
    glPushMatrix();
    glRotatef(60, 0, 0, 1);
    DrawEllipse(40, 10, 60, 55);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-60, 0, 0, 1);
    DrawEllipse(40, 10, -50, 75);
    glPopMatrix();

    // Head
    glColor3f(1.0f, 0.87f, 0.77f);
    DrawEllipse(15, 25, 12, 135);
}

// Striker follows the aim (ballDirectionX)
void drawStriker() {
    glPushMatrix();
    glTranslatef(ballDirectionX, 0.05f, 11.5f);
    glScalef(0.02f, 0.02f, 0.02f);
    humanBody();
    glPopMatrix();
}

// Goalkeeper at goalkeeperX
void drawGoalkeeper() {
    glPushMatrix();
    glTranslatef(goalkeeperX, 0.05f, 1.5f);
    glScalef(0.02f, 0.02f, 0.02f);
    humanBody();
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
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);

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
    float halfWidth = 3.66f;
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCylinder(-halfWidth, 0.0f, 0.0f, 4.0f, true);
    drawCylinder(halfWidth, 0.0f, 0.0f, 4.0f, true);
    drawCylinder(-halfWidth, 4.0f, 0.0f, 7.32f, false);
}

void drawGround() {
    glColor3f(0.3f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -10);
    glVertex3f(20, 0, -10);
    glVertex3f(20, 0, 20);
    glVertex3f(-20, 0, 20);
    glEnd();
}

void drawPenaltyBox() {
    float h = 0.02f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-6.0f, h, 0.0f); glVertex3f(6.0f, h, 0.0f);
    glVertex3f(-6.0f, h, 6.0f); glVertex3f(6.0f, h, 6.0f);
    glVertex3f(-6.0f, h, 0.0f); glVertex3f(-6.0f, h, 6.0f);
    glVertex3f(6.0f, h, 0.0f);  glVertex3f(6.0f, h, 6.0f);
    glEnd();
}

// Ball always at (ballDirectionX, ballZ)
void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(ballDirectionX, 0.3f, ballZ);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();
}

void update(int value) {
    // Ball movement
    if (ballKicked) {
        ballZ += ballSpeed;
        if (ballZ <= 0.5f) {
            ballZ = 0.5f;
            ballKicked = false;

            // Check if goalkeeper saves
            if (fabs(ballDirectionX - goalkeeperX) > 0.5f) {
                printf("GOAL!\n");
            } else {
                printf("SAVED!\n");
            }

            // Reset after a short delay
            ballZ = 10.0f;
            ballDirectionX = 0.0f;
        }
    }

    // Goalkeeper movement
    if (!ballKicked || ballZ > 0.5f) {
        if (goalkeeperMovingLeft)
            goalkeeperX -= 0.08f;
        else
            goalkeeperX += 0.08f;

        if (goalkeeperX < -1.5f)
            goalkeeperMovingLeft = false;
        else if (goalkeeperX > 1.5f)
            goalkeeperMovingLeft = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 4.0, 15.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

    drawGround();
    drawPenaltyBox();
    drawGoal();
    drawGoalkeeper();
    drawStriker();
    drawBall();

    glutSwapBuffers();
}

// Arrow keys: left/right to aim, up to shoot
void specialKeys(int key, int x, int y) {
    if (!ballKicked) {
        switch (key) {
        case GLUT_KEY_LEFT:
            ballDirectionX -= 0.2f;
            if (ballDirectionX < -3.0f) ballDirectionX = -3.0f;
            break;
        case GLUT_KEY_RIGHT:
            ballDirectionX += 0.2f;
            if (ballDirectionX > 3.0f) ballDirectionX = 3.0f;
            break;
        case GLUT_KEY_UP:
            ballKicked = true;
            break;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Striker vs Goalkeeper - OpenGL Animation");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
