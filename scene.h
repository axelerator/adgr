#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include <QImage>

#include "values.h"
#include "geometry.h"
#include "light.h"
#include "camera.h"
#include "drawable.h"
#include "photonmap.h"
#include "scenesettings.h"

class Material;
class LayeredImage;
class QProgressBar;

class Scene : public QObject,  public Drawable {
Q_OBJECT
  public:
    Scene();
    virtual ~Scene();
    void addLight(geovalue *pos, intensity *dif);
    void addSphere(geovalue *pos, geovalue rad, Material *mat);
    void addPlane(geovalue *pos, geovalue *normal, Material *mat);
    void addRectangle(geovalue *p1, geovalue *p2, geovalue *p3, geovalue *p4, Material *mat);
    virtual void drawGL();
    void clear();
    void moveCurrentObject(geovalue *delta);
    void render();
    void renderMap();
    void alterCam(float x, float y);
    void abortRendering();

    void createPhotonMap();
    bool recievePhoton(const Vector3D&, const Vector3D& power, const Vector3D& direction, unsigned int depth);
    PhotonMap* getPhotonMap() const {return photonMap;}

    void createCausticsMap();
    bool recieveCausticPhoton(const Vector3D& position, const Vector3D& power, const Vector3D& direction);
    PhotonMap* getCausticsMap() const { return causticsMap;}

    const SceneSettings& getSettings() { return settings; }

    QList<Geometry*> objects;
    QList<Light*> lights;
    QList<Camera> cameras;
    Camera viewportCam;
    int currentCam;

  signals:
      void maximumChanged (int);
      void progressChanged(int);
      void report(QString&);
      void renderCombinedUpdated(const QImage&, int w, int h);
      void renderDirectUpdated(const QImage&, int w, int h);
      void renderIndirectUpdated(const QImage&, int w, int h);
      void renderSpecularUpdated(const QImage&, int w, int h);
      void renderCausticUpdated(const QImage&, int w, int h);
      void renderMapUpdated(const QImage&, int w, int h);

  public slots:
      void progress();
      void setProgress(int);
      void updateRendering(int);
      void emitRenderingDone(const LayeredImage&);
      void emitRenderingMapDone(const LayeredImage&);

	
private:

    PhotonMap *photonMap;
    PhotonMap *causticsMap;
    PhotonMap *emmissionTarget;

    long currentProgress;
    SceneSettings settings;
};

#endif
