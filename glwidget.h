#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

#include "values.h"
#include "drawable.h"
#include "vector3d.h"

class GLWidget : public QGLWidget {
    Q_OBJECT

  public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    int xRotation() const {
      return xRot;
    }
    int yRotation() const {
      return yRot;
    }
    int zRotation() const {
      return zRot;
    }
    void setDrawable(Drawable *d);

  public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void moveObject();

  protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


  private:
    void drawHelper();
    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor clear;
    Drawable *drawable;
    float angle;
    geovalue move[3];
    QTimer movementTimer;
    float lastCamPos[2];
    
    Vector3D lookAt;
    Vector3D pos;
    Vector3D up;
    float yAng;
    float xAng;
};

#endif
