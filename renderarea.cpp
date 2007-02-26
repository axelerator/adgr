#include <iostream>

#include <QtGui>

#include "renderarea.h"

/**
 * Create a default 256x256 render area.
 */
RenderArea::RenderArea(QWidget *parent): QWidget(parent), fit(false), scaleFactor(1.0) {
  imageLabel = new QLabel;
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);

  scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(imageLabel);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(scrollArea);
  setLayout(layout);
}

/**
 * Saves an image. Currently unused.
 */
bool RenderArea::saveImage(const QString &fileName, const char *fileFormat) {
     QImage visibleImage = imageLabel->pixmap()->toImage();
     if (visibleImage.save(fileName, fileFormat)) {
         return true;
     } else {
         return false;
     }
}

/**
 * Shows content of provided Image
 */
void RenderArea::upateImage(const QImage& newImage, int w, int h) {
  imageLabel->setPixmap(QPixmap::fromImage(newImage.copy(0, 0, w, h)));
  scaleFactor = 1.0;
  if (!fit) {
    imageLabel->adjustSize();
  }
}

void RenderArea::zoomIn() {
  scaleImage(1.25);
}

void RenderArea::zoomOut() {
  scaleImage(0.8);
}

void RenderArea::normalSize() {
  imageLabel->adjustSize();
  scaleFactor = 1.0;
}

void RenderArea::fitToWindow() {
  fit = !fit;
  scrollArea->setWidgetResizable(fit);
  if (!fit) {
    normalSize();
  }
}

void RenderArea::scaleImage(double factor) {
  if (imageLabel->pixmap()) {
    std::cout << "zooming..."<<std::endl;
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
  }
}

void RenderArea::adjustScrollBar(QScrollBar *scrollBar, double factor) {
  scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
}
