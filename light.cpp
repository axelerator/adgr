#include <cstdlib>
#include <cmath>

#include <QtOpenGL>
#include <QProgressBar>

#include "light.h"
#include "scene.h"


Light::Light(const Vector3D& p, intensity *d, Scene *scene): position(p), scene(scene) {
  dif[0] = d[0];
  dif[1] = d[1];
  dif[2] = d[2];
}

Light::~Light() {}

void Light::drawGL() {
  glPointSize(13.0);
  glColor3dv(dif);
  glBegin(GL_POINTS);
    glVertex3d(position.x, position.y, -position.z);
  glEnd();
}





/*!
    \fn Light::getAverageColor()
 */
double Light::getAverageColor()
{
    return (dif[0]+dif[1]+dif[2])/3.0;
}
