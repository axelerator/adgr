#include <math.h>
#include <iostream>

#include "sphere.h"
#include "vector3d.h"
#include "scene.h"

GLuint Sphere::list;
geovalue Sphere::vdata[12][3] = {      {-X, 0.0,  Z},
                                       { X, 0.0,  Z},
                                       {-X, 0.0, -Z},
                                       { X, 0.0, -Z},

                                       { 0.0,  Z,  X},
                                       { 0.0,  Z, -X},
                                       { 0.0, -Z,  X},
                                       { 0.0, -Z, -X},

                                       {  Z,  X, 0.0},
                                       { -Z,  X, 0.0},
                                       {  Z, -X, 0.0},
                                       { -Z, -X, 0.0}};

GLuint Sphere::tindices[20][3] = {
                                   {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
                                   {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
                                   {3,10,7},{10,6,7,},{6,11,7},{6,0,11},{6,1,0},
                                   {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
                                 };

Sphere::Sphere(const Vector3D& p, Material *m, geovalue r, Scene *s): Geometry(p, m, s), radius(r) {
    this->aabbPos = this->position - Vector3D(r);
    this->aabbSize = 2 * Vector3D(r);

}

/**
 * Calculates the cut of the ray r(t) = e + t*w with the sphere. If the ray cuts the
 * sphere, the point of intersection and the normal at that point will be determined.
 * 
 * @param e Part of the ray equation.
 * @param w Part of the ray equation.
 * @param resultPoint Used to return the point of intersection.
 * @param resultNormal Used to return the normal at the point of intersection.
 * 
 * @return The position of the intersection point on the ray (the lenght of the ray between
 *         the intersection point and the origin of the ray) or -1.0 if the ray does not intersect.
 */
geovalue Sphere::cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal) {
  geovalue a = w * w;
  geovalue b = 2.0 * (w * (e - position));
  geovalue c = ((position - e) * (position - e)) - radius * radius;

  geovalue discr = b * b - 4 * (a * c);
  if (discr < 0.0)
    return -1.0; // not cut or touch

  discr = sqrt(discr);
  geovalue a2 = 2 * a;
  geovalue t1 = (-b + discr)/(a2);
  geovalue t2 = (-b - discr)/(a2);

  if ((t1 < 0.0) || (t2 < 0.0))
    return -2.0; // hit behind projection

  Vector3D p = w * fmin(t1, t2);
  resultPoint = e + p;
  resultNormal = (resultPoint - position).normal();

  return p.length();
}

bool Sphere::shadow(const Vector3D& e, const Vector3D& w, geovalue length) {
  geovalue a = w * w;
  geovalue b = 2.0 * (w * (e - position));
  geovalue c = ((position - e) * (position - e)) - radius * radius;

  geovalue discr = b * b - 4 * (a * c);
  if (discr < 0.0)
    return 0; // not cut or touch

  discr = sqrt(discr);
  geovalue a2 = 2 * a;
  geovalue t1 = (-b + discr)/(a2);
  geovalue t2 = (-b - discr)/(a2);

  if ((t1 < 0.0) || (t2 < 0.0))
    return 0; // hit behind surfacepoint

  Vector3D p = w * fmin(t1, t2);
  return (p.length() < length);

}
void Sphere::initDisplayList() {
   list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    for (int i =0; i< 20;++i)
     subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0],2);
    glEndList();
 
}

 

void Sphere::drawGL() {
  glColor3dv(material->getDif());
  glPushMatrix();
  glTranslated(position.x, position.y, -position.z);
  float factor = (radius);
  glScalef(factor, factor, factor);
  glCallList(Sphere::list);
 // for (int i =0; i< 20;++i)
 //   subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0],3);
  glPopMatrix();
}

void Sphere::drawTriangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) {

  glBegin(GL_TRIANGLES);
  glVertex3d(v1.x, v1.y, v1.z);
  glNormal3d(v1.x, v1.y, v1.z);
  glVertex3d(v2.x, v2.y, v2.z);
  glNormal3d(v2.x, v2.y, v2.z);
  glVertex3d(v3.x, v3.y, v3.z);
  glNormal3d(v3.x, v3.y, v3.z);
  glEnd();
}

void Sphere::subdivide(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, int depth) {
  Vector3D v12, v23, v31;

  if (!depth) {
    drawTriangle(v1, v2, v3);
    return;
  }

  v12 = ((v1 + v2) / 2.0).normal();
  v23 = ((v2 + v3) / 2.0).normal();
  v31 = ((v3 + v1) / 2.0).normal();

  subdivide(v1, v12, v31, depth-1);
  subdivide(v2, v23, v12, depth-1);
  subdivide(v3, v31, v23, depth-1);
  subdivide(v12, v23, v31, depth-1);
}

// currentRay = p + k*s
void Sphere::refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection) {
  Vector3D innerRay = refractRay(s, (p - position).normal(), 1.0, material->getRefractionIndex());

  // look for distant cut with sphere
  geovalue a = innerRay * innerRay;
  geovalue b = 2.0 * (innerRay * (p - position));
  geovalue c = ((position - p) * (position - p)) - radius * radius;

  geovalue discr = b * b - 4.0 * (a * c);
  discr = sqrt(discr);

  geovalue t1 = (-b + discr) / (2.0 * a);
  geovalue t2 = (-b - discr) / (2.0 * a);

  // the bigger t is the next intersection point
  if (fabs(t1) > fabs(t2))
    resultPoint = innerRay * t1 + p;
  else  
    resultPoint = innerRay * t2 + p;

  resultDirection = refractRay(innerRay, (position - resultPoint).normal(), material->getRefractionIndex(), 1.0);

//     resultDirection = innerRay;
}

/**
 * @param s Is the direction of an incoming (!) ray.
 * @param
 */
Vector3D Sphere::refractRay(const Vector3D& s, const Vector3D& n, double nFrom, double nTo) {
  Vector3D e = s;
  geovalue ne = e * n;

  geovalue reflection = 1.0 - pow((nFrom/nTo), 2.0) * (1.0 - pow(ne, 2.0));

  if (reflection < 0.0) {
    return Vector3D(0.0);
  }

  return Vector3D(((e - (n * ne)) * (nFrom/nTo) - (n * sqrt(reflection))).normal());
}
