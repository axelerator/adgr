//
// C++ Interface: settingswidget
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QGroupBox>

class QSlider;
class QLineEdit;
class QLabel;
class SceneSettings;
/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SettingsWidget : public QGroupBox
{
Q_OBJECT
public:
    SettingsWidget(QWidget *parent = 0);
    QWidget* labelWidget(QWidget *widget, const char *label);
    ~SettingsWidget();

signals:
 void photonCountChanged(long);
 void causticPhotonCountChanged(long);
 void heightChanged(int);
 void widthChanged(int);
 void lightSamplingChanged(int);
 void raySamplingChanged(int);
 void irradCountChanged(int);
 void irradRadiusChanged(float);
 void causticsIrradCountChanged(int);
 void causticsIrradRadiusChanged(float);

public slots:
  void loadSettings(const SceneSettings &settings);

private slots:
  void updatePhotonDisplay(int);
  void updateCausticPhotonDisplay(int);
  void updateLightSamplingDisplay(int);
  void updateHeight(const QString & text );
  void updateWidth(const QString & text );
  void updateRaySamplingDisplay(int);
  void updateIrradCount(int);
  void updateIrradRadius(int);
  void updateCausticsIrradCount(int);
  void updateCausticsIrradRadius(int);

private:
  void createImageResGroupBox();
  void createCausticMapGroupBox();
  void createGlobalMapGroupBox();
  void createMiscGroupBox();

  QLineEdit *width;
  QLineEdit *height;

  QGroupBox *imageResBox;
  QLabel *imageResLabels[2];
  QLineEdit *imageResEdits[2];

  QGroupBox *globalMapBox;
  QLabel *globalPhotonsNumLabel[2];
  QSlider *globalPhotonsNumSlider;
  QLabel *globalIrradNumLabel[2];
  QSlider *globalIrradNumSlider;
  QLabel *globalIrradRadLabel[2];
  QSlider *globalIrradRadSlider;

  QGroupBox *causticMapBox;
  QLabel *causticPhotonsNumLabel[2];
  QSlider *causticPhotonsNumSlider;
  QLabel *causticIrradNumLabel[2];
  QSlider *causticIrradNumSlider;
  QLabel *causticIrradRadLabel[2];
  QSlider *causticIrradRadSlider;

  QGroupBox *miscBox;
  QLabel *areaLightSampleLabel[2];
  QSlider *areaLightSampleSlider;
  QLabel *raySampleLabel[2];
  QSlider *raySampleSlider;

};

#endif
