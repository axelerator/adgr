#ifndef VECTORM_H
#define VECTORM_H
#include "values.h"

class VectorM {
  public:
    static geovalue *add(geovalue *a, geovalue *b, geovalue *c);
    static geovalue *diff(geovalue *a, geovalue *b, geovalue *c);
    static geovalue *times(geovalue *a, geovalue b, geovalue *c);
    static geovalue scalarProduct(geovalue *a, geovalue *b);
    static geovalue length(geovalue *a);
    static void normalize(geovalue *a);
    static geovalue *cross(geovalue *a, geovalue *b, geovalue *c);
    static void print (char *str, geovalue *a);
};
#endif
