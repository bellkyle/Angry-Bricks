#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "timer.h"


//#include "timer.h"
using namespace std;
//Timer game_time;
//Timer mole_time;

// Global variables

#define MIN_X_VIEW 0
#define MAX_X_VIEW 200
#define MIN_Y_VIEW 0
#define MAX_Y_VIEW 100
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 1000
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500

float point[4] = {0,0,0,0};
float xStart = 15.0;
float yStart = 15.0;
float currentX = xStart;
float currentY = yStart;
float xTran = 0;
float yTran = 0;
float angle = 0;
float red = 1.0;
float green = 0.0;
float blue = 0.0;
float velocity = 0;
float theta = 0;

bool launched = false;

Timer flight;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, MIN_Y_VIEW, MAX_Y_VIEW, MIN_Z_VIEW, MAX_Z_VIEW);
   srand(time(0));
   //flight.start_timer();
}


void brick(float x, float x2, float y, float y2, float z, float z2)
{

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x, y, z2);
  glVertex3f(x2, y, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x, y2, z2);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x, y, z);
  glVertex3f(x2, y, z);
  glVertex3f(x2, y2, z);
  glVertex3f(x, y2, z);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x, y2, z2);
  glVertex3f(x, y2, z);
  glVertex3f(x2, y2, z);
  glVertex3f(x2, y2, z2);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x, y, z2);
  glVertex3f(x, y, z);
  glVertex3f(x2, y, z);
  glVertex3f(x2, y, z2);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x2, y, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y2, z);
  glVertex3f(x2, y, z);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(red,green,blue);
  glVertex3f(x, y, z2);
  glVertex3f(x, y2, z2);
  glVertex3f(x, y2, z);
  glVertex3f(x, y, z);
  glEnd();
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
  if(launched)
  {
    if(currentY >= 0)
    {
    flight.end_timer();
    angle += 0.5;
    xTran = velocity * flight.elapsed_time() * cos(theta);
    yTran = velocity * flight.elapsed_time() * sin(theta) - ((9.8*flight.elapsed_time()*flight.elapsed_time())/2);
    //cout << xTran << " " << yTran << endl;
    currentX = xTran + xStart;
    currentY = yTran + yStart;
    if(currentY <= 0)
      flight.start_timer();
    //cout << currentX << " " << currentY << endl;
    }
    else
    {
      red = ((float) rand() / (RAND_MAX));
      green = ((float) rand() / (RAND_MAX));
      blue = ((float) rand() / (RAND_MAX));
      flight.end_timer();
      if(flight.elapsed_time()>3.0)
      {
        launched = false;
        xTran = 0;
        yTran = 0;
        angle = 0;
        red = 1.0;
        green = 0.0;
        blue = 0.0;
        currentY = yStart;
      }
    }


  }
  glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
  float xScale = (MAX_X_VIEW - MIN_X_VIEW) /
     (float)(MAX_X_SCREEN - MIN_X_SCREEN);
  float yScale = (MIN_Y_VIEW - MAX_Y_VIEW) /
     (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);
   // Handle mouse down
   if(!launched)
   {
   if (state == GLUT_DOWN)
   {
     float xPoint = MIN_X_VIEW + (x - MIN_X_SCREEN) * xScale;
     float yPoint = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * yScale;

     point[0] = xPoint;
     point[1] = yPoint;

   }

   else if (state == GLUT_UP)
   {

      point[2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * xScale;
      point[3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * yScale;
      float dx = point[0] - point[2];
      float dy = point[1] - point[3];
      float distance = sqrt(dx * dx + dy * dy);
      velocity = 1.25*distance;
      theta = atan((point[1]-point[3])/(point[0]-point[2]));
      flight.start_timer();
      launched = true;
     glutPostRedisplay();
   }
 }
}

void motion(int x, int y)
{
  float xScale = (MAX_X_VIEW - MIN_X_VIEW) /
     (float)(MAX_X_SCREEN - MIN_X_SCREEN);
  float yScale = (MIN_Y_VIEW - MAX_Y_VIEW) /
     (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);
  if(!launched)
  {
   point[2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * xScale;
   point[3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * yScale;

   xTran = point[2] - point[0];
   yTran = point[3] - point[1];

   glutPostRedisplay();
 }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glTranslatef(xTran, yTran, 0);
   glTranslatef(xStart,yStart,0.0);
   glRotatef(-angle, 0.3, 1.0, 0.5);
   //glRotatef(angle, 0.0, 1.0, 0.0);
   //glRotatef(angle, 0.0, 0.0, 1.0);
   glScalef(6, 3, 3);
   brick(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_X_SCREEN, MAX_Y_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("AngryBricks");
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
