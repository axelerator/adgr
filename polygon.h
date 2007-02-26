//
// C++ Interface: polygon
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POLYGONRECT_H
#define POLYGONRECT_H

#include <QList>
#include <QtOpenGL>

#include "geometry.h"
#include "values.h"



/**
 * @author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
 * 
 * ATTENTION! This class only implements a rectangle right now but should be
 * extended to support arbitrary convex polygons.
 */
class PolygonRect : public Geometry {
  public:
    PolygonRect(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4, Material *m, Scene *s);
    virtual ~PolygonRect();

    virtual geovalue cut(const Vector3D& e, const Vector3D& w, Vector3D &resultPoint, Vector3D &resultNormal);
    virtual bool shadow(const Vector3D& e, const Vector3D& w, geovalue length);
    virtual void refract(const Vector3D& p, const Vector3D& s, Vector3D &resultPoint, Vector3D &resultDirection);

    virtual void drawGL();

  private:
    bool isInside(const Vector3D& p);

    QList<Vector3D*> points;
    Vector3D v1, v2, v3, normal;
    geovalue d;
};

#endif
