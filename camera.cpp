#include <GL/gl.h>
#include <cmath>
#include <iostream>

#include "camera.h"
#include "renderarea.h"
#include "scene.h"
#include "radiance.h"

/**
 * Creates the camera with default properties and a specific scene.
 */
Camera::Camera(Scene *scene): 
target(0.0, 0.0, 3.0), 
upvector(0.0, 1.0, 0.0), 
justMap(false) {
  this->scene = scene;

  position.x = 0.0;
  position.z = 0.0;
  position.y = 0.0;
  d = 1.0;
  resolution[0] = 128;
  resolution[1] = 128;
  update();
}

/**
 * Creates the camera and initializes the cam with the given arguments.
 */
Camera::Camera(Vector3D p, Vector3D t, Vector3D u, geovalue d) : position(p), target(t), upvector(u), justMap(false) {
  d = d;
  resolution[0] = 128;
  resolution[1] = 128;
  update();
}

Camera::~Camera() {
}

/**
 * Recalculates the projection plane. Required after every change of
 * the camera position or direction.
 */
void Camera::update() {
  Vector3D eyeToTarget = (target - position).normal();

  u = (upvector % eyeToTarget).normal();
  v = (eyeToTarget % u).normal();

  // Calculate the center of the projectionplane
  Vector3D  pplaneCenter = position + (eyeToTarget * d);
  projOrigin = pplaneCenter - u - v;
  u = u * 2.0;
  v = v * 2.0;
}

/**
 * For convenience, show the camera in the OpenGL preview.
 */
void Camera::drawGL() {
  glPointSize(2.0);
  Vector3D eyeToTarget = position - target;

  glColor3f(0.5, 0.5, 1.0);
  glBegin(GL_LINES);

  glVertex3d(position.x,               position.y,               -position.z);
  glVertex3d(projOrigin.x,             projOrigin.y,             -(projOrigin.z));
  glVertex3d(position.x,               position.y,               -position.z);
  glVertex3d(projOrigin.x + u.x,       projOrigin.y + u.y,       -(projOrigin.z + u.z));
  glVertex3d(position.x,               position.y,               -position.z);
  glVertex3d(projOrigin.x + v.x,       projOrigin.y + v.y,       -(projOrigin.z + v.z));
  glVertex3d(position.x,               position.y,               -position.z);
  glVertex3d(projOrigin.x + u.x + v.x, projOrigin.y + u.y + v.y, -(projOrigin.z + u.z + v.z));

  glColor3f(0.5, 0.5, 0.5);

  glVertex3d(position.x, position.y, -position.z);
  glVertex3d(target.x, target.y, -target.z);
  glEnd();


  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POINTS);
  glVertex3d(temp.x, temp.y, -temp.z);
  glEnd();

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  glVertex3d(temp1.x, temp1.y, -temp1.z);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(projOrigin.x + u.x,       projOrigin.y + u.y,       -(projOrigin.z + u.z));
  glVertex3f(projOrigin.x,             projOrigin.y,             -(projOrigin.z));
  glColor3f(0.8, 0.8, 0.0);
  glVertex3f(projOrigin.x,             projOrigin.y,             -(projOrigin.z));
  glVertex3f(projOrigin.x + v.x,       projOrigin.y + v.y,       -(projOrigin.z + v.z));
  glVertex3f(projOrigin.x + v.x,       projOrigin.y + v.y,       -(projOrigin.z + v.z));
  glVertex3f(projOrigin.x + u.x + v.x, projOrigin.y + u.y + v.y, -(projOrigin.z + u.z + v.z));
  glVertex3f(projOrigin.x + u.x + v.x, projOrigin.y + u.y + v.y, -(projOrigin.z + u.z + v.z));
  glVertex3f(projOrigin.x + u.x,       projOrigin.y + u.y,       -(projOrigin.z + u.z));
  glEnd();
}

/**
 * Calculates the indirect term of global illumination.
 *
 */
void Camera::indirectIllumination(double radiance[3], Geometry *g,
                                  const Vector3D& position,
                                  const Vector3D& normal) {
  static const int MAX_SAMPLES = scene->getSettings().getSampleResolution();

  int usedSamples = 0;

  radiance[0] = radiance[1] = radiance[2] = 0.0;
  // determine a random direction(localU) in the plane defined by normal
  // choose a random ray
  Vector3D rrayDir(0.11 - normal.x, -0.11 - normal.y, 0.02 - normal.z );
  Vector3D rrayAnchor(normal);
  
  // calculate intersectionpoint with the plane
  geovalue denominator = normal * rrayDir;
  geovalue numerator = (normal * rrayAnchor);
  geovalue s = numerator / -denominator;

  Vector3D localU((rrayDir * s) + rrayAnchor);
  localU.normalize();// Point interpreted as direction is result
  
  // calculate a second direction inside plane
  Vector3D localV(normal%localU);
  localV.normalize();
  if ((g->getMaterial()->getAlpha() > 0.0) && (g->getMaterial()->getReflectivity() < 1.0)) {
    double angle1, angle2;
    float bit1 = (2.0 * M_PI)/MAX_SAMPLES; 
    float bit2 = M_PI/MAX_SAMPLES; 
    float bit25 = M_PI/(MAX_SAMPLES*2.0); 
    for (int t = 0; t < MAX_SAMPLES; ++t) {
      for (int i = 0; i < MAX_SAMPLES; ++i) {
          float angle1 = t*bit1 + (rand() / (RAND_MAX + 1.0))*bit2;
          float angle2 = i*bit2 + (rand() / (RAND_MAX + 1.0))*bit25;
          Vector3D newDirection = ((cos(angle2)*(sin(angle1)*localU+cos(angle1)*localV)) + sin(angle2)*normal);


        Radiance localRadiance;
        // Cast rays from the current position, but now we use the approximate calculation
        // to determine the radiance.
        if (raycast(position, newDirection, localRadiance, g, true)) {
          radiance[0] += localRadiance.getCombined().getRed();
          radiance[1] += localRadiance.getCombined().getGreen();
          radiance[2] += localRadiance.getCombined().getBlue();

          // Probably not needed anymore, because we do not distinguish between hit and non-hit.
          ++usedSamples;
        }
      }
    }

    double avg = 1.0 / usedSamples;

    // Perhaps we need to include reflectivity and alpha here.
    radiance[0] = radiance[0] * avg;
    radiance[1] = radiance[1] * avg;
    radiance[2] = radiance[2] * avg;
  }
}

/**
 * Calculates the direct term of global illumination.
 * 
 */
void Camera::directIllumination(double radiance[3], Geometry *g,
                                const Vector3D& position,
                                const Vector3D& normal) {
  radiance[0] = radiance[1] = radiance[2] = 0.0;

  geovalue reflectivity = g->getMaterial()->getReflectivity();
  geovalue alpha = g->getMaterial()->getAlpha();

  if ((alpha > 0.0) && (reflectivity < 1.0)) {
    g->lbrdf(radiance, position, normal);

    geovalue visibility = alpha * (1.0 - reflectivity);
    radiance[0] *= visibility;
    radiance[1] *= visibility;
    radiance[2] *= visibility;
  }
}

/**
 * Calculates the caustics term of global illumination.
 * 
 */
void Camera::causticsIllumination(double radiance[3], const Vector3D& position, const Vector3D& normal) {
  radiance[0] = radiance[1] = radiance[2] = 0.0;

  float localRadiance[3];
  localRadiance[0] = localRadiance[1] = localRadiance[2] = 0.0;

  float radius = scene->getSettings().getCausticsIrradRadius();
  int count = scene->getSettings().getCausticsIrradCount();
  scene->getCausticsMap()->irradiance_estimate(localRadiance, position, normal, radius, count);

  radiance[0] = localRadiance[0] * M_PI;
  radiance[1] = localRadiance[1] * M_PI;
  radiance[2] = localRadiance[2] * M_PI;
}

/**
 * Calculates the specular term of global illumination.
 * 
 */
void Camera::specularIllumination(double radiance[3], Geometry *g, const Vector3D& ray,
                                  const Vector3D& position, const Vector3D& normal, bool approximate) {
  radiance[0] = radiance[1] = radiance[2] = 0.0;

  Radiance reflected;
  Radiance refracted;

  geovalue reflectivity = g->getMaterial()->getReflectivity();
  geovalue alpha = g->getMaterial()->getAlpha();

  if (reflectivity > 0.0) {
    // Reflection
    geovalue vnn = (ray * normal) * 2.0;
    Vector3D reflectedDirection = (ray - (normal * vnn)).normal();
    raycast(position, reflectedDirection, reflected, g, approximate);
  }

  if (alpha < 1.0) {
    // Refraction
    Vector3D refractPoint, refractDirection;
    g->refract(position, ray, refractPoint, refractDirection);
    if (refractDirection.length() > 0.0) {
      raycast(refractPoint, refractDirection, refracted, g, approximate);
    }
  }

  // This probably needs some scaling by the alpha/reflectivity terms
  geovalue visibility = alpha * reflectivity;
  radiance[0] = visibility * reflected.getCombined().getRed() + (1.0 - alpha) * refracted.getCombined().getRed();
  radiance[1] = visibility * reflected.getCombined().getGreen() + (1.0 - alpha) * refracted.getCombined().getGreen();
  radiance[2] = visibility * reflected.getCombined().getBlue() + (1.0 - alpha) * refracted.getCombined().getBlue();
}

/**
 * This is where all the magic happens! Here the incoming ray is cut with every object
 * of the scene to calculate the color of the current pixel. The recursion for
 * calculation of reflective materials is done here, too.
 */
bool Camera::raycast(Vector3D p, Vector3D ray, Radiance& rad, Geometry *g, bool approximate) {
  geovalue z, zmin = 999.0;
  Vector3D lPoint, lNormal, rPoint, rNormal;
  int hitindex = -1;

  for (int i = 0; i < scene->objects.size(); ++i) {
    Geometry *s = (scene->objects.at(i));
    if (s != g) {
      z = s->cut(p, ray, lPoint, lNormal);
      if (( z > 0.0 ) && (z < zmin)) {
        zmin = z;
        hitindex = i;
        rPoint = lPoint;
        rNormal = lNormal;
      }
    }
  }

  intensity direct[3] = {0.0, 0.0, 0.0};
  intensity indirect[3] = {0.0, 0.0, 0.0};
  intensity specular[3] = {0.0, 0.0, 0.0};
  intensity caustics[3] = {0.0, 0.0, 0.0};

  if (hitindex >= 0) {
    Geometry *s = scene->objects.at(hitindex);

    // Calculate the specular illumination. This is needed for both the accurate
    // calculation and the approximate calculation of the radiance.
    specularIllumination(specular, s, ray, rPoint, rNormal, approximate);

    if (approximate) {
      // For an approximate calculation of the direct, indirect and caustic illumination
      // the global photon map can be used. It contains all three types of illumination.
      float localRadiance[3];
      localRadiance[0] = localRadiance[1] = localRadiance[2] = 0.0;
      float radius = scene->getSettings().getIrradRadius();
      int count = scene->getSettings().getIrradCount();
      scene->getPhotonMap()->irradiance_estimate(localRadiance, rPoint, rNormal, radius, count);
      indirect[0] = localRadiance[0] * M_PI;
      indirect[1] = localRadiance[1] * M_PI;
      indirect[2] = localRadiance[2] * M_PI;
    }
    else {
      // For accurate calculation all three terms have to be calculated independantly.
      directIllumination(direct, s, rPoint, rNormal);
      indirectIllumination(indirect, s, rPoint, rNormal);
      causticsIllumination(caustics, rPoint, rNormal);
    }

    // TODO: CLEANUP!!!
    rad = Radiance(direct, indirect, specular, caustics);

    return true;
  }
  else {
    // if a ray doesnt hit any object it returns the color black
    rad = Radiance();

    return false;
  }
}

/**
 * Starts the rendering process and draws the final image.
 */
void Camera::render(int w, int h) {
  resolution[0] = w;
  resolution[1] = h;
  layers.reset(w, h);
  justMap = false;
  start();
  setPriority(QThread::HighestPriority);
}

void Camera::renderMap(int w, int h) {
  resolution[0] = w;
  resolution[1] = h;
  layers.reset(w, h);
  justMap = true;
  start();
  setPriority(QThread::HighestPriority);
}

void Camera::run() {
  Vector3D projPlaneU;
  Vector3D projPlaneV;

  projPlaneV = v / resolution[1];
  projPlaneU = u / resolution[0];

  Vector3D projectPoint;
  Vector3D currentU;
  Vector3D currentV;
  Vector3D ray;

  abort = false;
  float percentPerPixel = (100.0/(float)resolution[0])/(float)resolution[1];
  // Calculate the color for every single pixel
  float progresss = 0;
  for (unsigned int y = 0; (y < resolution[1]) && !abort; ++y ) {
    for (unsigned int x = 0; (x < resolution[0]) && !abort; ++x ) {
      // Calculate the current ray based on the current pixel and the camera
      currentU = projPlaneU * x;
      currentV = projPlaneV * y;
      projectPoint = projOrigin + currentU + currentV;
      ray = (projectPoint - position).normal();

      // The color is actually calculated here
      Radiance r;
      if (justMap) {
        raycast(position, ray, r, NULL, true);
        // Rendering just the map is only for demonstration purposes, so we include some "magic factor" here
        // to produce a more lighter image.
        layers.setPixel(LayeredImage::MAP, x, resolution[1]-y, r.getCombined() * 2.0);
      }
      else {
        raycast(position, ray, r, NULL, false);
        layers.setPixel(x, resolution[1]-y, r);
      }

      progresss += percentPerPixel;
      emit progressChanged(lround(progresss));
    }
  }
  emit done(layers);
}

/**
 * Move the camera around.
 */
void Camera::move(geovalue *delta) {
  position += delta;
  update();
}

/**
 * Returns the camera position.
 */
Vector3D& Camera::getPosition() {
  return position;
}

/**
 * Set the new camera position.
 */
void Camera::setPosition(geovalue x, geovalue y) {
  position.x = x;
  position.y = y;
}

void Camera::abortRendering() {
  abort = true;
}
