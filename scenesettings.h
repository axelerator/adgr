//
// C++ Interface: scenesettings
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include <QObject>

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SceneSettings : public QObject{
Q_OBJECT
public:
    SceneSettings();
    ~SceneSettings();

    int getAreaLightResolution() const { return areaLightResolution; }
    long getCausticsPhotonCount() const { return causticsPhotonCount; }
    long getPhotonCount() const { return photonCount; }
    int getHeight() const { return height;}
    int getWidth() const { return width; }
    int getSampleResolution() const { return sampleResolution; }
    int getIrradCount() const { return irradCount; }
    float getIrradRadius() const { return irradRadius; }
    int getCausticsIrradCount() const { return causticsIrradCount; }
    float getCausticsIrradRadius() const { return causticsIrradRadius; }

    QString toString() const;

public slots:
    void setAreaLightResolution(int get);
    void setCausticsPhotonCount(long get);
    void setPhotonCount(long get);
    void setHeight(int get);
    void setWidth(int get);
    void setSampleResolution(int get);
    void setIrradCount(int get);
    void setIrradRadius(float get);
    void setCausticsIrradCount(int get);
    void setCausticsIrradRadius(float get);

private:
  long photonCount;
  int irradCount;
  float irradRadius;

  long causticsPhotonCount;
  int causticsIrradCount;
  float causticsIrradRadius;

  int width;
  int height;
  int areaLightResolution;
  int sampleResolution;
};

#endif
