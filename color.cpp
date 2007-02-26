//
// C++ Implementation: color
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

#include "color.h"

Color::Color(): red(0.0), green(0.0), blue(0.0) {
}

Color::Color(const Color& c): red(c.getRed()), green(c.getGreen()), blue(c.getBlue()) {
}

Color::Color(const double *c) {
 red = c[0] > 1.0 ? 1.0 : c[0];
 green = c[1] > 1.0 ? 1.0 : c[1];
 blue = c[0] > 1.0 ? 1.0 : c[2];
}

Color::Color(double fr, double fg, double fb) {
 red = fr > 1.0 ? 1.0 : fr;
 green = fg > 1.0 ? 1.0 : fg;
 blue = fb > 1.0 ? 1.0 : fb;
}

Color::Color(double fn): red(fn), green(fn), blue(fn) {
}

Color::~Color() {
}

Color& Color::operator+=(const Color& c) {
  red += c.getRed();
  green += c.getGreen();
  blue += c.getBlue();
  return (*this);
}

Color Color::operator+(const Color& c) const {
  return Color(red + c.getRed(), green + c.getGreen(), blue + c.getBlue());
}

Color Color::operator*(double f) const {
  return Color(red * f, green * f, blue * f);
}

double& Color::operator[](unsigned int i) {
  assert (i < 3);
  switch (i) {
    case 0: return red; break;
    case 1: return green; break;
    case 2: return blue; break;
  }
  // This should not happen
  return red;
}

double Color::operator[](unsigned int i) const {
  assert (i < 3);
  switch (i) {
    case 0: return red; break;
    case 1: return green; break;
    case 2: return blue; break;
  }
  // This should not happen
  return red;
}

std::ostream& operator << (std::ostream& os, const Color& c) {
  return os << "r: " << c.getRed() << " g: " << c.getGreen() << " b: " << c.getBlue() << std::endl;
}
