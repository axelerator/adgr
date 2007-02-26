#ifndef LIGHT_H
#define LIGHT_H

#include "values.h"
#include "vector3d.h"
#include <QObject>

class Scene;

class Light : public QObject {
  Q_OBJECT
  public:
    Light(const Vector3D& p, intensity *d, Scene* scene);
    virtual ~Light();
    virtual void drawGL();
    virtual long emitPhotons(long photonCount) = 0;
    virtual long emitCausticPhotons(long photonCount) = 0;
    double getAverageColor();
    virtual int getSampleCount() = 0;
    virtual const Vector3D& getSamplePosition(int i) = 0;
    virtual void setSampleCount(int) {};

    intensity *getDif()	{ return dif;	}

    Vector3D getPosition() const { return position; }

  signals:
    void photonEmitted();

  protected:
    Vector3D position;
    intensity dif[3];
    Scene *scene;
};

#endif
