//
// C++ Interface: material
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MATERIAL_H
#define MATERIAL_H

#include "values.h"
enum photonBehavior {ABSORB, DIF_REFLECT, TRANSMIT, SPEC_REFLECT};


/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Material{
public:
    Material();
    Material(intensity r,intensity g,intensity b, intensity refl);

    ~Material();

    void setDif(intensity* get);
    intensity *getDif();
    void setReflectivity(intensity get);
    intensity getReflectivity() ;
    geovalue getRefractionIndex();
    void setRefractionIndex(geovalue set);
    geovalue getAlpha();
    void setAlpha(geovalue set);
    void updateRoulette();
    photonBehavior spinRoulette();
private:
    intensity dif[3];
    intensity reflectivity;
    geovalue refractIdx;
    geovalue alpha;
};

#endif
