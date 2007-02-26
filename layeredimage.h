//
// C++ Interface: layeredimage
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LAYEREDIMAGE_H
#define LAYEREDIMAGE_H

#include <QImage>
#include <QMetaType>

class Color;
class Radiance;

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class LayeredImage{
  public:
    LayeredImage();

    ~LayeredImage();

    enum layer {COMBINED, DIRECT, INDIRECT, SPECULAR, CAUSTIC, MAP};

    void setPixel(layer l, unsigned int x, unsigned int y, const Color& col);
    void setPixel(unsigned int x, unsigned int y, const Radiance& rad);

    const QImage& getCombined() const { return combinedView; }
    const QImage& getDirect() const { return directView; }
    const QImage& getIndirect() const { return indirectView; }
    const QImage& getSpecular() const { return specularView; }
    const QImage& getCaustic() const { return causticView; }
    const QImage& getMap() const { return mapView; }

    void reset(int w, int h);

    int getWidth() const { return usedWidth; }
    int getHeight() const { return usedHeight; }

  private:
    int usedWidth;
    int usedHeight;

    QImage combinedView;         ///< The complete rendering
    QImage directView;           ///< Just direct illumination
    QImage indirectView;         ///< Just indirect illumination
    QImage specularView;         ///< Just specular illumination
    QImage causticView;          ///< Just caustics
    QImage mapView;              ///< The rendered photon map
};

Q_DECLARE_METATYPE(LayeredImage);

#endif
