#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <random>

float angle = M_PI;
bool isDay = true;
float minX = -2.f, maxX = 2.f; // Для рандома
float minY = 0.f, maxY = 2.f; // Для рандома


float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen); 
}

void update(int value) {
    if(angle <= -0.1f) { angle = M_PI + 0.1f; isDay = !isDay; }
    else { angle -= 0.01f; }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    static GLfloat houseCoords[] = {
        -0.8,-0.9,
        -0.8,-0.3,
        -0.4,-0.3,
        -0.4,-0.9
    };
    
    static GLfloat windowCoords[] = {
        -0.75, -0.6,
        -0.75, -0.4,
        -0.7, -0.4,
        -0.7, -0.6
    };
    
    static GLfloat doorCoords[] = {
        -0.6, -0.9,
        -0.6, -0.5,
        -0.45, -0.5,
        -0.45, -0.9,
    };
    
    static GLfloat roofCoords[] = {
        -0.9, -0.3,
        -0.3, -0.3,
        -0.6, 0.1
    };
    
    static GLfloat stwolCoord[] = {
            0.09,-0.9,
            0.09,-0.8,
            0.07,-0.8,
            0.07,-0.9
    };
    
    // Бэкграунд (если isDay - то светлое небо, если !isDay - тёмное небо и рандомно появляющиеся звёздочки (Эпилептикам привет))
    if(isDay) {
        glClearColor(0.529f, 0.808f, 0.922f, 1.);
    } else {
        glClearColor(0,0,0,1);
        glPointSize(10);
        glBegin(GL_POINTS);
            glColor3f(1,1,1);
            for(int i = 0; i < 15; i++) {
                glVertex2f(getRandomFloat(minX, maxX), getRandomFloat(minY, maxY));
            }
        glEnd();
    }

    glPointSize(150);
    glEnable(GL_POINT_SMOOTH);

    // Солнце или луна (зависит от isDay)
    glBegin(GL_POINTS);
        if(isDay) {
            glColor3f(1., 1., 0);
        } else {
            glColor3f(1., 1., 1);
        }
        glVertex2f(cos(angle) * 1.2, sin(angle) * 1.1 - 0.2);
    glEnd();

    glDisable(GL_POINT_SMOOTH);

    // Поляна
    glPointSize(60);
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.49f, 0.99f, 0.0f);
        for(float i = 0.f; i < M_PI; i += 0.05f) {
            glVertex2f(cos(i) * 1.4,-1.5 + sin(i) * 1.2);
        }
    glEnd();

    

    // Дом
    glColor3f(0.36f, 0.25f, 0.20f);
    glVertexPointer(2, GL_FLOAT, 0, houseCoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    if(isDay) { glColor3f(1,1,1); } \
    else { glColor3f(1,1,0); }
    glVertexPointer(2, GL_FLOAT, 0, windowCoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glColor3f(0.824f, 0.706f, 0.549f);  
    glVertexPointer(2, GL_FLOAT, 0, doorCoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glColor3f(0.87f, 0.72f, 0.29f);
    glVertexPointer(2, GL_FLOAT, 0, roofCoords);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glColor3f(0.36f, 0.25f, 0.20f);
    glVertexPointer(2, GL_FLOAT, 0, stwolCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBegin(GL_TRIANGLES);
        glColor3f(0.133, 0.545, 0.133);
        glVertex2f(-0.08, -0.8);
        glVertex2f(0.24, -0.8);
        glVertex2f(0.08,-0.6);

        glVertex2f(-0.04, -0.7);
        glVertex2f(0.2, -0.7);
        glVertex2f(0.08,-0.5);

        glVertex2f(-0.0, -0.6);
        glVertex2f(0.16, -0.6);
        glVertex2f(0.08,-0.4);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("Holy Moly");

    glEnableClientState(GL_VERTEX_ARRAY);

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();    

    return 0;
}