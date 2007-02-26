//
// C++ Implementation: squarelight
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "squarelight.h"
#include <math.h>
#include "scene.h"

SquareLight::SquareLight(const Vector3D& p, const Vector3D& u, const Vector3D& v,intensity *d, const int sampleRes, Scene *scene)
 : Light(p,d,scene), u(u), v(v), normal((u%v).normal()), corner(p - u*0.5 - v*0.5), sampleCount(sampleRes*sampleRes)
{
  Vector3D ustep(u*(1.0/sampleRes));
  Vector3D vstep(v*(1.0/sampleRes));
  for (int i = 0; i < sampleRes; ++i)
    for (int t = 0; t < sampleRes; ++t)
      samplePositions.push_back(Vector3D(corner + (i*ustep) + (t*vstep)));

}


SquareLight::~SquareLight()
{
}




/*!
    \fn Light::emit(double factor)
 */
long SquareLight::emitPhotons(long photonCount) {
  long photonHits = 0;
  double angle1, angle2, uOffset, vOffset;
  srand ( time(NULL) );
  
  Vector3D power(dif[0],dif[1],dif[2]);
  for (long i = 0; i < photonCount; ++i) { 
/*    angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    angle2 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;*/
    uOffset = (rand() / (RAND_MAX + 1.0));
    vOffset = (rand() / (RAND_MAX + 1.0));
/*    Vector3D direction( cos(angle1)*sin(angle2),
                        cos(angle2),
                        sin(angle1)*sin(angle2));*/
    angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    angle2 = (rand() / (RAND_MAX + 1.0)) * M_PI;
    Vector3D direction = ((cos(angle2)*(sin(angle1)*u.normal()+cos(angle1)*v)) + sin(angle2)*normal).normal();
    direction += normal;
    direction.normalize();
    // TODO: Something goes wrong here ...
    if (scene->recievePhoton(corner + vOffset*v + uOffset*u, power, direction, 10)) {
      ++photonHits;
    }
    emit photonEmitted();
  }
  return photonHits;
}


/*!
    \fn Light::emitCausticPhotons(double factor)
 */
long SquareLight::emitCausticPhotons(long photonCount) {
  long photonHits = 0;
  double angle1, angle2;
  srand ( time(NULL) );


  // determine a random direction(localU) in the plane defined by normal
  // choose a random ray
  Vector3D rrayDir(0.11 - normal.x, -0.11 - normal.y, 0.02 - normal.z );
  Vector3D rrayAnchor(normal);
  
  // calculate intersectionpoint with the plane
  geovalue denominator = normal * rrayDir;
  geovalue numerator = (normal * rrayAnchor);
  geovalue s = numerator / -denominator;

  Vector3D localU((rrayDir * s) + rrayAnchor);
  localU.normalize();// Point interpreted as direction is result
  
  // calculate a second direction inside plane
  Vector3D localV(normal%localU);
  localV.normalize();

  Vector3D power(dif[0],dif[1],dif[2]);
  for (long i = 0; i < photonCount; ++i) {
    angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    angle2 = (rand() / (RAND_MAX + 1.0)) * M_PI;
    Vector3D direction = ((cos(angle2)*(sin(angle1)*localU+cos(angle1)*localV)) + sin(angle2)*normal);

    if (scene->recieveCausticPhoton(position, power, direction)) {
      ++photonHits;
    }
    emit photonEmitted();
  }
  return photonHits;
}

void SquareLight::drawGL() {
  glPointSize(2.0);
  glColor3dv(dif);
  glBegin(GL_POINTS);
    glVertex3d(position.x, position.y, -position.z);
  glEnd();
  
  glPointSize(1.0);

  glBegin(GL_POINTS);
   for (int i = 0; i < sampleCount; ++i)
    glVertex3f(samplePositions[i].x,samplePositions[i].y, -samplePositions[i].z);
  glEnd();
  glBegin(GL_LINE_LOOP);
    glVertex3d(corner.x, corner.y, -corner.z);
    glVertex3d(corner.x + u.x , corner.y + u.y, -corner.z - u.z);
    glVertex3d(corner.x + u.x +v.x, corner.y + u.y + v.y, -corner.z - u.z - v.z);
    glVertex3d(corner.x + v.x , corner.y + v.y, -corner.z - v.z);
  glEnd();
  
  glBegin(GL_LINES);
    glVertex3d(position.x, position.y, -position.z);
    glVertex3d(position.x +  normal.x, position.y +  normal.y, -position.z - normal.z);
  glEnd();

}


/*!
    \fn SquareLight::sampleCount()
 */
int SquareLight::getSampleCount()
{
   return this->sampleCount;
}


/*!
    \fn SquareLight::*getSamplePositions()
 */
const Vector3D& SquareLight::getSamplePosition(int i)
{
    return samplePositions[i];
}

void SquareLight::setSampleCount(int sampleRes) {
  sampleCount = sampleRes * sampleRes;
  samplePositions.clear();
  Vector3D ustep(u*(1.0/sampleRes));
  Vector3D vstep(v*(1.0/sampleRes));
  for (int i = 0; i < sampleRes; ++i)
    for (int t = 0; t < sampleRes; ++t)
      samplePositions.push_back(Vector3D(corner + (i*ustep) + (t*vstep)));
}
