//
// C++ Interface: color
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COLOR_H
#define COLOR_H

#include <iostream>

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Color{
  public:
    Color();

    Color(const Color& c);
    Color(const double *c);
    Color(double fr, double fg, double fb);
    Color(double fn);

    virtual ~Color();

    Color& operator+=(const Color& c);
    Color operator+(const Color& c) const;

    Color operator*(double f) const;

    double& operator[](unsigned int i);
    double operator[](unsigned int i) const;

    void setR(double get) { red = (get > 1.0) ? 1.0 : get; }
    double getRed() const { return red; }

    void setG(double get) { green = (get > 1.0) ? 1.0 : get; }
    double getGreen() const { return green; }

    void setB(double get) { blue = (get > 1.0) ? 1.0 : get; }
    double getBlue() const { return blue; }

  private:
    double red;
    double green;
    double blue;
};

std::ostream& operator<<(std::ostream& os, const Color& c);

#endif
