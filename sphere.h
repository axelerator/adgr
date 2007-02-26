#ifndef SPHERE_H
#define SPHERE_H

#include <QtOpenGL>

#include "geometry.h"
#include "light.h"
#include "values.h"

#define X .525731112119133606
#define Z .850650808352039932

class Scene;

class Sphere : public Geometry {
public:
    Sphere(const Vector3D& p, Material *m, geovalue r, Scene *s);

    virtual geovalue cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal);
    virtual bool shadow(const Vector3D& e, const Vector3D& w, geovalue length);
    virtual void refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection);

    virtual void drawGL();
    static void initDisplayList();
	geovalue getRadius() const
	{
	  return radius;
	}
	

private:
    static void drawTriangle(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);
    static void subdivide(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, int depth);
    Vector3D refractRay(const Vector3D& s, const Vector3D& n, double nIn, double nOut);

    static GLuint list;
    static geovalue vdata[12][3];
    static GLuint tindices[20][3];

    geovalue radius;

};

#endif
