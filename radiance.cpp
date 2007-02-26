//
// C++ Implementation: radiance
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <assert.h>

#include "radiance.h"

Radiance::Radiance(): direct(0.0), indirect(0.0), specular(0.0), caustic(0.0), combined(0.0) {
}

Radiance::Radiance(const Color& d, const Color& i, const Color& s, const Color& c): direct(d), indirect(i), 
                                                                                    specular(s), caustic(c) {
  updateCombined();
}

Radiance::Radiance(const double *d, const double *i, const double *s, const double *c): direct(d), indirect(i),
                                                                                        specular(s), caustic(c) {
  updateCombined();
}

Radiance::~Radiance() {
}

void Radiance::updateCombined() {
  combined = direct + indirect + specular + caustic;
}

double Radiance::operator[](unsigned int i) const {
  assert (i < 3);
  return combined[i];
}

std::ostream& operator << (std::ostream& os, const Radiance& r) {
  return os << "combined: " << r.getCombined() << " direct: " << r.getDirect() << " indirect: " << r.getIndirect() << " specular: " << r.getSpecular() << " caustic: " << r.getCaustic() << std::endl;
}
