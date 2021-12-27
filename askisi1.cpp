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
GLfloat pts[7][3] = {{-20, -18, 0}, {-11, 7, 0}, {-5, -5, 0}, {0, 0, 0}, {5, 5, 0}, {8, 18, 0}, {26, -3, 0}};

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

/*
	Modifies the given control points so that when berzier is used,
	it will have the effect of a polynomial curve of the initial control points.
*/
// Polynomial to Bezier transformation matrix
float polyToBezier[4][4] = {
    {1, 0, 0, 0},
    {-0.833, 3, -1.5, 0.333},
    {0.333, -1.5, 3, -0.8333},
    {0, 0, 0, 1}};
void polynomialToBezierControlPoints(float bezier_points[4][3], float ctrlPoints[4][3])
{
    // Multiplies the control points with the proper matrix to convert them from polynomial to bezier
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            bezier_points[i][j] = 0; // Set initial value of every cell
            for (int k = 0; k < 4; k++)
            {
                bezier_points[i][j] += polyToBezier[i][k] * ctrlPoints[k][j];
            }
        }
    }
}

// //polynomial interpretation for N points
// float polyint(float points[][3], float x, int N)
// {
//     float y;

//     float num = 1.0, den = 1.0;
//     float sum = 0.0;

//     for (int i = 0; i < N; ++i)
//     {
//         num = den = 1.0;
//         for (int j = 0; j < N; ++j)
//         {
//             if (j == i)
//                 continue;

//             num = num * (x - points[j][0]); //x - xj
//         }
//         for (int j = 0; j < N; ++j)
//         {
//             if (j == i)
//                 continue;
//             den = den * (points[i][0] - points[j][0]); //xi - xj
//         }
//         sum += num / den * points[i][1];
//     }
//     y = sum;

//     return y;
// }

point get(list<point> _list, int _i)
{
    list<point>::iterator it = _list.begin();
    for (int i = 0; i < _i; i++)
    {
        ++it;
    }
    return *it;
}

float FindDistance(point pnt, float x, float y)
{
    return sqrt(pow(x - pnt.x, 2) +
                pow(y - pnt.y, 2) * 1.0);
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

            GLfloat pts[7][3] = {};
            int k = 0;
            list<point>::iterator it;
            for (it = ctrlpoints.begin(); it != ctrlpoints.end(); ++it)
            {
                pts[k][0] = it->x;
                pts[k][1] = it->y;
                pts[k++][2] = it->z;
            }
            GLfloat pts1[4][3];
            for (int i = 0; i < 4; i++)
            {
                pts1[i][0] = pts[i][0];
                pts1[i][1] = pts[i][1];
                pts1[i][2] = pts[i][2];
            }
            GLfloat pts2[4][3];
            for (int i = 3; i < 7; i++)
            {
                pts2[i-3][0] = pts[i][0];
                pts2[i-3][1] = pts[i][1];
                pts2[i-3][2] = pts[i][2];
            }
            GLfloat bezierpts[4][3];
            polynomialToBezierControlPoints(bezierpts, pts1);
            glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &bezierpts[0][0]);
            glEnable(GL_MAP1_VERTEX_3);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= 30; i++)
            {
                glEvalCoord1f((GLfloat)i / 30);
            }
            glEnd();
            GLfloat bezierpts2[4][3];
            glColor3f(1, 0, 1);

            polynomialToBezierControlPoints(bezierpts2, pts2);
            glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &bezierpts2[0][0]);
            glEnable(GL_MAP1_VERTEX_3);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= 30; i++)
            {
                glEvalCoord1f((GLfloat)i / 30);
            }
            glEnd();

            // glColor3f(1, 0, 1);
            // glBegin(GL_LINE_STRIP);
            // minx = get(ctrlpoints, 3);
            // maxx = get(ctrlpoints, 6);
            // for (int i = minx.x; i <= maxx.x; i++)
            // {
            //     float x = (float)i;

            //     list<point> tmpctrlpoints;
            //     tmpctrlpoints.assign(next(ctrlpoints.begin(), 3), next(ctrlpoints.begin(), 7));

            //     float tmp2ctrlp[4][3];
            //     int k = 0;
            //     for (point const &i : tmpctrlpoints)
            //     {
            //         tmp2ctrlp[k][0] = i.x;
            //         tmp2ctrlp[k][1] = i.y;
            //         tmp2ctrlp[k][2] = i.z;
            //         k++;
            //     }
            //     float y = polyint(tmp2ctrlp, x, 4);
            //     glVertex2f(x, y);
            // }
            // glEnd();
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
        if (ctrlpoints.size() == 7)
        {
            glColor3f(1.0, 1.0, 1.0);

            GLfloat pts[7][3] = {};
            int k = 0;
            list<point>::iterator it;
            for (it = ctrlpoints.begin(); it != ctrlpoints.end(); ++it)
            {
                pts[k][0] = it->x;
                pts[k][1] = it->y;
                pts[k++][2] = it->z;
            }
            glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 7, &pts[0][0]);
            glEnable(GL_MAP1_VERTEX_3);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= 30; i++)
            {
                glEvalCoord1f((GLfloat)i / 30);
            }
            glEnd();
        }
        glPushMatrix();
        glPointSize(5.0);
        glColor3f(0, 0, 1);
        glBegin(GL_POINTS);
        for (const point &point : ctrlpoints)
            glVertex3f(point.x, point.y, point.z);
        glEnd();
        glPopMatrix();
        break;
    case 2:
        glColor3f(1.0, 1.0, 1.0);

        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &pts[0][0]);
        glEnable(GL_MAP1_VERTEX_3);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++)
        {
            glEvalCoord1f((GLfloat)i / 30);
        }
        glEnd();

        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &pts[3][0]);
        glEnable(GL_MAP1_VERTEX_3);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++)
        {
            glEvalCoord1f((GLfloat)i / 30);
        }
        glEnd();

        glPushMatrix();
        glPointSize(5.0);
        glColor3f(0, 0, 1);
        glBegin(GL_POINTS);
        for (int i = 0; i < 7; i++)
            glVertex3f(pts[i][0], pts[i][1], pts[i][2]);
        glEnd();
        glPopMatrix();
        break;
    case 3:
        int SURFACE_LINES = 10;
        float const EDGES_X = 0;   // The x coordinate of the square that edges belong to (Y=Z, X=EDGES_X)
        float const EDGES_YZ = 30; // The maximum y and z axis values of the square edges belong to (max Y = max Z = EDGES_YZ)
        float const OTHER_X = 20;  // The x coordinate of the square that the rest control points belong to
        float ctrlPointsMode4[4][4][3] = {
            {{EDGES_X, EDGES_YZ / 2, EDGES_YZ}, {OTHER_X, EDGES_YZ / 2, 5}, {OTHER_X, EDGES_YZ / 2, -5}, {EDGES_X, EDGES_YZ / 2, -EDGES_YZ / 2}},
            {{OTHER_X, 5, EDGES_YZ / 2}, {OTHER_X, 5, 5}, {OTHER_X, 5, -5}, {OTHER_X, 5, -EDGES_YZ / 2}},
            {{OTHER_X, -5, EDGES_YZ / 2}, {OTHER_X, -5, 5}, {OTHER_X, -5, -5}, {OTHER_X, -5, -EDGES_YZ / 2}},
            {{EDGES_X, -EDGES_YZ / 2, EDGES_YZ}, {OTHER_X, -EDGES_YZ / 2, 5}, {OTHER_X, -EDGES_YZ / 2, -5}, {EDGES_X, -EDGES_YZ / 2, -EDGES_YZ / 2}}};
        glColor3f(0, 0, 0);
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &ctrlPointsMode4[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);
        // Draw surface
        for (int j = 0; j <= SURFACE_LINES; j++)
        {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= SURFACE_LINES; i++)
                glEvalCoord2f((float)i / SURFACE_LINES, (float)j / SURFACE_LINES);
            glEnd();
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= SURFACE_LINES; i++)
                glEvalCoord2f((float)j / SURFACE_LINES, (float)i / SURFACE_LINES);
            glEnd();
        }
        // // Draw control points
        // glPushMatrix();
        // glPointSize(5.0);
        // glColor3f(0, 0, 1);
        // glBegin(GL_POINTS);
        // for (int i = 0; i < 7; i++)
        //     glVertex3f(pts[i][0], pts[i][1], pts[i][2]);
        // glEnd();
        // glPopMatrix();
        break;
    }

    glutSwapBuffers();
    glFlush(); /* clear buffers */
}

void menu(int option)
{
    menuoption = option;
    ctrlpoints.clear();
    glutPostRedisplay();
}

bool leftButtonState = 0;
point *clickedpoint;
int clickedpoint2 = -1;
float d = 2; // maximum distance between clicking and point

void MouseFunc(int button, int state,
               int x, int y)
{
    // CONTROL POINTS
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if ((menuoption == 0 && ctrlpoints.size() < 7) || (menuoption == 1 && ctrlpoints.size() < 6))
        {
            point newpoint;
            newpoint.x = x / 8.0 - 50;
            newpoint.y = 50 - y / 8.0;
            newpoint.z = 0;
            ctrlpoints.push_back(newpoint);
            if (ctrlpoints.size() == 6 && menuoption == 1)
            {
                // make last point to be the same as the first one
                point lastpoint = get(ctrlpoints, 0);
                ctrlpoints.push_back(lastpoint);
            }
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // if we have 7 points and we havent clicked one already
        if (ctrlpoints.size() == 7 && clickedpoint == nullptr)
        {
            int k = 0;
            list<point>::iterator it;
            for (it = ctrlpoints.begin(); it != ctrlpoints.end(); ++it)
            {
                // for every point find if user clicked nearby
                if (FindDistance(*it, x / 8.0 - 50, 50 - y / 8.0) < d)
                {
                    cout << "CLOSE ENOUGH at point no " << k << endl;
                    leftButtonState = 1; // clicked
                    clickedpoint = &*it; // iterators have the value but are not the same as the pointer
                    // therefore we have to reference and then dereferenec
                    break;
                }
                k++;
            }
        }
        // merge these parts because its disgusting
        if (menuoption == 2 && clickedpoint2 == -1)
        {
            for (int i = 0; i < 7; i++)
            {
                point tmppoint;
                tmppoint.x = pts[i][0];
                tmppoint.y = pts[i][1];
                tmppoint.z = pts[i][2];

                // for every point find if user clicked nearby
                if (FindDistance(tmppoint, x / 8.0 - 50, 50 - y / 8.0) < d)
                {
                    cout << "CLOSE ENOUGH at point no " << i << endl;
                    leftButtonState = 1; // clicked
                    clickedpoint2 = i;
                    break;
                }
            }
        }
    }
    else
    {
        leftButtonState = 0;
        clickedpoint = nullptr;
        clickedpoint2 = -1;
    }

    glutPostRedisplay();
}
int n = 0;
void MouseDrag(int x, int y)
{
    if (leftButtonState)
    {
        // also merge :D
        if (menuoption == 2)
        {
            if (clickedpoint2 < 2 || clickedpoint2 > 4)
            { // moves freely
                pts[clickedpoint2][0] = x / 8.0 - 50;
                pts[clickedpoint2][1] = 50 - y / 8.0;
            }
            else if (clickedpoint2 == 2)
            { // Dependent point (2-4)
                pts[clickedpoint2][0] = x / 8.0 - 50;
                pts[clickedpoint2][1] = 50 - y / 8.0;

                // move the other dependent point
                // 2* center - other dependent
                pts[clickedpoint2 + 2][0] = (2 * pts[clickedpoint2 + 1][0]) - pts[clickedpoint2][0];
                pts[clickedpoint2 + 2][1] = (2 * pts[clickedpoint2 + 1][1]) - pts[clickedpoint2][1];
            }
            else if (clickedpoint2 == 4)
            {
                pts[clickedpoint2][0] = x / 8.0 - 50;
                pts[clickedpoint2][1] = 50 - y / 8.0;

                // move the other dependent point
                // 2* center - other dependent
                pts[clickedpoint2 - 2][0] = (2 * pts[clickedpoint2 - 1][0]) - pts[clickedpoint2][0];
                pts[clickedpoint2 - 2][1] = (2 * pts[clickedpoint2 - 1][1]) - pts[clickedpoint2][1];
            }
            else if (clickedpoint2 == 3)
            { // Connect point

                // move the dependent points around it based on the change in location
                pts[clickedpoint2 - 1][0] = pts[clickedpoint2 - 1][0] + (x / 8.0 - 50 - pts[clickedpoint2][0]);
                pts[clickedpoint2 - 1][1] = pts[clickedpoint2 - 1][1] + (50 - y / 8.0 - pts[clickedpoint2][1]);

                pts[clickedpoint2 + 1][0] = pts[clickedpoint2 + 1][0] + (x / 8.0 - 50 - pts[clickedpoint2][0]);
                pts[clickedpoint2 + 1][1] = pts[clickedpoint2 + 1][1] + (50 - y / 8.0 - pts[clickedpoint2][1]);

                // move itself
                pts[clickedpoint2][0] = x / 8.0 - 50;
                pts[clickedpoint2][1] = 50 - y / 8.0;
            }
        }
        else
        {
            clickedpoint->x = x / 8.0 - 50;
            clickedpoint->y = 50 - y / 8.0;
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
    glutMotionFunc(MouseDrag);

    glutCreateMenu(menu);
    glutAddMenuEntry("Cubic Interpolation Curve", 0);
    glutAddMenuEntry("Bezier Curve: 1 part/6th degree", 1);

    glutAddMenuEntry("Bezier Curve: 2 cubic parts", 2);
    glutAddMenuEntry("Bicubic interpolation surface", 3);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
