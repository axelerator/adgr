#ifndef CAMERA_H
#define CAMERA_H

#include "values.h"
#include "vector3d.h"
#include "layeredimage.h"

#include <QThread>
#include <QImage>

class RenderArea;
class Scene;
class Geometry;
class Radiance;

/**
 * This is the camera which defines the actually rendered viewport.
 */
class Camera : public QThread {
  Q_OBJECT
  public:
    Camera(Scene *parent);
    Camera(Vector3D p, Vector3D t, Vector3D u, geovalue d);
    ~Camera();

    void drawGL();
    void update();
    void move(geovalue *delta);

    void render(int w, int h);
    void renderMap(int w, int h);

    void setPosition(geovalue x, geovalue y);
    Vector3D& getPosition();

  signals:
    void progressChanged(int);
    void done(const LayeredImage&);

  public slots:
    void abortRendering();

  protected:
    void run();

  private:
    bool raycast(Vector3D p, Vector3D ray, Radiance& rad, Geometry *g, bool approximate);

    void directIllumination(double radiance[3], Geometry *g, const Vector3D& position, 
                            const Vector3D& normal);

    void indirectIllumination(double radiance[3], Geometry *g, const Vector3D& position, 
                              const Vector3D& normal);

    void causticsIllumination(double radiance[3], const Vector3D& position, const Vector3D& normal);

    void specularIllumination(double radiance[3], Geometry *g, const Vector3D& ray, 
                              const Vector3D& position, const Vector3D& normal, bool approximate);

    Vector3D position;           ///< Position of the camera
    Vector3D target;             ///< "Look" direction
    Vector3D upvector;           ///< Defining the up direction

    Vector3D temp;               ///< ???
    Vector3D temp1;              ///< ???
    geovalue d;                  ///< ???

    geovalue projWidth;          ///< Width of the projection plane
    geovalue projHeight;         ///< Height of the projection plane

    Vector3D projOrigin;         ///< Origin (upper left corner) of the projection plane
    Vector3D u;                  ///< Current vector in x direction
    Vector3D v;                  ///< Current vector in y direction
    unsigned int resolution[2];  ///< Resolution of the projection

    Scene *scene;                ///< The scene to render

    LayeredImage layers;         ///< Output of rendering

    bool abort;                  ///< rendering will be aborted when true
    bool justMap;              ///< Just render the photon map
};

#endif
