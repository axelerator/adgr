#include <iostream>

#include "geometry.h"
#include "scene.h"
#include <cmath>

/**
 * Base constructor for all geometries. Every geometry belongs to a
 * scene and needs a position as well as an material property.
 */
Geometry::Geometry(const Vector3D& p, Material *m, Scene *s): position(p), material(m), scene(s) {
}

Geometry::~Geometry() {
}

/**
 * Calculates the local lightning at the given point of
 * this geometry.
 */
void Geometry::lbrdf(intensity *result, const Vector3D& point, const Vector3D& normal) {
  intensity difaccu[3] =  { 0.0, 0.0, 0.0};

  for (int i = 0; i < scene->lights.size(); ++i) {
    Vector3D l;
    Light *currLight = (scene->lights)[i];
    
    for (int sample = 0; sample < currLight->getSampleCount(); ++sample) {
      const Vector3D& currSamplePos = currLight->getSamplePosition(sample);
      l = currSamplePos - point;
      geovalue length = l.length();
      l = l.normal();
  
      // Shadow rays
      bool hit = 0;
      int i=0;
  
      while ((i < scene->objects.size() && !hit)) {
        hit = (this !=  (scene->objects)[i]) && (scene->objects)[i]->shadow(point, l, length);
        ++i;
      }
  
      if (!hit) {
        float distance = 1.0 + (length*length)*.1 + length*.2;
        geovalue fact = (normal * l) / (currLight->getSampleCount()*distance);
        if (fact > 0.0) {
          difaccu[0] += fact * currLight->getDif()[0] * material->getDif()[0];
          difaccu[1] += fact * currLight->getDif()[1] * material->getDif()[1];
          difaccu[2] += fact * currLight->getDif()[2] * material->getDif()[2];
        }
      }
    }
  }
  result[0] = difaccu[0];
  result[1] = difaccu[1];
  result[2] = difaccu[2];
}

/**
 * Returns the material
 */
Material* Geometry::getMaterial() const {
  return material;
}

/**
 * Set a material
 */
void Geometry::setMaterial(Material* m) {
  material = m;
}


