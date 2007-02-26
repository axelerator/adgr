#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include "renderarea.h"
#include "sphere.h"
#include "scene.h"


class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QTextEdit;
class GLWidget;
class QProgressBar;
class SettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


protected:

private slots:
    void newScene();
    void save();
    void render();
    void renderMap();
    void abortRender();
    void createPhotonMap();
    void createCausticsMap();
    void setStatus(QString& msg);
    void showPhotons();
    void showCausticPhotons();
    void about();

private:
    void reflect(geovalue *ray, geovalue *point, geovalue *normal, geovalue *result, int depth);
    bool saveFile(const QByteArray &fileFormat);

    void createActions();
    void createMenus();
    void bigScene();
    void cornellBox();
    void simpleScene();

    QMenu *fileMenu;
    QMenu *sceneMenu;
    QMenu *mapMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;

    QAction *newAct;
    QAction *showPhotonMapAct;
    QAction *showCausticsMapAct;
    QAction *saveAct;
    QAction *renderAct;
    QAction *renderMapAct;
    QAction *abortRenderAct;
    QAction *createPMAct;
    QAction *createCPMAct;
    QAction *helpAct;
    QAction *aboutAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

    SettingsWidget *settings;
    GLWidget *oglScene;
    GLWidget *oglMap;
    QAction *exitAct;
    QTextEdit *text;
    QProgressBar *pbar;

    RenderArea *combinedCanvas;
    RenderArea *directCanvas;
    RenderArea *indirectCanvas;
    RenderArea *specularCanvas;
    RenderArea *causticCanvas;
    RenderArea *mapCanvas;

    RenderArea *directSmall;
    RenderArea *indirectSmall;
    RenderArea *specularSmall;
    RenderArea *causticSmall;

    Scene myScene;

};

#endif
