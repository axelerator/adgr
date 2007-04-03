#include <cmath>

#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "polygon.h"
#include "material.h"
#include "values.h"
#include "squarelight.h"
#include "squarelightbody.h"

Scene::Scene() : viewportCam(this), photonMap(0), causticsMap(0) {

}

Scene::~Scene() {
  clear();
}

void Scene::addLight(geovalue *pos, intensity *dif) {
  Vector3D u = Vector3D(0.5, 0.0 , 0.0);
  Vector3D v = Vector3D(0.0, 0.0 , 0.5);
  lights.push_back(new SquareLight(pos,u ,v , dif, 3, this));
  Vector3D orig(Vector3D(pos) - (u * 0.5) - (v * 0.5));
  objects.push_back(new SquareLightBody(orig, orig + u, orig + u + v, orig + v, dif, this));
  currentCam = -1;
}

void Scene::addSphere(geovalue *pos, geovalue rad, Material *mat) {
  Vector3D p1(pos[0],pos[1],pos[2]);
  for (int i=0; i<objects.size(); ++i) {
    Sphere * s;
    if ((s = dynamic_cast<Sphere*>(objects[i]))) {
        Vector3D distvec = s->getPosition() - p1;
        float dist = distvec.length();
        float minDist = rad + s->getRadius();
        if (dist < minDist) {
            rad = (minDist - s->getRadius())/2.0;
            pos[1] = -1.0 + rad; 
        }
    }
  }
  objects.push_back(new Sphere(pos, mat, rad, this));
}

void Scene::addPlane(geovalue *pos, geovalue *normal, Material *mat) {
  objects.push_back(new Plane(pos, normal, mat, this));
}

void Scene::addRectangle(geovalue *p1, geovalue *p2, geovalue *p3, geovalue *p4, Material *mat) {
  objects.push_back(new PolygonRect(p1, p2, p3, p4, mat, this));
}

void Scene::clear() {
  QList<Geometry*>::ConstIterator i;
  for (i = objects.begin(); i != objects.constEnd(); i++) {
    delete *i;
  }
  QList<Light*>::ConstIterator j;
  for (j = lights.begin(); j != lights.constEnd(); j++) {
    delete *j;
  }  
  objects.clear();
  lights.clear();
  if (photonMap)
    delete photonMap;
  if (causticsMap)
    delete causticsMap;
}

void Scene::drawGL() {
            for (int i = 0; i < objects.size(); ++i) {
                Geometry *s = (objects.at(i));
                s->drawGL();
            }
            for (int i = 0; i < lights.size(); ++i) {
                Light *l = (lights.at(i));
                l->drawGL();
            }
            viewportCam.drawGL();
}

/*!
    \fn Scene::moveCurrentObject(geovalue *delta)
 */
void Scene::moveCurrentObject(geovalue *delta) {
   viewportCam.move(delta);
}

/*!
    \fn Scene::render(RenderArea *canvas)
 */
void Scene::render() {
  for (int i = 0; i < lights.size(); ++i) {
      Light *l = (lights.at(i));
      l->setSampleCount(settings.getAreaLightResolution());
  }
  viewportCam.render(settings.getWidth(), settings.getHeight());

  emit maximumChanged(100);
  connect(&viewportCam, SIGNAL(progressChanged(int)), this, SLOT(updateRendering(int )));
  connect(&viewportCam, SIGNAL(done(const LayeredImage&)), this, SLOT(emitRenderingDone(const LayeredImage&)));
}

void Scene::renderMap() {
  viewportCam.renderMap(settings.getWidth(), settings.getHeight());

  emit maximumChanged(100);
  connect(&viewportCam, SIGNAL(progressChanged(int)), this, SLOT(updateRendering(int )));
  connect(&viewportCam, SIGNAL(done(const LayeredImage&)), this, SLOT(emitRenderingMapDone(const LayeredImage&)));
}

/*!
    \fn Scene::alterCam(float x, float y)
 */
void Scene::alterCam(float x, float y)
{
    viewportCam.setPosition(x,y);
}

/*!
    \fn Scene::recievePhoton(const Vector3D&, const Vector3D& power, const Vector3D& direction)
 */
bool Scene::recievePhoton(const Vector3D& position, const Vector3D& power, const Vector3D& direction, unsigned int depth)
{
  
  geovalue z, zmin = 999.0;
  Vector3D lPoint, lNormal, rPoint, rNormal;
  int hitindex;
  hitindex = -1;

  for (int i = 0; i < objects.size(); ++i) {
    Geometry *s = (objects.at(i));
//     if (s != g) {
      z = s->cut(position, direction, lPoint, lNormal);
      if (( z > 0.0 ) && (z < zmin)) {
        zmin = z;
        hitindex = i;
        rPoint = lPoint;
        rNormal = lNormal;
      }
//     }
  }

  if (hitindex >= 0) { // the photon hit an object
    Geometry *s = objects.at(hitindex);
    photonBehavior behav = ABSORB;
    if (depth == 0)
      if ((s->getMaterial()->getAlpha() > 0.0) || (s->getMaterial()->getReflectivity() < 1.0))
        behav = ABSORB;// in case we have to abort recursion and material is somehow diffuse
      else
        return true;
    else
        behav = s->getMaterial()->spinRoulette();
    switch (behav) {
        case (DIF_REFLECT):{// we want to find a point on the hemisphere, to determine a random direction
                              // determine a random direction(localU) in the plane defined by normal
                              // choose a random ray
                              Vector3D rrayDir(0.11 - rNormal.x, -0.11 - rNormal.y, 0.02 - rNormal.z );
                              Vector3D rrayAnchor(rNormal);
                              
                              // calculate intersectionpoint with the plane
                              geovalue denominator = rNormal * rrayDir;
                              geovalue numerator = (rNormal * rrayAnchor);
                              geovalue s = numerator / -denominator;
                            
                              Vector3D localU((rrayDir * s) + rrayAnchor);
                              localU.normalize();// Point interpreted as direction is result
                              
                              // calculate a second direction inside plane
                              Vector3D localV(rNormal%localU);
                              localV.normalize();
                              float angle1 = (rand() / (RAND_MAX + 1.0)) * 2.0 * M_PI;
                              float angle2 = (rand() / (RAND_MAX + 1.0)) * M_PI;
                              Vector3D newDirection = ((cos(angle2)*(sin(angle1)*localU+cos(angle1)*localV)) + sin(angle2)*rNormal);

                           // TODO: find a better solution for the following
                           // move point a bit along new direction to avoid collision with the same object
                           rPoint += (newDirection * 0.0001);
                           this->recievePhoton(rPoint, power, newDirection, depth-1);
                       // no break here, because photon gets stored every time it hits a diffuse surface
                       }
        case (ABSORB):{
                       intensity *dif = s->getMaterial()->getDif();
                       float avg = (dif[0]+dif[1]+dif[2])*.33333;
                       Vector3D scaledPower (power.x*dif[0]/avg,
                                             power.y*dif[1]/avg,
                                             power.z*dif[2]/avg);
                       emmissionTarget->store(scaledPower, rPoint, direction);
                       break;}
        case (TRANSMIT):{
                           Vector3D newDirection;
                           Vector3D newPos;
                           s->refract(rPoint, direction, newPos, newDirection);
                           // TODO: find a better solution for the following
                           // move point a bit along new direction to avoid collision with the same object
                           newPos += (newDirection * 0.0001);
                           this->recievePhoton(newPos, power, newDirection, depth-1);

                        break;}
        case (SPEC_REFLECT):{
                           Vector3D reflected;
                           geovalue vnn = (direction * rNormal) * 2.0;
                           reflected = (direction - (rNormal * vnn)).normal();
                           // TODO: find a better solution for the following
                           // move point a bit along new direction to avoid collision with the same object
                           rPoint += (reflected * 0.0001);
                           this->recievePhoton(rPoint, power, reflected, depth-1);
                        break;}
    }
    return true;
  }
  return false;
}

/*!
    \fn Scene::createPhotonMap()
 */
void Scene::createPhotonMap() {
  const long PHOTON_COUNT = settings.getPhotonCount();
  double colorSum = 0;
  currentProgress = 0;
  emit maximumChanged(PHOTON_COUNT/100);
  for (int i=0; i < lights.size(); ++i ) {
    colorSum += lights[i]->getAverageColor();
  }
  if (photonMap != 0) {
    delete photonMap;
  }
  photonMap = new PhotonMap(PHOTON_COUNT);
  emmissionTarget = photonMap;

  for (int i=0; i < lights.size(); ++i ) {
    long currPhotonCount = (int)(PHOTON_COUNT * ( lights[i]->getAverageColor() / colorSum));
    connect(lights[i], SIGNAL(photonEmitted()), this, SLOT(progress()));
    lights[i]->emitPhotons(currPhotonCount);
    photonMap->scale_photon_power(1.0/currPhotonCount);
    disconnect(lights[i]);
  }

  photonMap->balance();
  photonMap->generateVertexArray(settings.getPhotonCount() / lights.size());
}


/*!
    \fn Scene::createCausticsMap()
 */
void Scene::createCausticsMap()
{
  const long PHOTON_COUNT = settings.getCausticsPhotonCount();
  double colorSum = 0;
  currentProgress = 0;
  emit maximumChanged(PHOTON_COUNT/100);
  for (int i=0; i < lights.size(); ++i ) {
    colorSum += lights[i]->getAverageColor();
  }
  if (causticsMap != 0) {
    delete causticsMap;
  }
  causticsMap = new PhotonMap(PHOTON_COUNT);
  emmissionTarget = causticsMap;

  for (int i=0; i < lights.size(); ++i ) {
    long currPhotonCount = (int)(PHOTON_COUNT * ( lights[i]->getAverageColor() / colorSum));
    connect(lights[i], SIGNAL(photonEmitted()), this, SLOT(progress()));
    lights[i]->emitCausticPhotons(currPhotonCount);
    causticsMap->scale_photon_power(1.0/currPhotonCount);
    disconnect(lights[i]);
  }

  causticsMap->balance();
  causticsMap->generateVertexArray(settings.getCausticsPhotonCount() / lights.size());
}


/*!
    \fn Scene::recieveCausticPhoton(position, power, direction)
 */
bool Scene::recieveCausticPhoton(const Vector3D& position, const Vector3D& power, const Vector3D& direction)
{
  geovalue z, zmin = 999.0;
  Vector3D lPoint, lNormal, rPoint, rNormal;
  int hitindex;
  hitindex = -1;

  for (int i = 0; i < objects.size(); ++i) {
    Geometry *s = (objects.at(i));
    if ( (s->getMaterial()->getReflectivity() > 0.0) || (s->getMaterial()->getAlpha() < 1.0 )) {
      z = s->cut(position, direction, lPoint, lNormal);
      if (( z > 0.0 ) && (z < zmin)) {
        zmin = z;
        hitindex = i;
        rPoint = lPoint;
        rNormal = lNormal;
      }
    }
  }

  if (hitindex >= 0) { // the photon hit an object
    Geometry *s = objects.at(hitindex);
    photonBehavior behav = ABSORB;
    while ( behav == ABSORB || behav == DIF_REFLECT) // because for caustics we want only 
      behav = s->getMaterial()->spinRoulette();      // reflection or transmission

    switch (behav) {
        case (ABSORB):
        case (DIF_REFLECT):{// for the caustics map photons will be stored immediatly in map
                       intensity *dif = s->getMaterial()->getDif();
                       float avg = (dif[0]+dif[1]+dif[2])*.33333;
                       Vector3D scaledPower (power.x*dif[0]/avg,
                                             power.y*dif[1]/avg,
                                             power.z*dif[2]/avg);
                       emmissionTarget->store(scaledPower, rPoint, direction);
                       break;}
        case (TRANSMIT):{
                           Vector3D newDirection;
                           Vector3D newPos;
                           s->refract(rPoint, direction, newPos, newDirection);
                           // TODO: find a better solution for the following
                           // move point a bit along new direction to avoid collision with the same object
                           newPos += (newDirection * 0.01);
                           // depth=0, because we dont want anymore bounces for caustics
                           this->recievePhoton(newPos, power, newDirection,0);

                        break;}
        case (SPEC_REFLECT):{
                           Vector3D reflected;
                           geovalue vnn = (direction * rNormal) * 2.0;
                           reflected = (direction - (rNormal * vnn)).normal();
                           // TODO: find a better solution for the following
                           // move point a bit along new direction to avoid collision with the same object
                           rPoint += (reflected * 0.01);
                           // depth=0, because we dont want anymore bounces for caustics
                           this->recievePhoton(rPoint, power, reflected,0);
                        break;}
    }
    return true;
  }
  return false;
}


void Scene::progress() {
  emit progressChanged((++currentProgress)/100);
}

void Scene::setProgress(int progress) {
  emit progressChanged(progress);
}

void Scene::abortRendering() {
  viewportCam.abortRendering();
}

void Scene::emitRenderingDone(const LayeredImage& image) {
  QString msg("Finished rendering scene.");
  emit report(msg);
  emit renderCombinedUpdated(image.getCombined(), image.getWidth(), image.getHeight());
  emit renderDirectUpdated(image.getDirect(), image.getWidth(), image.getHeight());
  emit renderIndirectUpdated(image.getIndirect(), image.getWidth(), image.getHeight());
  emit renderSpecularUpdated(image.getSpecular(), image.getWidth(), image.getHeight());
  emit renderCausticUpdated(image.getCaustic(), image.getWidth(), image.getHeight());
}

void Scene::emitRenderingMapDone(const LayeredImage& image) {
  QString msg("Finished rendering photon map.");
  emit report(msg);
  emit renderMapUpdated(image.getMap(), image.getWidth(), image.getHeight());
}

void Scene::updateRendering(int progress) {
  emit progressChanged(progress);
}

