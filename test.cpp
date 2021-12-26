/*  polyint.cpp			
 *  This program demonstrates using polynomial interpretation to draw a curve
 *  using Lagrange's method.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <iostream>
using namespace std;

GLfloat ctrlpoints[4][3] = {
    {-4.0, -4.0, 0.0}, {-2.0, 4.0, 0.0}, {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

int main(int argc, char **argv)
{
   typedef struct pointstruct
   {
      float x, y, z;
   } point;
   list<point> ctrlpoints;
   point newpoint;
   newpoint.x = 2;
   newpoint.y = 3;
   newpoint.z = 4;
   ctrlpoints.push_back(newpoint);

   list<point>::iterator it;
   for (it = ctrlpoints.begin(); it != ctrlpoints.end(); ++it)
   {
      point *ptr = &*it;
      cout << &it->x << endl;
      cout << ptr->x << it->x << endl;

      return 0;
   }
}