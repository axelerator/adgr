//
// C++ Implementation: vector3d
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <cmath>

#include "vector3d.h"

/**
 * Create a zero vector.
 */
Vector3D::Vector3D() {
  x = 0.0;
  y = 0.0;
  z = 0.0;
}

/**
 * Create a copy of another vector. This is the copy-constructor.
 *
 * @param v The other vector.
 */
Vector3D::Vector3D(const Vector3D& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

/**
 * Create vector from an arry.
 *
 * @param v A vector as array.
 */
Vector3D::Vector3D(const double *v) {
  x = v[0];
  y = v[1];
  z = v[2];
}

/**
 * Create a vector from three values.
 *
 * @param fx X-value.
 * @param fy Y-value.
 * @param fz Z-value.
 */
Vector3D::Vector3D(double fx, double fy, double fz): x(fx), y(fy), z(fz) {
}

/**
 * Create a vector with the same value for x, y and z.
 *
 * @param fn The value.
 */
Vector3D::Vector3D(double fn) {
  x = y = z = fn;
}

Vector3D::~Vector3D() {
}

Vector3D& Vector3D::operator += (const Vector3D& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return (*this);
}

Vector3D& Vector3D::operator -= (const Vector3D& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return (*this);
}

Vector3D& Vector3D::operator *= (double f) {
  x *= f;
  y *= f;
  z *= f;
  return (*this);
}

Vector3D& Vector3D::operator /= (double f) {
  double s = 1.0f/f;
  x *= s;
  y *= s;
  z *= s;
  return (*this);
}

Vector3D& Vector3D::operator = (const Vector3D& v) {
  x = v.x;
  y = v.y;
  z = v.z;
  return (*this);
}

Vector3D Vector3D::operator + () const {
  return Vector3D(*this);
}

Vector3D Vector3D::operator - () const {
  return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator + (const Vector3D& v) const {
  return Vector3D(x+v.x, y+v.y, z+v.z);
}

Vector3D Vector3D::operator - (const Vector3D& v) const {
  return Vector3D(x-v.x, y-v.y, z-v.z);
}

/**
 * Calculate the cross product of the vector and another vector.
 *
 * @param v The other vector.
 * @return The cross product of the two vectors.
 */
Vector3D Vector3D::operator % (const Vector3D& v) const {
  return Vector3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

bool Vector3D::operator > (const Vector3D& v) const {
  return sqrt(x*x + y*y + z*z) > sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

bool Vector3D::operator < (const Vector3D& v) const {
  return sqrt(x*x + y*y + z*z) < sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3D Vector3D::operator + (double f) const {
  return Vector3D(x+f, y+f, z+f);
}

Vector3D Vector3D::operator - (double f) const {
  return Vector3D(x-f, y-f, z-f);
}

Vector3D Vector3D::operator * (double f) const {
  return Vector3D(x*f, y*f, z*f);
}

Vector3D Vector3D::operator / (double f) const {
  double s = 1.0f/f;
  return Vector3D(x*s, y*s, z*s);
}

bool Vector3D::operator == (const Vector3D& v) const {
  if ((x == v.x) && (y == v.y) && (z == v.z)) return true;
  return false;
}

bool Vector3D::operator != (const Vector3D& v) const {
  if ((x == v.x) && (y == v.y) && (z == v.z)) return false;
  return true;
}

/**
 * Allowing indexed access to the three members. This is here
 * just for compatability!!!
 */
double& Vector3D::operator [] (unsigned int i) {
  switch (i) {
   case 0: return x; break;
   case 1: return y; break;
   case 2: return z;
 }

  // We should throw some exception here...
  return x;
}

double Vector3D::operator [] (unsigned int i) const {
  switch (i) {
   case 0: return x; break;
   case 1: return y; break;
   case 2: return z;
 }

  // We should throw some exception here...
  return 0.0;
}

/**
 * Calculates the dot product of the vector and another vector.
 *
 * @param v The other vector.
 * @return The dot product of the two vectors.
 */
double Vector3D::operator * (const Vector3D& v) const { 
  return x*v.x + y*v.y + z*v.z;
}

/**
 * Calculates the length of the vector.
 *
 * @return Length of the vector.
 */
double Vector3D::length() const {
  return static_cast<double>(sqrt(x*x + y*y + z*z));
}

/**
 * Calculate the squared length of the vector.
 *
 * @return Squared length of the vector.
 */
double Vector3D::lengthSquare() const {
  return static_cast<double>(x*x + y*y + z*z);
}

/**
 * Return a normalzed version of this vector.
 *
 * @return The normalized vector.
 */
Vector3D Vector3D::normal() const {
  double s = 1.0f / length();
  return Vector3D(x*s, y*s, z*s);
}

const Vector3D operator+(double lhs, const Vector3D& rhs) {
  return Vector3D(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}

const Vector3D operator*(double lhs, const Vector3D& rhs) {
  return Vector3D(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

std::ostream& operator << (std::ostream& os, const Vector3D& v) {
  return os << "x: " << v.x << " y: " << v.y << " z: " << v.z << std::endl;
}



/**
 * Normalize the vector, that is, make it's length one.
 *
 * @return The normalized vector.
 */
Vector3D& Vector3D::normalize()
{
 double s = 1.0f / length();
 this->operator *=( s );
 return (*this);
}


/**
 * @return the angle between this and the provided vector
 */
double Vector3D::angleTo(Vector3D& rhs)
{
    return acos(operator *(rhs)/(length()*rhs.length()));
}
/*
Vector3D Vector3D::randomHemi() {
  Vector3D u(Vector3D(1.0, -x/y, 0.0).normal());
  Vector3D v((u%(*this)).normal());
  float angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
  float angle2 = (rand() / (RAND_MAX + 1.0)) * M_PI;
  Vector3D direction = ((cos(angle2)*(sin(angle1)*u.+cos(angle1)*v)) + sin(angle2)*this->normal());

}*/