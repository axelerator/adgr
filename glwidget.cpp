#include <cmath>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "sphere.h"
#include "photonmap.h"
#define TILES 30.0  // Gridtiles
#define SIZE 30.0   // Gridsize

GLWidget::GLWidget(QWidget *parent):
QGLWidget(parent),
lookAt(0.0),
pos(0.0, -1.0, -2.0),
up(0.0, 1.0, 0.0) 
{
  object = 0;
  xRot = 0;
  yRot = 0;
  zRot = 0;

  clear = QColor::fromCmykF(0.0, 0.0, 0.0, 1.0);
  drawable = 0;
  angle = 0;
  move[0] = 0.0;
  move[1] = 0.01;
  move[2] = 0.0;
  
  lastCamPos[0] = 2.0;
  lastCamPos[1] = 1.0;
  
  yAng = xAng = 0;
  
  }

GLWidget::~GLWidget() {
  makeCurrent();
  glDeleteLists(object, 1);
}

QSize GLWidget::minimumSizeHint() const {
  return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
  return QSize(100, 100);
}

void GLWidget::setXRotation(int angle) {
  if (angle != xRot) {
    xRot = angle;
    updateGL();
  }
}

void GLWidget::setYRotation(int angle) {
  if (angle != yRot) {
    yRot = angle;
    updateGL();
  }
}

void GLWidget::setZRotation(int angle) {
  if (angle != zRot) {
    zRot = angle;
    updateGL();
  }
}

void GLWidget::initializeGL() {
  qglClearColor(clear);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_POINT_SMOOTH);
  Sphere::initDisplayList();
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
}


void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(xRot, 1.0, 0.0, 0.0);
  glRotatef(yRot, 0.0, 1.0, 0.0);
  glRotatef(zRot, 0.0, 0.0, 1.0);
  glTranslatef(pos[0], pos[1],  pos[2]);
  drawHelper();

  
  if (drawable) {
    drawable->drawGL();
  }
}

void GLWidget::setDrawable(Drawable *d) {
  drawable = d;
  updateGL();
}

void GLWidget::resizeGL(int width, int height) {
  glViewport(0,0, width, height);
  float sx,sy;
  if (width <= height) {
    sx = 1.0;
    sy = (float)height/width;
   } else {
    sx = (float)width/(height);
    sy = 1.0;
   }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-sx, sx, -sy, sy, 1, 20);
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
  update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRot + dy);
    setYRotation(yRot + dx);
  } else if (event->buttons() & Qt::RightButton) {
    float yRotRad = yRot/180.0 * M_PI; 
    Vector3D d1(cos(yRotRad), 0.0, sin(yRotRad));
    Vector3D d2(0.0, 0.04, 0.0);
    pos += (dx * 0.04) * d1;
    pos += dy * d2;
    updateGL();
  } else 
  if (event->buttons() & Qt::MidButton) {
    float yRotRad = yRot/180.0 * M_PI; 
    Vector3D d1(sin(yRotRad), 0.0, -cos(yRotRad));
    pos += (dy * 0.1) * d1;
    updateGL();
  }
  lastPos = event->pos();
}

void GLWidget::moveObject() {
  update();
}

void GLWidget::drawHelper() {
  float eins;
  glColor3f(0.2f, 0.2f, 0.1f);
  eins = (float)(SIZE/TILES);

  // Draw the grid
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for (int t=0; t<TILES; t++) {
    for (int i=0; i<TILES; i++) {
      glBegin(GL_POLYGON);
      glVertex3f(-SIZE/2.0 + t * eins + eins, 0.0, -SIZE/2.0 + i * eins);
      glVertex3f(-SIZE/2.0 + t * eins + eins,   0.0, -SIZE/2.0 + i * eins + eins);
      glVertex3f(-SIZE/2.0 + t * eins,  0.0, -SIZE/2.0 + i * eins + eins);
      glVertex3f(-SIZE/2.0 + t * eins , 0.0, -SIZE/2.0 + i * eins);
      glEnd();
    }
  }

  glBegin(GL_LINES);
  glColor3f(0.6f, 0.0f, 0.0f);
  glVertex3f(-10.0, 0.001,0.0);
  glVertex3f(10.0, 0.001,0.0);

  glColor3f(0.0f, 0.6f, 0.0f);
  glVertex3f(0.0, 10.0,0.0);
  glVertex3f(0.0, -10.0 ,0.0);

  glColor3f(0.0f, 0.0f, 0.6f);
  glVertex3f(0.0, 0.001 ,10.0);
  glVertex3f(0.0, 0.001, -10.0);

  glEnd();
}

