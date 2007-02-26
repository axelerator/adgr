//
// C++ Interface: pointlight
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <light.h>

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class PointLight : public Light
{

public:
    PointLight(const Vector3D& p, intensity *d, Scene* scene);
    virtual ~PointLight();
    
    virtual long emitPhotons(long photonCount);
    virtual long emitCausticPhotons(long photonCount) ;
    virtual int getSampleCount();
    virtual const Vector3D& getSamplePosition(int i);
    

};

#endif
