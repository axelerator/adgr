//
// C++ Interface: squarelight
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SQUARELIGHT_H
#define SQUARELIGHT_H

#include <light.h>

class Scene;
/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SquareLight : public Light
{

public:
    SquareLight(const Vector3D& p, const Vector3D& u, const Vector3D& v,intensity *d, const int sampleRes, Scene *scene);

    virtual ~SquareLight();
    virtual long emitPhotons(long photonCount);
    virtual long emitCausticPhotons(long photonCount) ;
    virtual void drawGL();
    virtual int getSampleCount();
    virtual const Vector3D& getSamplePosition(int i);
    virtual void setSampleCount(int sampleRes);
private:
    Vector3D u;
    Vector3D v;
    Vector3D normal;
    Vector3D corner;
    int sampleCount;
    QList <Vector3D> samplePositions;
    
};

#endif
