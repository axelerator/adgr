#ifndef PLANE_H
#define PLANE_H

#include "values.h"
#include "geometry.h"

class Scene;

class Plane : public Geometry {

  public:
    Plane(const Vector3D& p, const Vector3D& n, Material *m, Scene *s) ;

    virtual geovalue cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal);
    virtual bool shadow(const Vector3D& e, const Vector3D& w, geovalue length);
    virtual void refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection);

    virtual void drawGL();

  private:
    geovalue width;
    geovalue height;
    Vector3D normal;
    geovalue d;
    
};

#endif
