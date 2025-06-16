#include <GL/freeglut.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Вращение источника света
float lightAngle = 0.0f;
float lightRadius = 5.0f;
GLfloat lightPos[] = {5.0f, 2.0f, 0.0f, 1.0f};
bool lightEnable = true;


GLuint textureID;
const char* textureFile = "cube_texture.jpg";
bool textureEnable = true;

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

void loadTexture()
{
    int width, height, channels;
    unsigned char* data = stbi_load(textureFile, &width, &height, &channels, 0);
    if (!data) {
        fprintf(stderr, "Не удалось загрузить текстуру: %s\n", textureFile);
        exit(1);
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // или GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}


void drawFace(float nx, float ny, float nz, float coords[4][3]) {
    float uv[4][2] = { {0,0}, {1,0}, {1,1}, {0,1} };
    
    glBegin(GL_QUADS);
    glNormal3f(nx, ny, nz);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(coords[i]);
        if(textureEnable) {
            glTexCoord2fv(uv[i]);
        }

    }
    glEnd();
}

void drawCube() {
    glBindTexture(GL_TEXTURE_2D, textureID);

    if(!textureEnable) {
        glColor4f(0.2f, 0.6f, 1.0f, alpha);
    } else {
        glColor4f(1.f, 1.f, 1.0f, alpha);
    }

    float o = faceOffset;  // Смещение граней наружу (exploded view)

    // Каждая грань — 4 вершины по 3 координаты
    float front[4][3]  = { {-1, -1, 1+o}, {1, -1, 1+o}, {1, 1, 1+o}, {-1, 1, 1+o} };
    float back[4][3]   = { {-1, -1, -1-o}, {-1, 1, -1-o}, {1, 1, -1-o}, {1, -1, -1-o} };
    float left[4][3]   = { {-1-o, -1, -1}, {-1-o, -1, 1}, {-1-o, 1, 1}, {-1-o, 1, -1} };
    float right[4][3]  = { {1+o, -1, -1}, {1+o, 1, -1}, {1+o, 1, 1}, {1+o, -1, 1} };
    float top[4][3]    = { {-1, 1+o, -1}, {-1, 1+o, 1}, {1, 1+o, 1}, {1, 1+o, -1} };
    float bottom[4][3] = { {-1, -1-o, -1}, {1, -1-o, -1}, {1, -1-o, 1}, {-1, -1-o, 1} };

    // Вызываем drawFace для каждой грани
    drawFace(0, 0, 1,  front);   // перед
    drawFace(0, 0, -1, back);    // зад
    drawFace(-1, 0, 0, left);    // лево
    drawFace(1, 0, 0,  right);   // право
    drawFace(0, 1, 0,  top);     // верх
    drawFace(0, -1, 0, bottom);  // низ
}

// Отрисовка источника света (шарик)
void drawLight() {
    if(lightEnable) {
        glPushMatrix();
        glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
        glColor3f(1.0f, 1.0f, 1.f);
        glutSolidSphere(0.1, 16, 16);
        glPopMatrix();
    }
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
        
        case 'l':
            lightEnable = !lightEnable;
            if (lightEnable) {
                glEnable(GL_LIGHTING);
            } else {
                glDisable(GL_LIGHTING);
            }
            break;

        case 'g':
            textureEnable = !textureEnable;
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    loadTexture();  

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("EBAT ETO KUB NAHUY AHUET MOZHNO");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
