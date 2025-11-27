/*
 *
 *Práctica 03 de Computación visual
 *Alumno: Kevin Jesús Uscata Flores (kevin.uscata@unmsm.edu.pe)
 *Facultad de Ingeniería de Sistemas e Informática
 *Universidad Nacional Mayor de San Marcos
 *
 *Consideraciones:
 *1. Se utilizó freeglut por cuestiones de compatibilidad
 *2. Se utilizó las versiones CXX de las librerías (i.e. cstdio en lugar de stdio.h)
 *3. Compile el archivo con el estándar C++26. Asegúrese de que su compilador trabaje con el estándar.
 *
 *Atajos de teclado:
 *Rote la cámara con las teclas i/j/k/l
 *
 *w: Cambiar entre modo wireframe y modo sólido
 *e: Activar/desactivar líneas de referencia
 *a/s/d: Activar/desactivar rotación en los ejes x/y/z
 *+/-: Aumentar/reducir velocidad de rotación
 *1/2: Reducir/aumentar longitud de cilindros
 *3/4: Reducir/aumentar radio de cilindros
 *5/6: Reducir/aumentar radio de esferas
 *7/8: Reducir/aumentar proporción de las esferas externas
 *</z: Reducir/aumentar cantidad de cortes en los cilindros
 *x/c: Reducir/aumentar cantidad de stacks en los cilindros
 *v/b: Reducir/aumentar cantidad de cortes en las esferas
 *n/m: Reducir/aumentar cantidad de stacks en las esferas
 *p: Imprimir estado de rotación (ejes activos, velocidad)
 *
 *q: Salir
 *
 */

#include <cstdio>
#include <cmath>
#include <GL/glut.h>

struct Vec2f { GLfloat x, y; };
struct Vec3f { GLfloat x, y, z; };

// constexpr float FRAMERATE{60.f};
constexpr unsigned int POINT_COUNT{6};
constexpr Vec2f POINT_SET[6]{{-10, 0}, {-10, 10}, {10, 10}, {10, 0}, {10, -10}, {0, 0}};

float g_deltaTime{0.f};
float g_lastTime{0.f};

GLfloat angle{0}, angleIncrement{.1f};
Vec3f v1{0, 0, 5}, v2{0, 0, 4};
bool isRotationEnabled[]{false, false, false};
GLfloat theta[]{0, 0, 0};
GLint axis{2};

int g_samples{400};

bool g_drawBezier{true};
bool g_drawAxis{true};

namespace Bezier {
    long factorial(const long& n) {
        if(n == 0) return 1;
        return n * factorial(n - 1);
    }

    float newton(const int& n, const int& k) {
        return factorial(n) / static_cast<float>(factorial(k) * factorial(n-k));
    }

    float curve(const float& u, const int& coord) {
        float sum{0};
        for(int i{0}; i < POINT_COUNT; ++i) {
            const auto& [ x, y] = POINT_SET[i];
            sum += (coord ? y : x) * newton(POINT_COUNT - 1, i)
            * pow(u, POINT_COUNT - 1 - i) * pow(1 - u, i);
        }
        return sum;
    }
}

namespace Display {
    void points() {
        glBegin(GL_POINTS);
        for(const auto& [x, y] : POINT_SET) glVertex2f(x, y);
        glEnd();
    }

    void curve() {
        glBegin(GL_LINE_STRIP);
        for(int i{0}; i <= 100; ++i)
            glVertex2f(Bezier::curve(i / 100.f, 0), Bezier::curve(i / 100.f, 1));
        glEnd();
    }
}

void init() {
    glClearColor(1, 1, 1, 0);
    glShadeModel(GL_FLAT);
}

void drawAxis() {
    glBegin(GL_LINES);
    glVertex2f(-20, 0);
    glVertex2f(20, 0);
    glVertex2f(0, -20);
    glVertex2f(0, 20);
    glEnd();
}

namespace GlutHelpers {
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glColor3f(1, 0, 0);
        if(g_drawAxis) drawAxis();
        if(g_drawBezier) {
            glColor3f(0, 0, 1);
            glPointSize(5);
            Display::points();
            glPointSize(1);
            glColor3f(0, 1, 0);
            Display::curve();
        }
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
    }

    void reshape(int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // gluPerspective(60, static_cast<double>(w) / h, 0.1f, 30);
        glOrtho(-20, 20, -20, 20, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void idle() {
        const int now{glutGet(GLUT_ELAPSED_TIME)};
        g_deltaTime = (now - g_lastTime) / 1000.f;
        g_lastTime = now;

        display();
    }

    void keyboard(unsigned char key, int x, int y) {
        switch (key) {
            case 'b':
                g_drawBezier = !g_drawBezier;
                glutPostRedisplay();
                break;
            case 'l':
                g_drawAxis = !g_drawAxis;
                break;
            case 'q':
                exit(0);
            default:
                break;
        }
    }

    // void timer(int value) {
        // glutPostRedisplay();
        // glutTimerFunc(1000.f / FRAMERATE, timer, 0);
    // }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("OpenGL");

    init();

    glutDisplayFunc(GlutHelpers::display);
    glutReshapeFunc(GlutHelpers::reshape);
    glutIdleFunc(GlutHelpers::idle);
    glutKeyboardFunc(GlutHelpers::keyboard);
    // glutTimerFunc(0, GlutHelpers::timer, 0);


    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    // glShadeModel(GL_SMOOTH);

    glutMainLoop();
    return 0;
}
