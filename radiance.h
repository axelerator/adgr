//
// C++ Interface: radiance
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RADIANCE_H
#define RADIANCE_H

#include <iostream>

#include "color.h"

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Radiance{
  public:
    Radiance();
    Radiance(const Color& d, const Color& i, const Color& s, const Color& c);
    Radiance(const double d[3], const double i[3], const double s[3], const double c[3]);
    ~Radiance();

    double operator[](unsigned int i) const;

    void setCaustic(const Color& get) { caustic = get; updateCombined(); }
    const Color& getCaustic() const { return caustic; }

    void setDirect(const Color& get) { direct = get; updateCombined(); }
    const Color getDirect() const { return direct; }

    void setIndirect(const Color& get) { indirect = get; updateCombined(); }
    const Color getIndirect() const { return indirect; }

    void setSpecular(const Color& get) { specular = get; updateCombined(); }
    const Color getSpecular() const { return specular; }

    const Color getCombined() const { return combined; }

  private:
    void updateCombined();

    Color direct;
    Color indirect;
    Color specular;
    Color caustic;
    Color combined;
};

std::ostream& operator<<(std::ostream& os, const Radiance& r);

#endif
