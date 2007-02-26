//
// C++ Implementation: scenesettings
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scenesettings.h"

SceneSettings::SceneSettings():
  photonCount(50000),
  causticsPhotonCount(10000),
  width(256),
  height(256),
  areaLightResolution(2),
  sampleResolution(10),
  irradCount(75),
  irradRadius(0.1),
  causticsIrradCount(100),
  causticsIrradRadius(0.1)
{
}

SceneSettings::~SceneSettings() {
}

void SceneSettings::setPhotonCount(long get) {
  photonCount = get;
}

void SceneSettings::setCausticsPhotonCount(long get) {
  causticsPhotonCount = get;
}

void SceneSettings::setAreaLightResolution(int get) {
  areaLightResolution = get;
}

void SceneSettings::setSampleResolution(int get) {
  sampleResolution = get;
}

void SceneSettings::setHeight(int get) {
  height = get;
}

void SceneSettings::setWidth(int get) {
  width = get;
}

void SceneSettings::setIrradCount(int get) {
  irradCount = get;
}

void SceneSettings::setIrradRadius(float get) {
  irradRadius = get;
}

void SceneSettings::setCausticsIrradCount(int get) {
  causticsIrradCount = get;
}

void SceneSettings::setCausticsIrradRadius(float get) {
  causticsIrradRadius = get;
}

QString SceneSettings::toString() const {
  return QString( "light sampling" + QString::number(areaLightResolution)  + "\nwidth:" + QString::number(width) + "  height:" + QString::number(height) +"\n" +
  "global photons:" + QString::number(photonCount) + "  caustic photons:" + QString::number(causticsPhotonCount) +"\n" );
}
