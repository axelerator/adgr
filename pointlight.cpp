//
// C++ Implementation: pointlight
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pointlight.h"
#include <math.h>
#include "scene.h"

PointLight::PointLight(const Vector3D& p, intensity *d, Scene* scene)
 : Light(p,d,scene)
{
}


PointLight::~PointLight()
{
}


/*!
    \fn Light::emit(double factor)
 */
long PointLight::emitPhotons(long photonCount) {
  long photonHits = 0;
  double angle1, angle2;
  srand ( time(NULL) );

  Vector3D power(dif[0],dif[1],dif[2]);
  for (long i = 0; i < photonCount; ++i) { 
    angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    angle2 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    Vector3D direction( cos(angle1)*sin(angle2),
                        cos(angle2),
                        sin(angle1)*sin(angle2));
    if (scene->recievePhoton(position, power, direction, 10)) {
      ++photonHits;
    }
    emit photonEmitted();
  }
  return photonHits;
}


/*!
    \fn Light::emitCausticPhotons(double factor)
 */
long PointLight::emitCausticPhotons(long photonCount) {
  long photonHits = 0;
  double angle1, angle2;
  srand ( time(NULL) );

  Vector3D power(dif[0],dif[1],dif[2]);
  for (long i = 0; i < photonCount; ++i) {
    angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    angle2 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
    Vector3D direction( cos(angle1)*sin(angle2),
                        cos(angle2),
                        sin(angle1)*sin(angle2));
    if (scene->recieveCausticPhoton(position, power, direction)) {
      ++photonHits;
    }
    emit photonEmitted();
  }
  return photonHits;
}


/*!
    \fn PointLight::sampleCount()
 */
int PointLight::getSampleCount()
{
    return 1;
}


/*!
    \fn PointLight::*getSamplePositions()
 */
const Vector3D& PointLight::getSamplePosition(int) {
    return position;
}
