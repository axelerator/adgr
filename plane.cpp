#include <math.h>
#include <QtOpenGL>

#include "plane.h"
#include "scene.h"
#include "vector3d.h"

Plane::Plane(const Vector3D& p, const Vector3D& n, Material *m, Scene *s): Geometry(p, m, s), normal(n.normal()) {
  d = -(normal * position);
  
  // to generate a BB we have to assume a size
  // TODO: add parameters to init size properly
  
  double width = 40.0;
  double height = 40.0;
  
  // get a point in x-direction
  Vector3D xdir = p + Vector3D(1.0, 0.0, 0.0);
  double a = (n.x*xdir.x + n.y*xdir.y + n.y*xdir.y)/d;
  xdir = ((a * xdir) - p).normal() * width; 
  
  // get a point in z-direction
  Vector3D zdir = p + Vector3D(0.0, 0.0, 1.0);
  double b = (n.x*zdir.x + n.y*zdir.y + n.y*zdir.y)/d;
  zdir = ((b * zdir) - p).normal() * height; 
  
  this->aabbPos = this->position - xdir - zdir;
  this->aabbSize = 2 * (xdir+zdir);
  
}

/**
 * Calculates the cut of the ray r(t) = e + t*w with the plane. If the ray cuts the
 * plane, the point of intersection and the normal at that point will be determined.
 * 
 * @param e Part of the ray equation.
 * @param w Part of the ray equation.
 * @param resultPoint Used to return the point of intersection.
 * @param resultNormal Used to return the normal at the point of intersection.
 * 
 * @return The position of the intersection point on the ray (the lenght of the ray between
 *         the intersection point and the origin of the ray) or -1.0 if the ray does not intersect.
 */
geovalue Plane::cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal) {
  geovalue denominator = normal * w;

  geovalue numerator = (normal * e) + d;
  geovalue s = numerator / -denominator;

  if (s < 0.0) return -1.0;

  resultNormal = normal;
  resultPoint = (w * s) + e;

  return (resultPoint - e).length();
}


bool Plane::shadow(const Vector3D& e, const Vector3D& w, geovalue length) {
  geovalue denominator = normal * w;

//   if (fabsf(denominator) < 0.0001)
//     return false;

  geovalue numerator = (normal * e) + d;
  geovalue s = numerator / -denominator;

  if (s <= 0.0) return false;

  return (w * s).length() < length;
}

void Plane::drawGL() {
  glPointSize(2.0);
  glBegin(GL_POINTS);
  glVertex3f(position.x, position.y, -position.z);
  glEnd();
  
  glBegin(GL_LINES);
  glVertex3f(position.x, position.y, -position.z);
  glVertex3f(position.x + normal.x, position.y + normal.y, -position.z-normal.z);
  glEnd();
}

void Plane::refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection) {
  std::cerr << "This should not happen" << std::endl;
}
