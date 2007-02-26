//
// C++ Implementation: settingswidget
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settingswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>
#include <QSlider>
#include <QLineEdit>
#include "scenesettings.h"

SettingsWidget::SettingsWidget(QWidget *parent)
 : QGroupBox(parent)
{
  createImageResGroupBox();
  createGlobalMapGroupBox();
  createCausticMapGroupBox();
  createMiscGroupBox();

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(imageResBox);
  layout->addWidget(globalMapBox);
  layout->addWidget(causticMapBox);
  layout->addWidget(miscBox);
  setLayout(layout);
  setTitle("Settings for rendering");
}

QWidget *SettingsWidget::labelWidget(QWidget *widget, const char *label) {
  QWidget *holder = new QWidget(this);
  QHBoxLayout *hbl = new QHBoxLayout(this);
  hbl->addWidget(new QLabel(label,this));
  hbl->addWidget(widget);
  holder->setLayout(hbl);
  return holder;
}
void SettingsWidget::createImageResGroupBox()
{
  imageResBox = new QGroupBox(tr("Image Resolution"));
  QGridLayout *layout = new QGridLayout;
  QValidator *intValid = new QIntValidator(1,3000,this);

  imageResLabels[0] = new QLabel(tr("Width:"));
  imageResEdits[0] = new QLineEdit("256");
  imageResEdits[0]->setMaximumWidth(50);
  imageResEdits[0]->setValidator(intValid);
  layout->addWidget(imageResLabels[0], 1, 0);
  layout->addWidget(imageResEdits[0], 1, 1);

  imageResLabels[1] = new QLabel(tr("Height:"));
  imageResEdits[1] = new QLineEdit("256");
  imageResEdits[1]->setMaximumWidth(50);
  imageResEdits[1]->setValidator(intValid);
  layout->addWidget(imageResLabels[1], 2, 0);
  layout->addWidget(imageResEdits[1], 2, 1);
  
  connect(imageResEdits[0], SIGNAL(textChanged ( const QString &  )), this, SLOT(updateWidth(const QString & )));
  connect(imageResEdits[1], SIGNAL(textChanged (const  QString &  )), this, SLOT(updateHeight(const QString & )));
  imageResBox->setMaximumWidth(160);
  imageResBox->setLayout(layout);

}

void SettingsWidget::createGlobalMapGroupBox() {
  globalMapBox = new QGroupBox(tr("Global photon map"));

  globalPhotonsNumLabel[0] = new QLabel(tr("Photons:"));
  globalPhotonsNumLabel[1] = new QLabel(tr("50.000"));
  globalPhotonsNumSlider = new QSlider(Qt::Horizontal);

  globalIrradNumLabel[0] = new QLabel(tr("Irradiance estimate:"));
  globalIrradNumLabel[1] = new QLabel(tr("75"));
  globalIrradNumSlider = new QSlider(Qt::Horizontal);

  globalIrradRadLabel[0] = new QLabel(tr("Maximum radius:"));
  globalIrradRadLabel[1] = new QLabel(tr("0,1"));
  globalIrradRadSlider = new QSlider(Qt::Horizontal);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(globalPhotonsNumLabel[0], 0, 0);
  layout->addWidget(globalPhotonsNumLabel[1], 0, 1);
  layout->addWidget(globalPhotonsNumSlider,   1, 0, 1, 2);

  layout->addWidget(globalIrradNumLabel[0], 2, 0);
  layout->addWidget(globalIrradNumLabel[1], 2, 1);
  layout->addWidget(globalIrradNumSlider,   3, 0, 1, 2);

  layout->addWidget(globalIrradRadLabel[0], 4, 0);
  layout->addWidget(globalIrradRadLabel[1], 4, 1);
  layout->addWidget(globalIrradRadSlider,   5, 0, 1, 2);

  globalPhotonsNumSlider->setMaximum(500);
  globalPhotonsNumSlider->setMinimum(1);
  globalPhotonsNumSlider->setTickInterval(10);
  globalPhotonsNumSlider->setValue(50);

  globalIrradNumSlider->setMaximum(500);
  globalIrradNumSlider->setMinimum(1);
  globalIrradNumSlider->setTickInterval(1);
  globalIrradNumSlider->setValue(75);

  globalIrradRadSlider->setMaximum(100);
  globalIrradRadSlider->setMinimum(1);
  globalIrradRadSlider->setTickInterval(1);
  globalIrradRadSlider->setValue(10);

  connect(globalPhotonsNumSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePhotonDisplay(int)));
  connect(globalIrradNumSlider, SIGNAL(valueChanged(int)), this, SLOT(updateIrradCount(int)));
  connect(globalIrradRadSlider, SIGNAL(valueChanged(int)), this, SLOT(updateIrradRadius(int)));

  globalMapBox->setLayout(layout);
  globalMapBox->setMinimumWidth(210);
}

void SettingsWidget::createCausticMapGroupBox() {
  causticMapBox = new QGroupBox(tr("Caustics photon map"));

  causticPhotonsNumLabel[0] = new QLabel(tr("Photons:"));
  causticPhotonsNumLabel[1] = new QLabel(tr("10.000"));
  causticPhotonsNumSlider = new QSlider(Qt::Horizontal);

  causticIrradNumLabel[0] = new QLabel(tr("Irradiance estimate:"));
  causticIrradNumLabel[1] = new QLabel(tr("100"));
  causticIrradNumSlider = new QSlider(Qt::Horizontal);

  causticIrradRadLabel[0] = new QLabel(tr("Maximum radius:"));
  causticIrradRadLabel[1] = new QLabel(tr("0,1"));
  causticIrradRadSlider = new QSlider(Qt::Horizontal);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(causticPhotonsNumLabel[0], 0, 0);
  layout->addWidget(causticPhotonsNumLabel[1], 0, 1);
  layout->addWidget(causticPhotonsNumSlider,   1, 0, 1, 2);

  layout->addWidget(causticIrradNumLabel[0], 2, 0);
  layout->addWidget(causticIrradNumLabel[1], 2, 1);
  layout->addWidget(causticIrradNumSlider,   3, 0, 1, 2);

  layout->addWidget(causticIrradRadLabel[0], 4, 0);
  layout->addWidget(causticIrradRadLabel[1], 4, 1);
  layout->addWidget(causticIrradRadSlider,   5, 0, 1, 2);

  causticPhotonsNumSlider->setMaximum(50);
  causticPhotonsNumSlider->setMinimum(1);
  causticPhotonsNumSlider->setTickInterval(10);
  causticPhotonsNumSlider->setValue(10);

  causticIrradNumSlider->setMaximum(500);
  causticIrradNumSlider->setMinimum(1);
  causticIrradNumSlider->setTickInterval(1);
  causticIrradNumSlider->setValue(100);

  causticIrradRadSlider->setMaximum(100);
  causticIrradRadSlider->setMinimum(1);
  causticIrradRadSlider->setTickInterval(1);
  causticIrradRadSlider->setValue(10);

  connect(causticPhotonsNumSlider, SIGNAL(valueChanged(int)), this, SLOT(updateCausticPhotonDisplay(int)));
  connect(causticIrradNumSlider, SIGNAL(valueChanged(int)), this, SLOT(updateCausticsIrradCount(int)));
  connect(causticIrradRadSlider, SIGNAL(valueChanged(int)), this, SLOT(updateCausticsIrradRadius(int)));

  causticMapBox->setLayout(layout);
  causticMapBox->setMinimumWidth(210);
}

void SettingsWidget::createMiscGroupBox()
{
  miscBox = new QGroupBox(tr("Miscellaneous"));
  areaLightSampleLabel[0] = new QLabel(tr("Area light resolution:"));
  areaLightSampleLabel[1] = new QLabel(tr("2"));
  areaLightSampleSlider = new QSlider(Qt::Horizontal);
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(areaLightSampleLabel[0], 0, 0);
  layout->addWidget(areaLightSampleLabel[1], 0, 1);
  layout->addWidget(areaLightSampleSlider, 1, 0,1,2);

  raySampleLabel[0] = new QLabel(tr("Ray samples:"));
  raySampleLabel[1] = new QLabel(tr("10"));
  raySampleSlider = new QSlider(Qt::Horizontal);
  layout->addWidget(raySampleLabel[0], 2, 0);
  layout->addWidget(raySampleLabel[1], 2, 1);
  layout->addWidget(raySampleSlider, 3, 0, 1, 2);
  
  areaLightSampleSlider->setMaximum(50);
  areaLightSampleSlider->setMinimum(1);
  areaLightSampleSlider->setTickInterval(1);
  areaLightSampleSlider->setValue(2);

  raySampleSlider->setMaximum(50);
  raySampleSlider->setMinimum(1);
  raySampleSlider->setTickInterval(1);
  raySampleSlider->setValue(10);

  connect(areaLightSampleSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLightSamplingDisplay(int)));
  connect(raySampleSlider, SIGNAL(valueChanged(int)), this, SLOT(updateRaySamplingDisplay(int)));
  
  miscBox->setMinimumWidth(200);
  miscBox->setLayout(layout);
}

void SettingsWidget::loadSettings(const SceneSettings& settings) {
  imageResEdits[0]->setText(QString::number(settings.getWidth()));
  imageResEdits[1]->setText(QString::number(settings.getHeight()));
  
  globalPhotonsNumSlider->setValue(settings.getPhotonCount()/1000);
  causticPhotonsNumSlider->setValue(settings.getCausticsPhotonCount()/1000);
  
  areaLightSampleSlider->setValue(settings.getAreaLightResolution());
  
  raySampleSlider->setValue(settings.getSampleResolution());
}

void SettingsWidget::updatePhotonDisplay(int val) {
  globalPhotonsNumLabel[1]->setText(QString(QString::number(val)+tr(".000")));
  emit photonCountChanged(val*1000);
}

void SettingsWidget::updateCausticPhotonDisplay(int val) {
  causticPhotonsNumLabel[1]->setText(QString(QString::number(val)+tr(".000")));
  emit causticPhotonCountChanged(val*1000);
}

void SettingsWidget::updateLightSamplingDisplay(int val) {
  areaLightSampleLabel[1]->setText(QString::number(val));
  emit lightSamplingChanged(val);
}

void SettingsWidget::updateRaySamplingDisplay(int val) {
  raySampleLabel[1]->setText(QString::number(val));
  emit raySamplingChanged(val);
}

void SettingsWidget::updateHeight(const QString & text ) {
  emit heightChanged(text.toInt());
}

void SettingsWidget::updateWidth(const QString & text ) {
  emit widthChanged(text.toInt());
}

void SettingsWidget::updateIrradCount(int val) {
  globalIrradNumLabel[1]->setText(QString(QString::number(val)));
  emit irradCountChanged(val);
}

void SettingsWidget::updateIrradRadius(int val) {
  globalIrradRadLabel[1]->setText(QString(QString::number(val / 100.0)));
  emit irradRadiusChanged(val / 100.0);
}

void SettingsWidget::updateCausticsIrradCount(int val) {
  causticIrradNumLabel[1]->setText(QString(QString::number(val)));
  emit causticsIrradCountChanged(val);
}

void SettingsWidget::updateCausticsIrradRadius(int val) {
  causticIrradRadLabel[1]->setText(QString(QString::number(val / 100.0)));
  emit causticsIrradRadiusChanged(val / 100.0);
}

SettingsWidget::~SettingsWidget()
{
}


