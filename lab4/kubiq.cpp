#include <GL/freeglut.h>
#include <cmath>

// Вращение источника света
float lightAngle = 0.0f;
float lightRadius = 5.0f;
GLfloat lightPos[] = {5.0f, 2.0f, 0.0f, 1.0f};

// Вращение куба
float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

// Раздвижение граней
bool exploded = false;
bool expanding = false;
float faceOffset = 0.0f;
float maxOffset = 1.5f;
float offsetSpeed = 0.05f;

// Прозрачность
float alpha = 1.0f;

// Отрисовка куба из 6 граней
void drawCube() {
    glColor4f(0.2f, 0.6f, 1.0f, alpha);

    // Front face (+Z)
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-1, -1, 1 + faceOffset);
    glVertex3f( 1, -1, 1 + faceOffset);
    glVertex3f( 1,  1, 1 + faceOffset);
    glVertex3f(-1,  1, 1 + faceOffset);
    glEnd();

    // Back face (-Z)
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glVertex3f(-1, -1, -1 - faceOffset);
    glVertex3f(-1,  1, -1 - faceOffset);
    glVertex3f( 1,  1, -1 - faceOffset);
    glVertex3f( 1, -1, -1 - faceOffset);
    glEnd();

    // Left face (-X)
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-1 - faceOffset, -1, -1);
    glVertex3f(-1 - faceOffset, -1,  1);
    glVertex3f(-1 - faceOffset,  1,  1);
    glVertex3f(-1 - faceOffset,  1, -1);
    glEnd();

    // Right face (+X)
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glVertex3f(1 + faceOffset, -1, -1);
    glVertex3f(1 + faceOffset,  1, -1);
    glVertex3f(1 + faceOffset,  1,  1);
    glVertex3f(1 + faceOffset, -1,  1);
    glEnd();

    // Top face (+Y)
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-1, 1 + faceOffset, -1);
    glVertex3f(-1, 1 + faceOffset,  1);
    glVertex3f( 1, 1 + faceOffset,  1);
    glVertex3f( 1, 1 + faceOffset, -1);
    glEnd();

    // Bottom face (-Y)
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-1, -1 - faceOffset, -1);
    glVertex3f( 1, -1 - faceOffset, -1);
    glVertex3f( 1, -1 - faceOffset,  1);
    glVertex3f(-1, -1 - faceOffset,  1);
    glEnd();
}

// Отрисовка источника света (шарик)
void drawLight() {
    glPushMatrix();
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glColor3f(1.0f, 1.0f, 0.3f);
    glutSolidSphere(0.1, 16, 16);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLoadIdentity();
    gluLookAt(6.0, 4.0, 6.0,  0, 0, 0,  0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Рисуем источник света
    drawLight();

    // Вращение куба
    glPushMatrix();
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(rotZ, 0, 0, 1);
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void idle() {
    // Анимация граней
    if (expanding) {
        if (exploded && faceOffset < maxOffset) {
            faceOffset += offsetSpeed;
        } else if (!exploded && faceOffset > 0.0f) {
            faceOffset -= offsetSpeed;
        } else {
            expanding = false;
            faceOffset = exploded ? maxOffset : 0.0f;
        }
    }

    // Вращение света
    lightAngle += 0.01f;
    lightPos[0] = lightRadius * cos(lightAngle);
    lightPos[2] = lightRadius * sin(lightAngle);

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'a': rotX -= 5.0f; break;
        case 'd': rotX += 5.0f; break;
        case 'w': rotY -= 5.0f; break;
        case 's': rotY += 5.0f; break;
        case 'q': rotZ -= 5.0f; break;
        case 'e': rotZ += 5.0f; break;

        case ' ':
            if (!expanding) {
                exploded = !exploded;
                expanding = true;
            }
            break;

        case 'z':
            alpha = (alpha == 1.0f) ? 0.3f : 1.0f;
            break;
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chudo-Kubiq ischezaushiy");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
