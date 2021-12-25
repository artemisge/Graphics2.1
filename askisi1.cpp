#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <cmath>
#include <list>
#define M_PI 3.14159265358979323846
using namespace std;

GLfloat cam[] = {0, 0, 70};
GLfloat camAngle = M_PI / 180 * 270;

typedef struct pointstruct
{
    GLfloat x, y, z;
} point;
list<point> ctrlpoints; // 7 control points

void myinit()
{
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST); // to see back of the cube, has to do with 3Dness
    glClearColor(0.5, 0.5, 0.5, 0);
    glOrtho(-50.0, 50.0, -50.0, 50.0, 0, 100); // αν ειναι αρνητικά, πάει πίσω από τον παρατηρητή

    glMatrixMode(GL_MODELVIEW); // peirazoume ton xoro kai ta simeia tora, oxi tin kamera
    gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);

    // glulookat explanation: https://stackoverflow.com/questions/5717654/glulookat-explanation/5721110
}

void idleFunc()
{

    //glutPostRedisplay();
}

//polynomial interpretation for N points
float polyint(float points[][3], float x, int N)
{
    float y;

    float num = 1.0, den = 1.0;
    float sum = 0.0;

    for (int i = 0; i < N; ++i)
    {
        num = den = 1.0;
        for (int j = 0; j < N; ++j)
        {
            if (j == i)
                continue;

            num = num * (x - points[j][0]); //x - xj
        }
        for (int j = 0; j < N; ++j)
        {
            if (j == i)
                continue;
            den = den * (points[i][0] - points[j][0]); //xi - xj
        }
        sum += num / den * points[i][1];
    }
    y = sum;

    return y;
}

point get(list<point> _list, int _i)
{
    list<point>::iterator it = _list.begin();
    for (int i = 0; i < _i; i++)
    {
        ++it;
    }
    return *it;
}

int menuoption = 0; //default
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
    //printf(" %f|%f|%f\n", cam[0], cam[1], cam[2]);

    switch (menuoption)
    {

    case 0:
        // PLOT CURVE
        if (ctrlpoints.size() == 7)
        {
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_STRIP);
            point minx = get(ctrlpoints, 0);
            point maxx = get(ctrlpoints, 3);
            for (int i = minx.x; i <= maxx.x; i++)
            {
                float x = (float)i;

                list<point> tmpctrlpoints;
                tmpctrlpoints.assign(next(ctrlpoints.begin(), 0), next(ctrlpoints.begin(), 4));

                float tmp2ctrlp[4][3];
                int k = 0;
                for (point const &i : tmpctrlpoints)
                {
                    tmp2ctrlp[k][0] = i.x;
                    tmp2ctrlp[k][1] = i.y;
                    tmp2ctrlp[k][2] = i.z;
                    k++;
                }
                float y = polyint(tmp2ctrlp, x, 4);
                glVertex2f(x, y);
            }
            glEnd();
            glColor3f(1, 0, 1);
            glBegin(GL_LINE_STRIP);
            minx = get(ctrlpoints, 3);
            maxx = get(ctrlpoints, 6);
            for (int i = minx.x; i <= maxx.x; i++)
            {
                float x = (float)i;

                list<point> tmpctrlpoints;
                tmpctrlpoints.assign(next(ctrlpoints.begin(), 3), next(ctrlpoints.begin(), 7));

                float tmp2ctrlp[4][3];
                int k = 0;
                for (point const &i : tmpctrlpoints)
                {
                    tmp2ctrlp[k][0] = i.x;
                    tmp2ctrlp[k][1] = i.y;
                    tmp2ctrlp[k][2] = i.z;
                    k++;
                }
                float y = polyint(tmp2ctrlp, x, 4);
                glVertex2f(x, y);
            }
            glEnd();
        }

        // PLOT POINTS
        //cout << point.x << "|" << point.y << "\n";
        glPushMatrix();
        glPointSize(5.0);
        glColor3f(0, 0, 0.0);
        glBegin(GL_POINTS);
        for (const point &point : ctrlpoints)
            glVertex3f(point.x, point.y, point.z);
        glEnd();
        glPopMatrix();
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }

    glutSwapBuffers();
    glFlush(); /* clear buffers */
}

void menu(int option)
{
    menuoption = option;
    glutPostRedisplay();
}

void MouseFunc(int button, int state,
               int x, int y)
{
    // CONTROL POINTS
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (ctrlpoints.size() < 7)
        {
            point newpoint;
            newpoint.x = x / 8.0 - 50;
            newpoint.y = 50 - y / 8.0;
            newpoint.z = 0;
            ctrlpoints.push_back(newpoint);
        }
    }

    glutPostRedisplay();
}

void SpecialKeyHandler(int key, int x, int y)
{
    // CAMERA left-right
    if (key == GLUT_KEY_RIGHT)
        camAngle += M_PI / 180 * 0.45;
    if (key == GLUT_KEY_LEFT)
        camAngle -= M_PI / 180 * 0.45;

    if (camAngle > M_PI / 180 * 360)
        camAngle -= M_PI / 180 * 360;
    if (camAngle < 0)
        camAngle += M_PI / 180 * 360;

    // now calculate new (x,z) for camera
    cam[0] = cos(camAngle) * 70; // 70 = radius
    cam[2] = -sin(camAngle) * 70;

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 150);
    glutCreateWindow("Askisi 1");
    myinit();
    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);
    glutSpecialFunc(SpecialKeyHandler);
    glutMouseFunc(MouseFunc);

    glutCreateMenu(menu);
    glutAddMenuEntry("Κυβική καμπύλη παρεμβολής", 0);
    glutAddMenuEntry("Καμπύλη Bezier: 1 τμήμα/6ου βαθμού", 1);

    glutAddMenuEntry("Καμπύλη Bezier: 2 κυβικά τμήματα", 2);
    glutAddMenuEntry("Δικυβική επιφάνεια παρεμβολής", 3);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
