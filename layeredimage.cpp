//
// C++ Implementation: layeredimage
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <cmath>
#include <QColor>

#include "layeredimage.h"
#include "color.h"
#include "radiance.h"

LayeredImage::LayeredImage(): combinedView(512, 512, QImage::Format_ARGB32), 
                              directView(512, 512, QImage::Format_ARGB32), 
                              indirectView(512, 512, QImage::Format_ARGB32), 
                              specularView(512, 512, QImage::Format_ARGB32), 
                              causticView(512, 512, QImage::Format_ARGB32),
                              mapView(512, 512, QImage::Format_ARGB32) {

  reset(512, 512);

  qRegisterMetaType<LayeredImage>();
}

LayeredImage::~LayeredImage() {
}

void LayeredImage::setPixel(unsigned int x, unsigned int y, const Radiance& rad) {
  setPixel(COMBINED, x, y, rad.getCombined());
  setPixel(DIRECT, x, y, rad.getDirect());
  setPixel(INDIRECT, x, y, rad.getIndirect());
  setPixel(SPECULAR, x, y, rad.getSpecular());
  setPixel(CAUSTIC, x, y, rad.getCaustic());
}

void LayeredImage::setPixel(layer l, unsigned int x, unsigned int y, const Color& col) {
  QImage *targetView = 0;

  switch(l) {
    case COMBINED: targetView = &combinedView; break;
    case DIRECT:   targetView = &directView; break;
    case INDIRECT: targetView = &indirectView; break;
    case SPECULAR: targetView = &specularView; break;
    case CAUSTIC:  targetView = &causticView; break;
    case MAP:      targetView = &mapView; break;
  }

  if (targetView) {
    // Draw the pixel (some conversion to Qt's color format needed)
    targetView->setPixel(x, y, QColor::QColor(lround(col[0]*255.0),
                        lround(col[1]*255.0),lround(col[2]*255.0)).rgb());
  }
}

void LayeredImage::reset(int w, int h) {
  usedWidth = w > 512 ? 512 : w;
  usedHeight = h > 512 ? 512 : h;

//   combinedView.fill(0);
//   directView.fill(0);
//   indirectView.fill(0);
//   specularView.fill(0);
//   causticView.fill(0);
//   mapView.fill(0);
}
