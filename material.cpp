//
// C++ Implementation: material
//
// Description:
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "material.h"
#include <stdlib.h>

Material::Material() {
  dif[0] = 1.0;
  dif[1] = 1.0;
  dif[2] = 1.0;
  reflectivity = 0.0;
  alpha = 1.0;
  refractIdx = 1.0;
}

Material:: Material(intensity r, intensity g, intensity b, intensity refl) {
  dif[0] = r;
  dif[1] = g;
  dif[2] = b;
  reflectivity = refl;
  alpha = 1.0;
  refractIdx = 1.0;
}

Material::~Material() {}


intensity *Material::getDif() {
  return dif;
}


void Material::setDif(intensity *get) {
  dif[0] = get[0];
  dif[1] = get[1];
  dif[2] = get[2];
}


intensity Material::getReflectivity() {
  return reflectivity;
}


void Material::setReflectivity(intensity get) {
  reflectivity = get;
}

geovalue Material::getRefractionIndex() {
    return this->refractIdx;
}

void Material::setRefractionIndex(geovalue set) {
    this->refractIdx = set;
}

geovalue Material::getAlpha() {
    return alpha;
}

void Material::setAlpha(geovalue set) {
    alpha = set;
}



/**
 * 
 */
void Material::updateRoulette()
{
    /// @todo implement me
}

photonBehavior Material::spinRoulette() {
    float spin =  (rand() / (RAND_MAX + 1.0)) * (1.0 + reflectivity);
    float avg = (dif[0]+dif[1]+dif[2])*.33333;
    if ( spin < alpha)
        if (spin > alpha*avg)
            return ABSORB;
        else
            return DIF_REFLECT;
    else if ( spin > 1.0 )
        return SPEC_REFLECT;
    return TRANSMIT;    
}
