//
// C++ Implementation: polygon
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "polygon.h"

PolygonRect::PolygonRect(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4, 
                 Material *m, Scene *s) : Geometry(p1, m, s) {

  points.push_back(new Vector3D(p1));
  points.push_back(new Vector3D(p2));
  points.push_back(new Vector3D(p3));
  points.push_back(new Vector3D(p4));

  normal = ((p2 - p1) % (p4 - p1)).normalize();

  d = -(normal * position);
}

PolygonRect::~PolygonRect() {
}

geovalue PolygonRect::cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal) {
  geovalue denominator = normal * w;

  geovalue numerator = (normal * e) + d;
  geovalue s = numerator / -denominator;

  if (s < 0.0) return -1.0;

  resultNormal = normal;
  resultPoint = (w * s) + e;

  if (isInside(resultPoint)) {
    return (resultPoint - e).length();
  }
  else {
    return -1.0;   // Not cut
  }
}

bool PolygonRect::shadow(const Vector3D& e, const Vector3D& w, geovalue length) {
  geovalue denominator = normal * w;

  geovalue numerator = (normal * e) + d;
  geovalue s = numerator / -denominator;

  if (s <= 0.0) return false;

  Vector3D direction = w * s;

  if (direction.length() < length) {
    return isInside(direction + e);
  }

  return false;
}

bool PolygonRect::isInside(const Vector3D& p) {
  int s = points.size();
  for (int i = 0; i < s; ++i) {
    int j = i + 1;
    if (j == s) {
      j = 0;
    }

    Vector3D vn, vp, pi;
    pi = *points[i];
    vn = *points[j] - pi;
    vp = p - pi;
    if ((vn * vp) < 0.0) {
      return false;
    }
  }

  return true;
}

void PolygonRect::refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection) {
  std::cerr << "This should not happen" << std::endl;
}

void PolygonRect::drawGL() {
  glColor3dv(material->getDif());
  glBegin(GL_LINE_LOOP);
    for (int i = 0; i < points.size(); ++i) {
      glVertex3d(points[i]->x, points[i]->y, -points[i]->z);  // OpenGL has the z-axis reversed
    }
  glEnd();
}
