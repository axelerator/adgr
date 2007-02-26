#include "vectorm.h"
#include "values.h"
#include "math.h"
#include "stdio.h"

geovalue *VectorM::add(geovalue *a, geovalue *b, geovalue *c) {
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
  return c;
}

geovalue *VectorM::diff(geovalue *a, geovalue *b, geovalue *c) {
  c[0] = a[0] - b[0];
  c[1] = a[1] - b[1];
  c[2] = a[2] - b[2];
  return c;
}

geovalue *VectorM::times(geovalue *a, geovalue b, geovalue *c) {
  c[0] = a[0] * b;
  c[1] = a[1] * b;
  c[2] = a[2] * b;
  return c;
}

geovalue VectorM::scalarProduct(geovalue *a, geovalue *b) {
  return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) ;
}

geovalue VectorM::length(geovalue *a) {
  return sqrt(scalarProduct(a,a)) ;
}

void VectorM::normalize(geovalue *a) {
  geovalue l = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
  a[0] /= l;
  a[1] /= l;
  a[2] /= l;
}

geovalue *VectorM::cross(geovalue *a, geovalue *b, geovalue *c) {
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
  return c;
}

void VectorM::print(char *str, geovalue *a) {
    printf("%s x: %f   y: %f   z: %f\n",str, a[0],a[1],a[2]);
}
