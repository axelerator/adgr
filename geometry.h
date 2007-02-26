#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QList>

#include "values.h"
#include "material.h"
#include "vector3d.h"

class Scene;

/**
 * The base class for all geometries. Implements the local lightning and requires all
 * descendant classes to implement the cut and shadow methods. For convenience, the
 * descendant classes also should implement the drawGL methods for the OpenGL preview.
 */
class Geometry {

  public:
    Geometry(const Vector3D& p, Material *m, Scene *s);
    virtual ~Geometry();

    virtual geovalue cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal) = 0;
    virtual bool shadow(const Vector3D& e, const Vector3D& w, geovalue length) = 0;
    virtual void refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection) = 0;

    virtual void lbrdf(intensity *result, const Vector3D& point, const Vector3D& normal);

    void setMaterial(Material* m);
    Material* getMaterial() const;

    virtual void drawGL() = 0;
    Vector3D getPosition() const
    { return position; }

  protected:
    Vector3D position;      ///< Position of the geometry
    Material *material;     ///< Material descibing the surface of the geometry
    Scene *scene;           ///< The parent scene of the geometry

    Vector3D aabbPos;
    Vector3D aabbSize;
};

#endif
