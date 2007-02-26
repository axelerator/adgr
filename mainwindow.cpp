#include <cstdlib>
#include <ctime>
#include <QtGui>
#include <QColor>
#include <QSplitter> 
#include <QTabWidget>

#include "mainwindow.h"
#include "vectorm.h"
#include "light.h"
#include "values.h"
#include "math.h"
#include "glwidget.h"
#include "material.h"
#include "settingswidget.h"

MainWindow::MainWindow() {
  QWidget *main = new QWidget;
  QWidget *top = new QWidget;
  QWidget *left = new QWidget;

  QTabWidget *tabs = new QTabWidget;
  tabs->setTabPosition(QTabWidget::South);

  QHBoxLayout *hboxTop = new QHBoxLayout;
  QVBoxLayout *vboxLeft = new QVBoxLayout;
  QVBoxLayout *vbox = new QVBoxLayout;

  oglScene = new GLWidget;
  oglMap = new GLWidget;

  vboxLeft->setMargin(5);
  QSplitter *leftSplit = new QSplitter(Qt::Vertical, this);
  leftSplit->addWidget(oglScene);
  leftSplit->addWidget(oglMap);
  vboxLeft->addWidget(leftSplit);
  left->setLayout(vboxLeft);

  combinedCanvas = new RenderArea;
  directCanvas = new RenderArea;
  indirectCanvas = new RenderArea;
  specularCanvas = new RenderArea;
  causticCanvas = new RenderArea;
  mapCanvas = new RenderArea;

  directSmall = new RenderArea;
  indirectSmall = new RenderArea;
  specularSmall = new RenderArea;
  causticSmall = new RenderArea;

  QWidget *gridCanvas = new QWidget;
  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->addWidget(directSmall, 0, 0);
  gridLayout->addWidget(indirectSmall, 0, 1);
  gridLayout->addWidget(specularSmall, 1, 0);
  gridLayout->addWidget(causticSmall, 1, 1);
  gridCanvas->setLayout(gridLayout);

  tabs->addTab(combinedCanvas, "Combined");
  tabs->addTab(gridCanvas, "Components");
  tabs->addTab(mapCanvas, "Map");
  tabs->addTab(directCanvas, "Direct");
  tabs->addTab(indirectCanvas, "Indirect");
  tabs->addTab(specularCanvas, "Specular");
  tabs->addTab(causticCanvas, "Caustic");

  settings = new SettingsWidget;
  settings->setFixedHeight(190);
  QSplitter *topSplitter = new QSplitter(this);
  hboxTop->setMargin(5);
  topSplitter->addWidget(left);
  topSplitter->addWidget(tabs);
  hboxTop->addWidget(topSplitter);

  top->setLayout(hboxTop);

  pbar = new QProgressBar;

  vbox->setMargin(5);
  vbox->addWidget(top);
  vbox->addWidget(settings);
  main->setLayout(vbox);

  setCentralWidget(main);

  createActions();
  createMenus();

  statusBar()->addPermanentWidget(pbar);
  setWindowTitle(tr("Photonmapmaster 3000"));
  setMinimumSize(160, 160);
  resize(830, 700);
}

void MainWindow::setStatus(QString& msg) {
  statusBar()->showMessage(msg);
}

void MainWindow::newScene() {
  if (&(myScene.getSettings()))
    disconnect(&(myScene.getSettings()));
// TODO: Make the scene selectable from the GUI
// bigScene();
// simpleScene();
  cornellBox();
  settings->loadSettings(myScene.getSettings());
  disconnect(pbar);

  connect(&myScene, SIGNAL(maximumChanged(int)), pbar, SLOT(setMaximum(int)));
  connect(&myScene, SIGNAL(progressChanged(int)), pbar, SLOT(setValue(int)));

  const SceneSettings *sceneSettings = &(myScene.getSettings());
  connect(settings, SIGNAL(widthChanged(int)), sceneSettings, SLOT(setWidth(int)));
  connect(settings, SIGNAL(heightChanged(int)), sceneSettings, SLOT(setHeight(int)));
  connect(settings, SIGNAL(lightSamplingChanged(int)), sceneSettings, SLOT(setAreaLightResolution(int)));
  connect(settings, SIGNAL(photonCountChanged(long)), sceneSettings, SLOT(setPhotonCount(long)));
  connect(settings, SIGNAL(causticPhotonCountChanged(long)), sceneSettings, SLOT(setCausticsPhotonCount(long)));
  connect(settings, SIGNAL(raySamplingChanged(int)), sceneSettings, SLOT(setSampleResolution(int)));
  connect(settings, SIGNAL(irradCountChanged(int)), sceneSettings, SLOT(setIrradCount(int)));
  connect(settings, SIGNAL(irradRadiusChanged(float)), sceneSettings, SLOT(setIrradRadius(float)));
  connect(settings, SIGNAL(causticsIrradCountChanged(int)), sceneSettings, SLOT(setCausticsIrradCount(int)));
  connect(settings, SIGNAL(causticsIrradRadiusChanged(float)), sceneSettings, SLOT(setCausticsIrradRadius(float)));
}

void MainWindow::createPhotonMap() {
  statusBar()->showMessage(QString("Generating photon map..."));
  myScene.createPhotonMap();
  oglMap->setDrawable(myScene.getPhotonMap());
  statusBar()->showMessage(QString("Generating photon map finished"));
  oglMap->update();
}


void MainWindow::createCausticsMap() {
  statusBar()->showMessage(QString("Generating caustics photon map..."));
  myScene.createCausticsMap();
  oglMap->setDrawable(myScene.getCausticsMap());
  statusBar()->showMessage(QString("Generating caustics photon map finished"));
}

/*!
    \fn MainWindow::cornellBox()
 */
void MainWindow::cornellBox() {
  myScene.clear();
  oglScene->setDrawable(&myScene);

  Material *red = new Material(1.0, 0.0, 0.0, 0.0);
  Material *blue = new Material(0.0, 0.0, 1.0, 0.0);
  Material *white = new Material(1.0, 1.0, 1.0, 0.0);
  Material *whiteBall = new Material(1.0, 1.0, 1.0, 0.4);
  Material *reflect = new Material(1.0, 1.0, 1.0, 1.0);
  Material *glass = new Material(1.0, 1.0, 1.0, 1.0);
  glass->setAlpha(0.1);
  glass->setRefractionIndex(1.45);

  intensity whiteCol[3] = {.7, .7, .7};

  geovalue lightpos3[3] = {0.0, 0.99, 2.0};

  myScene.addLight(lightpos3, whiteCol);

  geovalue spos1[3] = {-0.4, -0.59, 2.6};
  geovalue spos2[3] = {-0.5, -0.79, 1.4};
  geovalue spos3[3] = {0.34,  -0.59, 1.9};

  myScene.addSphere(spos1, 0.4, reflect);
  myScene.addSphere(spos2, 0.2, whiteBall);
  myScene.addSphere(spos3, 0.4, glass);

  geovalue bottom_p1[3] = { 1.0, -1.0, 3.0};
  geovalue bottom_p2[3] = { 1.0, -1.0, 1.0};
  geovalue bottom_p3[3] = {-1.0, -1.0, 1.0};
  geovalue bottom_p4[3] = {-1.0, -1.0, 3.0};
  myScene.addRectangle(bottom_p1, bottom_p2, bottom_p3, bottom_p4, white);

  geovalue left_p1[3] = {-1.0, -1.0, 1.0};
  geovalue left_p2[3] = {-1.0,  1.0, 1.0};
  geovalue left_p3[3] = {-1.0,  1.0, 3.0};
  geovalue left_p4[3] = {-1.0, -1.0, 3.0};
  myScene.addRectangle(left_p1, left_p2, left_p3, left_p4, red);

  geovalue right_p1[3] = {1.0, -1.0, 3.0};
  geovalue right_p2[3] = {1.0,  1.0, 3.0};
  geovalue right_p3[3] = {1.0,  1.0, 1.0};
  geovalue right_p4[3] = {1.0, -1.0, 1.0};
  myScene.addRectangle(right_p1, right_p2, right_p3, right_p4, blue);

  geovalue top_p1[3] = {-1.0, 1.0, 3.0};
  geovalue top_p2[3] = {-1.0, 1.0, 1.0};
  geovalue top_p3[3] = { 1.0, 1.0, 1.0};
  geovalue top_p4[3] = { 1.0, 1.0, 3.0};
  myScene.addRectangle(top_p1, top_p2, top_p3, top_p4, white);

  geovalue back_p1[3] = {-1.0,  1.0, 3.0};
  geovalue back_p2[3] = { 1.0,  1.0, 3.0};
  geovalue back_p3[3] = { 1.0, -1.0, 3.0};
  geovalue back_p4[3] = {-1.0, -1.0, 3.0};
  myScene.addRectangle(back_p1, back_p2, back_p3, back_p4, white);
}

void MainWindow::simpleScene() {
  myScene.clear();
  oglScene->setDrawable(&myScene);

  Material *red = new Material(1.0, 0.0, 0.0, 0.0);
  Material *blue = new Material(0.0, 0.0, 1.0, 0.0);
  Material *white = new Material(1.0, 1.0, 1.0, 0.0);
  Material *glass = new Material(1.0, 1.0, 1.0, 0.0);
  glass->setAlpha(0.0);
  glass->setRefractionIndex(1.45);

  intensity whiteCol[3] = {.6, .6, .6};

  geovalue lightpos3[3] = {0.0, 0.6, 2.0};

  myScene.addLight(lightpos3, whiteCol);

  geovalue bottom_plane_normal[3] = { 0.0, 1.0, 0.0 };
  geovalue bottom_plane_origin[3] = { 0.0, -1.0, 0.0 };

  geovalue left_plane_normal[3] = { 1.0, 0.0, 0.0 };
  geovalue left_plane_origin[3] = { -1.0, 0.0, 0.0 };

  geovalue right_plane_normal[3] = { -1.0, 0.0, 0.0 };
  geovalue right_plane_origin[3] = { 1.0, 0.0, 0.0 };

  geovalue top_plane_normal[3] = { 0.0, -1.0, 0.0 };
  geovalue top_plane_origin[3] = { 0.0, 1.0, 0.0 };

  geovalue back_plane_normal[3] = { 0.0, 0.0, -1.0 };
  geovalue back_plane_origin[3] = { 0.0, 0.0, 3.0 };

  geovalue front_plane_normal[3] = { 0.0, 0.0, 1.0 };
  geovalue front_plane_origin[3] = { 0.0, 0.0, -3.0 };

  myScene.addPlane(left_plane_origin, left_plane_normal, red);
  myScene.addPlane(right_plane_origin, right_plane_normal, blue);
  myScene.addPlane(bottom_plane_origin, bottom_plane_normal, white);
  myScene.addPlane(top_plane_origin, top_plane_normal, white);
  myScene.addPlane(back_plane_origin, back_plane_normal, white);
  myScene.addPlane(front_plane_origin, front_plane_normal, white);
}

void MainWindow::save() {
     QAction *action = qobject_cast<QAction *>(sender());
     QByteArray fileFormat = action->data().toByteArray();
     saveFile(fileFormat);


}

bool MainWindow::saveFile(const QByteArray &fileFormat) {

     QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                initialPath,
                                tr("%1 Files (*.%2);;All Files (*)")
                                .arg(QString(fileFormat.toUpper()))
                                .arg(QString(fileFormat)));
     if (fileName.isEmpty()) {
         return false;
     } else {
         return combinedCanvas->saveImage(fileName, fileFormat);
     }
}

void MainWindow::abortRender() {
  myScene.abortRendering();
}

void MainWindow::showPhotons() {
  oglMap->setDrawable(myScene.getPhotonMap());
}

void MainWindow::showCausticPhotons() {
  oglMap->setDrawable(myScene.getCausticsMap());
}

void MainWindow::render() {
  if (!myScene.getPhotonMap())
    this->createPhotonMap();
  if (!myScene.getCausticsMap())
    this->createCausticsMap();

  statusBar()->showMessage(QString("Rendering scene..."));

  connect (&myScene, SIGNAL(renderCombinedUpdated(const QImage&, int, int)), 
           combinedCanvas, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderDirectUpdated(const QImage&, int, int)), 
           directCanvas, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderIndirectUpdated(const QImage&, int, int)), 
           indirectCanvas, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderSpecularUpdated(const QImage&, int, int)), 
           specularCanvas, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderCausticUpdated(const QImage&, int, int)), 
           causticCanvas, SLOT(upateImage(const QImage&, int, int)));

  connect (&myScene, SIGNAL(renderDirectUpdated(const QImage&, int, int)), 
           directSmall, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderIndirectUpdated(const QImage&, int, int)), 
           indirectSmall, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderSpecularUpdated(const QImage&, int, int)), 
           specularSmall, SLOT(upateImage(const QImage&, int, int)));
  connect (&myScene, SIGNAL(renderCausticUpdated(const QImage&, int, int)), 
           causticSmall, SLOT(upateImage(const QImage&, int, int)));

  myScene.render();
}

void MainWindow::renderMap() {
  if (!myScene.getPhotonMap())
    this->createPhotonMap();

  statusBar()->showMessage(QString("Rendering photon map..."));

  connect (&myScene, SIGNAL(renderMapUpdated(const QImage&, int, int)), 
           mapCanvas, SLOT(upateImage(const QImage&, int, int)));

  myScene.renderMap();
}

void MainWindow::createActions() {
  // General actions
  newAct = new QAction(tr("&New scene"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new scene"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newScene()));

  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the image to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  // Render actions
  createPMAct = new QAction(tr("Create global &photon map"), this);
  createPMAct->setShortcut(tr("Ctrl+P"));
  createPMAct->setStatusTip(tr("Emit and store photons"));
  connect(createPMAct, SIGNAL(triggered()), this, SLOT(createPhotonMap()));    

  createCPMAct = new QAction(tr("Create &caustics photon map"), this);
  createCPMAct->setShortcut(tr("Ctrl+C"));
  createCPMAct->setStatusTip(tr("Emit and store photons in direction of specular objects"));
  connect(createCPMAct, SIGNAL(triggered()), this, SLOT(createCausticsMap()));    

  renderAct = new QAction(tr("&Render scene"), this);
  renderAct->setShortcut(tr("Ctrl+R"));
  renderAct->setStatusTip(tr("Render the image"));
  connect(renderAct, SIGNAL(triggered()), this, SLOT(render()));

  renderMapAct = new QAction(tr("Render &global map"), this);
  renderMapAct->setShortcut(tr("Ctrl+G"));
  renderMapAct->setStatusTip(tr("Render the global photon map"));
  connect(renderMapAct, SIGNAL(triggered()), this, SLOT(renderMap()));

  abortRenderAct = new QAction(tr("&Abort rendering"), this);
  abortRenderAct->setShortcut(tr("Ctrl+A"));
  abortRenderAct->setStatusTip(tr("Cancel the rendering process"));
  connect(abortRenderAct, SIGNAL(triggered()), this, SLOT(abortRender()));  

  // Photon map actions
  showPhotonMapAct = new QAction(tr("Show &global map"), this);
  showPhotonMapAct->setShortcut(tr("Ctrl+M"));
  showPhotonMapAct->setStatusTip(tr("Visualize the location of the photons of the map for indirect illumination"));
  connect(showPhotonMapAct, SIGNAL(triggered()), this, SLOT(showPhotons()));

  showCausticsMapAct = new QAction(tr("Show &caustic map"), this);
  showCausticsMapAct->setShortcut(tr("Ctrl+N"));
  showCausticsMapAct->setStatusTip(tr("Visualize the location of the photons of the map for caustics"));
  connect(showCausticsMapAct, SIGNAL(triggered()), this, SLOT(showCausticPhotons()));

  // View actions
  zoomInAct = new QAction(tr("Zoom &In"), this);
  zoomInAct->setShortcut(tr("Ctrl++"));
  connect(zoomInAct, SIGNAL(triggered()), combinedCanvas, SLOT(zoomIn()));
  connect(zoomInAct, SIGNAL(triggered()), directCanvas, SLOT(zoomIn()));
  connect(zoomInAct, SIGNAL(triggered()), indirectCanvas, SLOT(zoomIn()));
  connect(zoomInAct, SIGNAL(triggered()), specularCanvas, SLOT(zoomIn()));
  connect(zoomInAct, SIGNAL(triggered()), causticCanvas, SLOT(zoomIn()));
  connect(zoomInAct, SIGNAL(triggered()), mapCanvas, SLOT(zoomIn()));

  zoomOutAct = new QAction(tr("Zoom &Out"), this);
  zoomOutAct->setShortcut(tr("Ctrl+-"));
  connect(zoomOutAct, SIGNAL(triggered()), combinedCanvas, SLOT(zoomOut()));
  connect(zoomOutAct, SIGNAL(triggered()), directCanvas, SLOT(zoomOut()));
  connect(zoomOutAct, SIGNAL(triggered()), indirectCanvas, SLOT(zoomOut()));
  connect(zoomOutAct, SIGNAL(triggered()), specularCanvas, SLOT(zoomOut()));
  connect(zoomOutAct, SIGNAL(triggered()), causticCanvas, SLOT(zoomOut()));
  connect(zoomOutAct, SIGNAL(triggered()), mapCanvas, SLOT(zoomOut()));

  normalSizeAct = new QAction(tr("&Normal Size"), this);
  normalSizeAct->setShortcut(tr("Ctrl+S"));
  connect(normalSizeAct, SIGNAL(triggered()), combinedCanvas, SLOT(normalSize()));
  connect(normalSizeAct, SIGNAL(triggered()), directCanvas, SLOT(normalSize()));
  connect(normalSizeAct, SIGNAL(triggered()), indirectCanvas, SLOT(normalSize()));
  connect(normalSizeAct, SIGNAL(triggered()), specularCanvas, SLOT(normalSize()));
  connect(normalSizeAct, SIGNAL(triggered()), causticCanvas, SLOT(normalSize()));
  connect(normalSizeAct, SIGNAL(triggered()), mapCanvas, SLOT(normalSize()));

  fitToWindowAct = new QAction(tr("&Fit to Window"), this);
  fitToWindowAct->setCheckable(true);
  fitToWindowAct->setShortcut(tr("Ctrl+F"));
  connect(fitToWindowAct, SIGNAL(triggered()), combinedCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), directCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), indirectCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), specularCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), causticCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), mapCanvas, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), directSmall, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), indirectSmall, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), specularSmall, SLOT(fitToWindow()));
  connect(fitToWindowAct, SIGNAL(triggered()), causticSmall, SLOT(fitToWindow()));

  // Help actions
  helpAct = new QAction(tr("&Handbook"), this);
  helpAct->setShortcut(tr("Ctrl+H"));
  helpAct->setStatusTip(tr("Show online help"));
// TODO: Implement some online help
//  connect(renderAct, SIGNAL(triggered()), this, SLOT(render()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setShortcut(tr("Ctrl+A"));
  aboutAct->setStatusTip(tr("Show program information"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&Scene"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(saveAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  sceneMenu = menuBar()->addMenu(tr("&Render"));
  sceneMenu->addAction(renderAct);
  sceneMenu->addAction(renderMapAct);
  sceneMenu->addAction(abortRenderAct);

  mapMenu = menuBar()->addMenu(tr("&Maps"));
  mapMenu->addAction(createPMAct);
  mapMenu->addAction(createCPMAct);
  mapMenu->addAction(showPhotonMapAct);
  mapMenu->addAction(showCausticsMapAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);
  viewMenu->addAction(normalSizeAct);
  viewMenu->addAction(fitToWindowAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpAct);
  helpMenu->addAction(aboutAct);
}

/*!
    \fn MainWindow::bigScene()
 */
void MainWindow::bigScene() {
  srand ( time(NULL) );

  myScene.clear();
  oglScene->setDrawable(&myScene);
  Material *mats[10];
  mats[0] = new Material(1.0, 0.0, 0.0, 0.0);
  mats[1] = new Material(0.0, 0.0, 1.0, 0.0);
  mats[2] = new Material(0.0, 1.0, 0.0, 0.0);
  mats[3] = new Material(1.0, 1.0, 1.0, 0.0);
  mats[4] = new Material(1.0, 1.0, 1.0, 1.0);
  mats[5] = new Material(1.0, 1.0, 1.0, 0.0);
  mats[5]->setAlpha(0.2);
  mats[5]->setRefractionIndex(1.5);
  
  intensity whiteCol[3] = {.33, .33, .33};
   
  geovalue lightpos1[3] = {-7.0, 3.0, 0.0};
  geovalue lightpos2[3] = {5.0, 3.0, 10.0};
  geovalue lightpos3[3] = {0.0, 5.6, 6.0};
  
  myScene.addLight(lightpos1,whiteCol);
  myScene.addLight(lightpos2,whiteCol);
  myScene.addLight(lightpos3, whiteCol);
  for (int i=0; i<150; ++i) {
      geovalue randomPos[3];
      float radius = 1.5 * (rand() / (RAND_MAX + 1.0));
      randomPos[0] = -15.0 +  (30.0 * (rand() / (RAND_MAX + 1.0)));
      randomPos[1] = -1.0 + radius ;
      randomPos[2] = -1.0 + (30.0 * (rand() / (RAND_MAX + 1.0)));
      int matnum = lround (6.0 * (rand() / (RAND_MAX + 1.0)));
      myScene.addSphere(randomPos, radius, mats[matnum]);
  }

  geovalue bottom_plane_normal[3] = { 0.0, 1.0, 0.0 };
  geovalue bottom_plane_origin[3] = { 0.0, -1.0, 0.0 };
  myScene.addPlane(bottom_plane_origin, bottom_plane_normal, mats[3]);
}

void MainWindow::about() {
     QMessageBox::about(this, tr("About Photonmapmaster 3000"),
             tr("<p>The <b>Photonmapmaster 3000</b> is a sample implementation of "
                "the famous photon mapping approach to the computer-based synthesis of "
                "realistic images by Henrik Wann Jensen.</p>"
                "<p>Copyright &copy; 2007 Axel Tetzlaff & Timo H&uuml;bel</p>"
                "<p>Version: <b>0.9.1</b> - 25.01.2007</p>"
                "<p>Contact: axel.tetzlaff@gmx.de  /  t.h@gmx.com</p>"
                "<p>Source code is avaliable upon request under the terms of the GNU Public License.</p>"));
}
