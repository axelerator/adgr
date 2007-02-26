//
// C++ Implementation: squarelightbody
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "squarelightbody.h"

SquareLightBody::SquareLightBody(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4, intensity *dif, Scene *s)
 : PolygonRect(p1, p2, p3, p4, new Material(dif[0], dif[1], dif[2], 0.0), s)
{
}

void SquareLightBody::lbrdf(intensity *result, const Vector3D& point, const Vector3D& normal) {
result[0] = material->getDif()[0];
result[1] = material->getDif()[1];
result[2] = material->getDif()[2];
}

bool SquareLightBody::shadow(const Vector3D& e, const Vector3D& w, geovalue length) {
  return false;
}


SquareLightBody::~SquareLightBody() {
  // this is deleted here because this lightgeomatry uses it's own material, which is not managed with the others
  delete material;
}


