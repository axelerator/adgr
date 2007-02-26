//
// C++ Interface: squarelightbody
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SQUARELIGHTBODY_H
#define SQUARELIGHTBODY_H

#include <polygon.h>

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SquareLightBody : public PolygonRect
{
public:
    SquareLightBody(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4,intensity *dif, Scene *s);
    virtual void lbrdf(intensity *result, const Vector3D& point, const Vector3D& normal);
    virtual bool shadow(const Vector3D& e, const Vector3D& w, geovalue length);
    virtual ~SquareLightBody();



};

#endif
