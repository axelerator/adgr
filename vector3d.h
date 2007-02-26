//
// C++ Interface: vector3d
//
// Description:
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <sstream>

/**
  @author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Vector3D {
  public:
    Vector3D();
    Vector3D(const Vector3D& v);
    Vector3D(const double *v);
    Vector3D(double fx, double fy, double fz);
    Vector3D(double fn);

    virtual ~Vector3D();

    Vector3D& operator+=(const Vector3D& v);
    Vector3D& operator-=(const Vector3D& v);
    Vector3D& operator*=(double f);
    Vector3D& operator/=(double f);
    Vector3D& operator=(const Vector3D& v);
    Vector3D& normalize();

    Vector3D operator+() const;
    Vector3D operator-() const;

    Vector3D operator+(const Vector3D& v) const;
    Vector3D operator-(const Vector3D& v) const;

    Vector3D operator%(const Vector3D& v) const;

    Vector3D operator+(double f) const;
    Vector3D operator-(double f) const;
    Vector3D operator*(double f) const;
    Vector3D operator/(double f) const;

    double operator*(const Vector3D& v) const;

    bool operator>(const Vector3D& v) const;
    bool operator<(const Vector3D& v) const;

    bool operator==(const Vector3D& v) const;
    bool operator!=(const Vector3D& v) const;

    double& operator[](unsigned int i);
    double operator[](unsigned int i) const;

    double length() const;
    double lengthSquare() const;

    Vector3D normal() const;
    double angleTo(Vector3D& rhs);

//     Vector3D randomHemi();

    double x;
    double y;
    double z;

  private:
};

const Vector3D operator+(double lhs, const Vector3D& rhs);
const Vector3D operator*(double lhs, const Vector3D& rhs);

std::ostream& operator<<(std::ostream& os, const Vector3D& v);

#endif
