#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QColor>
#include <QWidget>

class QScrollArea;
class QScrollBar;
class QLabel;
class QImage;

/**
 * The canvas used to display the rendered image.
 */
class RenderArea : public QWidget {
  Q_OBJECT

  public:
    RenderArea(QWidget *parent = 0);

    bool saveImage(const QString &fileName, const char *fileFormat);

  public slots:
    void upateImage(const QImage&, int w, int h);
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();

  private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    bool fit;
    double scaleFactor;

    QLabel *imageLabel;
    QScrollArea *scrollArea;
};

#endif


