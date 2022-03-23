/*
 *  Qtstalker - Commodity/Stock Charting and Technical Analysis Platform
 *
 *  Copyright (C) 2001-2009 Stefan S. Stratigakos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include "ChartToolbar.h"
//#include "Config.h"
//#include "RcFile.h"
#include "BarData.h"
#include "PrefDialog.h"
#include <qtooltip.h>
#include <q3accel.h>
#include <qvalidator.h>
//Added by qt3to4:
#include <QKeyEvent>

#include <QPixmap>
#include "../pics/eye.xpm"

#include "../pics/date.xpm"
#include "../pics/next.xpm"

#include <QTimer>
// #include <qdebug.h>
#include <Config.h>

ChartToolbar::ChartToolbar(QMainWindow *mw) : QToolBar("chartToolbar", mw )
{
  minPixelspace = 2;

  QString ts; // temporary string
  int ti;  // temporary integer

  ptDate = QDateTime::currentDateTime();

  BarData bd(ts);
  compressionCombo = new QComboBox(this);
  compressionCAction = addWidget(compressionCombo);
  bd.getBarLengthList(compressionList);
  compressionCombo->insertStringList(compressionList, -1);
  rcfile.loadData(RcFile::BarLength, ti);
  compressionCombo->setCurrentItem((BarData::BarLength) ti);
  QToolTip::add(compressionCombo, tr("Bar Length (Compression)"));
  connect(compressionCombo, SIGNAL(activated(int)), this, SLOT(compressionChanged(int)));
  connect(compressionCombo, SIGNAL(activated(int)), this, SIGNAL(signalBarLengthChanged(int)));

  // customBarCount = new QLineEdit(this);
  // customBarCountAction = addWidget(customBarCount);
  // // regexp: a non-zero digit followed by 0 to 4 other digits
  // QRegExp rx("[1-9]\\d{0,4}");
  // QValidator *rv = new QRegExpValidator(rx, this);
  // customBarCount->setValidator(rv);
  // // rcfile.loadData(RcFile::BarsToLoad, ti);
  // customBarCount->setText(QString::number(ti));
  // QToolTip::add(customBarCount, tr("custom bar length in seconds"));
  // connect(customBarCount, SIGNAL(lostFocus()), this, SLOT(barsChangedValidate()));
  // connect(customBarCount, SIGNAL(returnPressed()), this, SLOT(barsChanged()));

  // //try to get screen to refresh on a timer
  // refreshTbtn = new QToolButton(this);
  // refreshTAct = addWidget(refreshTbtn);
  // QToolTip::add(refreshTbtn, tr("turn on auto refresh"));
  // refreshTbtn->setToggleButton(TRUE);
  // connect(refreshTbtn, SIGNAL(toggled(bool)), this, SLOT(refreshTimerClicked(bool)));
  // refreshTimer = new QTimer(this);
  // connect(refreshTimer, SIGNAL(timeout()), SLOT(refresh()));

  //try to refresh on filechanges
  QPixmap icon(eye);
  refreshTbtn = new QToolButton(this);
  refreshTAct = addWidget(refreshTbtn);
  refreshTbtn->setIconSet(icon);
  QToolTip::add(refreshTbtn, tr("turn on auto refresh"));
  refreshTbtn->setToggleButton(TRUE);
  connect(refreshTbtn, SIGNAL(toggled(bool)), this, SLOT(fileWatcherClicked(bool)));
  watcherOn = FALSE;
  watcher = new QFileSystemWatcher();
  connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(slotFileModDetected()));

  cmpsBtnCustomBar = new QToolButton(this);
  cmpsActCustomBar = addWidget(cmpsBtnCustomBar);
  QToolTip::add(cmpsBtnCustomBar, tr("custom (sec) Compression"));
  cmpsBtnCustomBar->setToggleButton(TRUE);
  connect(cmpsBtnCustomBar, SIGNAL(clicked()), this, SLOT(cmpsBtnCustomBarClicked()));

  cmpsBtn5S = new QToolButton(this);
  cmpsAct5S = addWidget(cmpsBtn5S);
  QToolTip::add(cmpsBtn5S, tr("5 Second Compression"));
  cmpsBtn5S->setToggleButton(TRUE);
  connect(cmpsBtn5S, SIGNAL(clicked()), this, SLOT(cmpsBtn5SClicked()));

  cmpsBtn10S = new QToolButton(this);
  cmpsAct10S = addWidget(cmpsBtn10S);
  QToolTip::add(cmpsBtn10S, tr("10 Second Compression"));
  cmpsBtn10S->setToggleButton(TRUE);
  connect(cmpsBtn10S, SIGNAL(clicked()), this, SLOT(cmpsBtn10SClicked()));

  cmpsBtn15S = new QToolButton(this);
  cmpsAct15S = addWidget(cmpsBtn15S);
  QToolTip::add(cmpsBtn15S, tr("15 Second Compression"));
  cmpsBtn15S->setToggleButton(TRUE);
  connect(cmpsBtn15S, SIGNAL(clicked()), this, SLOT(cmpsBtn15SClicked()));

  cmpsBtn20S = new QToolButton(this);
  cmpsAct20S = addWidget(cmpsBtn20S);
  QToolTip::add(cmpsBtn20S, tr("20 Second Compression"));
  cmpsBtn20S->setToggleButton(TRUE);
  connect(cmpsBtn20S, SIGNAL(clicked()), this, SLOT(cmpsBtn20SClicked()));

  cmpsBtn30S = new QToolButton(this);
  cmpsAct30S = addWidget(cmpsBtn30S);
  QToolTip::add(cmpsBtn30S, tr("30 Second Compression"));
  cmpsBtn30S->setToggleButton(TRUE);
  connect(cmpsBtn30S, SIGNAL(clicked()), this, SLOT(cmpsBtn30SClicked()));

  cmpsBtn45S = new QToolButton(this);
  cmpsAct45S = addWidget(cmpsBtn45S);
  QToolTip::add(cmpsBtn45S, tr("45 Second Compression"));
  cmpsBtn45S->setToggleButton(TRUE);
  connect(cmpsBtn45S, SIGNAL(clicked()), this, SLOT(cmpsBtn45SClicked()));

  cmpsBtn1M = new QToolButton(this);
  cmpsAct1M = addWidget(cmpsBtn1M);
  QToolTip::add(cmpsBtn1M, tr("1 Minute Compression"));
  cmpsBtn1M->setToggleButton(TRUE);
  connect(cmpsBtn1M, SIGNAL(clicked()), this, SLOT(cmpsBtn1MClicked()));

  cmpsBtn2M = new QToolButton(this);
  cmpsAct2M = addWidget(cmpsBtn2M);
  QToolTip::add(cmpsBtn2M, tr("2 Minute Compression"));
  cmpsBtn2M->setToggleButton(TRUE);
  connect(cmpsBtn2M, SIGNAL(clicked()), this, SLOT(cmpsBtn2MClicked()));

  cmpsBtn3M = new QToolButton(this);
  cmpsAct3M = addWidget(cmpsBtn3M);
  QToolTip::add(cmpsBtn3M, tr("3 Minute Compression"));
  cmpsBtn3M->setToggleButton(TRUE);
  connect(cmpsBtn3M, SIGNAL(clicked()), this, SLOT(cmpsBtn3MClicked()));

  cmpsBtn4M = new QToolButton(this);
  cmpsAct4M = addWidget(cmpsBtn4M);
  QToolTip::add(cmpsBtn4M, tr("4 Minute Compression"));
  cmpsBtn4M->setToggleButton(TRUE);
  connect(cmpsBtn4M, SIGNAL(clicked()), this, SLOT(cmpsBtn4MClicked()));

  cmpsBtn5M = new QToolButton(this);
  cmpsAct5M = addWidget(cmpsBtn5M);
  QToolTip::add(cmpsBtn5M, tr("5 Minute Compression"));
  cmpsBtn5M->setToggleButton(TRUE);
  connect(cmpsBtn5M, SIGNAL(clicked()), this, SLOT(cmpsBtn5MClicked()));

  cmpsBtn6M = new QToolButton(this);
  cmpsAct6M = addWidget(cmpsBtn6M);
  QToolTip::add(cmpsBtn6M, tr("6 Minute Compression"));
  cmpsBtn6M->setToggleButton(TRUE);
  connect(cmpsBtn6M, SIGNAL(clicked()), this, SLOT(cmpsBtn6MClicked()));

  cmpsBtn10M = new QToolButton(this);
  cmpsAct10M = addWidget(cmpsBtn10M);
  QToolTip::add(cmpsBtn10M, tr("10 Minute Compression"));
  cmpsBtn10M->setToggleButton(TRUE);
  connect(cmpsBtn10M, SIGNAL(clicked()), this, SLOT(cmpsBtn10MClicked()));

  cmpsBtn12M = new QToolButton(this);
  cmpsAct12M = addWidget(cmpsBtn12M);
  QToolTip::add(cmpsBtn12M, tr("12 Minute Compression"));
  cmpsBtn12M->setToggleButton(TRUE);
  connect(cmpsBtn12M, SIGNAL(clicked()), this, SLOT(cmpsBtn12MClicked()));

  cmpsBtn15M = new QToolButton(this);
  cmpsAct15M = addWidget(cmpsBtn15M);
  QToolTip::add(cmpsBtn15M, tr("15 Minute Compression"));
  cmpsBtn15M->setToggleButton(TRUE);
  connect(cmpsBtn15M, SIGNAL(clicked()), this, SLOT(cmpsBtn15MClicked()));

  cmpsBtn20M = new QToolButton(this);
  cmpsAct20M = addWidget(cmpsBtn20M);
  QToolTip::add(cmpsBtn20M, tr("20 Minute Compression"));
  cmpsBtn20M->setToggleButton(TRUE);
  connect(cmpsBtn20M, SIGNAL(clicked()), this, SLOT(cmpsBtn20MClicked()));

  cmpsBtn30M = new QToolButton(this);
  cmpsAct30M = addWidget(cmpsBtn30M);
  QToolTip::add(cmpsBtn30M, tr("30 Minute Compression"));
  cmpsBtn30M->setToggleButton(TRUE);
  connect(cmpsBtn30M, SIGNAL(clicked()), this, SLOT(cmpsBtn30MClicked()));

  cmpsBtn45M = new QToolButton(this);
  cmpsAct45M = addWidget(cmpsBtn45M);
  QToolTip::add(cmpsBtn45M, tr("45 Minute Compression"));
  cmpsBtn45M->setToggleButton(TRUE);
  connect(cmpsBtn45M, SIGNAL(clicked()), this, SLOT(cmpsBtn45MClicked()));

  cmpsBtn1H = new QToolButton(this);
  cmpsAct1H = addWidget(cmpsBtn1H);
  QToolTip::add(cmpsBtn1H, tr("1 Hour Compression"));
  cmpsBtn1H->setToggleButton(TRUE);
  connect(cmpsBtn1H, SIGNAL(clicked()), this, SLOT(cmpsBtn1HClicked()));

  cmpsBtn2H = new QToolButton(this);
  cmpsAct2H = addWidget(cmpsBtn2H);
  QToolTip::add(cmpsBtn2H, tr("2 Hour Compression"));
  cmpsBtn2H->setToggleButton(TRUE);
  connect(cmpsBtn2H, SIGNAL(clicked()), this, SLOT(cmpsBtn2HClicked()));

  cmpsBtn3H = new QToolButton(this);
  cmpsAct3H = addWidget(cmpsBtn3H);
  QToolTip::add(cmpsBtn3H, tr("3 Hour Compression"));
  cmpsBtn3H->setToggleButton(TRUE);
  connect(cmpsBtn3H, SIGNAL(clicked()), this, SLOT(cmpsBtn3HClicked()));

  cmpsBtn4H = new QToolButton(this);
  cmpsAct4H = addWidget(cmpsBtn4H);
  QToolTip::add(cmpsBtn4H, tr("4 Hour Compression"));
  cmpsBtn4H->setToggleButton(TRUE);
  connect(cmpsBtn4H, SIGNAL(clicked()), this, SLOT(cmpsBtn4HClicked()));

  cmpsBtn6H = new QToolButton(this);
  cmpsAct6H = addWidget(cmpsBtn6H);
  QToolTip::add(cmpsBtn6H, tr("6 Hour Compression"));
  cmpsBtn6H->setToggleButton(TRUE);
  connect(cmpsBtn6H, SIGNAL(clicked()), this, SLOT(cmpsBtn6HClicked()));

  cmpsBtn8H = new QToolButton(this);
  cmpsAct8H = addWidget(cmpsBtn8H);
  QToolTip::add(cmpsBtn8H, tr("8 Hour Compression"));
  cmpsBtn8H->setToggleButton(TRUE);
  connect(cmpsBtn8H, SIGNAL(clicked()), this, SLOT(cmpsBtn8HClicked()));

  cmpsBtn12H = new QToolButton(this);
  cmpsAct12H = addWidget(cmpsBtn12H);
  QToolTip::add(cmpsBtn12H, tr("12 Hour Compression"));
  cmpsBtn12H->setToggleButton(TRUE);
  connect(cmpsBtn12H, SIGNAL(clicked()), this, SLOT(cmpsBtn12HClicked()));

  cmpsBtn1D = new QToolButton(this);
  cmpsAct1D = addWidget(cmpsBtn1D);
  QToolTip::add(cmpsBtn1D, tr("1 Day Compression"));
  cmpsBtn1D->setToggleButton(TRUE);
  connect(cmpsBtn1D, SIGNAL(clicked()), this, SLOT(cmpsBtn1DClicked()));

  cmpsBtn2D = new QToolButton(this);
  cmpsAct2D = addWidget(cmpsBtn2D);
  QToolTip::add(cmpsBtn2D, tr("2 Day Compression"));
  cmpsBtn2D->setToggleButton(TRUE);
  connect(cmpsBtn2D, SIGNAL(clicked()), this, SLOT(cmpsBtn2DClicked()));

  cmpsBtn3D = new QToolButton(this);
  cmpsAct3D = addWidget(cmpsBtn3D);
  QToolTip::add(cmpsBtn3D, tr("3 Day Compression"));
  cmpsBtn3D->setToggleButton(TRUE);
  connect(cmpsBtn3D, SIGNAL(clicked()), this, SLOT(cmpsBtn3DClicked()));

  cmpsBtn1W = new QToolButton(this);
  cmpsAct1W = addWidget(cmpsBtn1W);
  QToolTip::add(cmpsBtn1W, tr("1 Week Compression"));
  cmpsBtn1W->setToggleButton(TRUE);
  connect(cmpsBtn1W, SIGNAL(clicked()), this, SLOT(cmpsBtn1WClicked()));

  cmpsBtnMN = new QToolButton(this);
  cmpsActMN = addWidget(cmpsBtnMN);
  QToolTip::add(cmpsBtnMN, tr("1 Month Compression"));
  cmpsBtnMN->setToggleButton(TRUE);
  connect(cmpsBtnMN, SIGNAL(clicked()), this, SLOT(cmpsBtnMNClicked()));

  addSeparator();

  cmpsBtn2Dw = new QToolButton(this);
  cmpsAct2Dw = addWidget(cmpsBtn2Dw);
  QToolTip::add(cmpsBtn2Dw, tr("2 Day w weekend trading"));
  cmpsBtn2Dw->setToggleButton(TRUE);
  connect(cmpsBtn2Dw, SIGNAL(clicked()), this, SLOT(cmpsBtn2DwClicked()));

  cmpsBtn3Dw = new QToolButton(this);
  cmpsAct3Dw = addWidget(cmpsBtn3Dw);
  QToolTip::add(cmpsBtn3Dw, tr("3 Day w weekend trading"));
  cmpsBtn3Dw->setToggleButton(TRUE);
  connect(cmpsBtn3Dw, SIGNAL(clicked()), this, SLOT(cmpsBtn3DwClicked()));

  cmpsBtn5Dw = new QToolButton(this);
  cmpsAct5Dw = addWidget(cmpsBtn5Dw);
  QToolTip::add(cmpsBtn5Dw, tr("5 Day w weekend trading"));
  cmpsBtn5Dw->setToggleButton(TRUE);
  connect(cmpsBtn5Dw, SIGNAL(clicked()), this, SLOT(cmpsBtn5DwClicked()));

  cmpsBtn8Dw = new QToolButton(this);
  cmpsAct8Dw = addWidget(cmpsBtn8Dw);
  QToolTip::add(cmpsBtn8Dw, tr("8 Day w weekend trading"));
  cmpsBtn8Dw->setToggleButton(TRUE);
  connect(cmpsBtn8Dw, SIGNAL(clicked()), this, SLOT(cmpsBtn8DwClicked()));

  compressionChanged((BarData::BarLength) ti);

  addSeparator();

  ps3Button = new QToolButton(this);
  ps3Button->setToggleButton(TRUE);
  rcfile.loadData(RcFile::PSButton, ps3ButtonText, 3);

  ps2Button = new QToolButton(this);
  ps2Button->setToggleButton(TRUE);
  rcfile.loadData(RcFile::PSButton, ps2ButtonText, 2);
  if (ps2ButtonText == ps3ButtonText && ps3ButtonText == 6)
  {
    ps2ButtonText = 4;
    rcfile.saveData(RcFile::PSButton, ps2ButtonText, 2);
  }

  ps1Button = new QToolButton(this);
  ps1Button->setToggleButton(TRUE);
  rcfile.loadData(RcFile::PSButton, ps1ButtonText, 1);
  if (ps1ButtonText == ps3ButtonText && ps3ButtonText == 6)
  {
    ps1ButtonText = 2;
    rcfile.saveData(RcFile::PSButton, ps1ButtonText, 1);
  }

  pixelspace = new QSpinBox(this);
  pixelspaceAction = addWidget(pixelspace);
  pixelspace->setRange(minPixelspace, 9);
  rcfile.loadData(RcFile::Pixelspace, ti);
  connect(pixelspace, SIGNAL(valueChanged(int)), this, SIGNAL(signalPixelspaceChanged(int)));
  connect(pixelspace, SIGNAL(valueChanged(int)), this, SLOT(slotPixelspaceChanged(int)));
  pixelspace->setValue(ti);
  QToolTip::add(pixelspace, tr("Bar Spacing"));

  addWidget(ps1Button);
  connect(ps1Button, SIGNAL(clicked()), this, SLOT(ps1ButtonClicked()));

  addWidget(ps2Button);
  connect(ps2Button, SIGNAL(clicked()), this, SLOT(ps2ButtonClicked()));

  addWidget(ps3Button);
  connect(ps3Button, SIGNAL(clicked()), this, SLOT(ps3ButtonClicked()));

  addSeparator();

  barCount = new QLineEdit(this);
  barCountAction = addWidget(barCount);
  // regexp: a non-zero digit followed by 0 to 4 other digits
  QRegExp rx("[1-9]\\d{0,4}");
  QValidator *rv = new QRegExpValidator(rx, this);
  barCount->setValidator(rv);
  rcfile.loadData(RcFile::BarsToLoad, ti);
  barCount->setText(QString::number(ti));
  QToolTip::add(barCount, tr("Total bars to load"));
  connect(barCount, SIGNAL(lostFocus()), this, SLOT(barsChangedValidate()));
  connect(barCount, SIGNAL(returnPressed()), this, SLOT(barsChanged()));

  addSeparator();

  ptdButton = new QToolButton(this);
  ptdAction = addWidget(ptdButton);
  QToolTip::add(ptdButton, tr("Paper Trade Date"));
  ptdButton->setPixmap(date);
  connect(ptdButton, SIGNAL(clicked()), this, SLOT(paperTradeDate()));

  ptnButton = new QToolButton(this);
  ptnAction = addWidget(ptnButton);
  QToolTip::add(ptnButton, tr("Load Next Bar"));
  ptnButton->setPixmap(next);
  connect(ptnButton, SIGNAL(clicked()), this, SLOT(paperTradeNextBar()));
  //ptnButton->setFocusPolicy(QWidget::StrongFocus);

  slider = new QSlider(this);
  sliderAction = addWidget(slider);
  slider->setOrientation(Qt::Horizontal);
  connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(signalSliderChanged(int)));
  slider->setEnabled(FALSE);
  QToolTip::add(slider, tr("Pan Chart"));

  slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
  //setStretchableWidget(slider);

  Q3Accel *a = new Q3Accel(mw);
  connect(a, SIGNAL(activated(int)), this, SLOT(slotAccel(int)));
  a->insertItem(Qt::CTRL+Qt::Key_End, ChartPannerFocus);
  a->insertItem(Qt::CTRL+Qt::Key_Plus, BarsLoadedFocus);
  a->insertItem(Qt::CTRL+Qt::Key_Minus, BarSpacingFocus);
  a->insertItem(Qt::CTRL+Qt::Key_Prior, BarLengthFocus);
  // a->insertItem(Qt::CTRL+Qt::Key_B, ToolbarFocus);

  focusFlag = BarLengthFocus;

  connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(slotOrientationChanged(Qt::Orientation)));

  //setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
  //setVerticallyStretchable(TRUE);
}

ChartToolbar::~ChartToolbar()
{
}

int ChartToolbar::getBars()
{
  bool ok;
  int t = barCount->text().toInt(&ok);
  if (ok)
    return t;
  else
    return 0;
}

void ChartToolbar::enableSlider(bool d)
{
  slider->setEnabled(d);
}

void ChartToolbar::setPixelspace(int min, int d)
{
  pixelspace->blockSignals(TRUE);
  pixelspace->setRange(min, 9);
  pixelspace->setValue(d);
  pixelspace->blockSignals(FALSE);
}

int ChartToolbar::getPixelspace()
{
  return pixelspace->value();
}

int ChartToolbar::getBarLengthInt()
{
  return compressionCombo->currentItem();
}

QString ChartToolbar::getBarLength()
{
  return compressionCombo->currentText();
}

int ChartToolbar::getSlider()
{
  return slider->value();
}

int ChartToolbar::setSliderStart(int width, int records)
{
  int page = width / getPixelspace();
  int max = records - page;
  if (max < 0)
    max = 0;
  slider->blockSignals(TRUE);
  slider->setRange(0, records - 1);
  int rc = 0;

  if ((int) records < page)
    slider->setValue(0);
  else
  {
    slider->setValue(max + 1);
    rc = max + 1;
  }

  slider->blockSignals(FALSE);

  return rc;
}

void ChartToolbar::saveSettings()
{
  rcfile.saveData(RcFile::BarsToLoad, getBars());
  rcfile.saveData(RcFile::BarLength, getBarLengthInt());
  rcfile.saveData(RcFile::Pixelspace, getPixelspace());
  //rcfile.savePoint(RcFile::ChartToolBarPos, pos());
  //FIXME: IMPORTANT- save all belonged to restore the position of the toolbar. maybe not here but in Qtstalker.cpp. Possible is this the way
  //http://doc.trolltech.com/3.3/qdockarea.html#operator-lt-lt
}

void ChartToolbar::slotSetButtonView()
{
  // do all relating to visual aspekts

  QString ts;    // temporary variables
  int ti;
  bool tb;

  rcfile.loadData(RcFile::ShowCmpsComboBox, tb);
  if (tb) compressionCAction->setVisible(true);
  else compressionCAction->setVisible(false);

  rcfile.loadData(RcFile::ShowCmpsCustomBarBtn, tb);
  if (tb) cmpsActCustomBar->setVisible(true);
  else cmpsActCustomBar->setVisible(false);
  cmpsBtnCustomBar->setText("?s");

  rcfile.loadData(RcFile::ShowCmps5SBtn, tb);
  if (tb) cmpsAct5S->setVisible(true);
  else cmpsAct5S->setVisible(false);
  cmpsBtn5S->setText("5s");

  rcfile.loadData(RcFile::ShowCmps10SBtn, tb);
  if (tb) cmpsAct10S->setVisible(true);
  else cmpsAct10S->setVisible(false);
  cmpsBtn10S->setText("10s");

  rcfile.loadData(RcFile::ShowCmps15SBtn, tb);
  if (tb) cmpsAct15S->setVisible(true);
  else cmpsAct15S->setVisible(false);
  cmpsBtn15S->setText("15s");

  rcfile.loadData(RcFile::ShowCmps20SBtn, tb);
  if (tb) cmpsAct20S->setVisible(true);
  else cmpsAct20S->setVisible(false);
  cmpsBtn20S->setText("20s");

  rcfile.loadData(RcFile::ShowCmps30SBtn, tb);
  if (tb) cmpsAct30S->setVisible(true);
  else cmpsAct30S->setVisible(false);
  cmpsBtn30S->setText("30s");

  rcfile.loadData(RcFile::ShowCmps45SBtn, tb);
  if (tb) cmpsAct45S->setVisible(true);
  else cmpsAct45S->setVisible(false);
  cmpsBtn45S->setText("45s");

  rcfile.loadData(RcFile::ShowCmps1MBtn, tb);
  if (tb) cmpsAct1M->setVisible(true);
  else cmpsAct1M->setVisible(false);
  cmpsBtn1M->setText("1m");

  rcfile.loadData(RcFile::ShowCmps2MBtn, tb);
  if (tb) cmpsAct2M->setVisible(true);
  else cmpsAct2M->setVisible(false);
  cmpsBtn2M->setText("2m");

  rcfile.loadData(RcFile::ShowCmps3MBtn, tb);
  if (tb) cmpsAct3M->setVisible(true);
  else cmpsAct3M->setVisible(false);
  cmpsBtn3M->setText("3m");

  rcfile.loadData(RcFile::ShowCmps4MBtn, tb);
  if (tb) cmpsAct4M->setVisible(true);
  else cmpsAct4M->setVisible(false);
  cmpsBtn4M->setText("4m");

  rcfile.loadData(RcFile::ShowCmps5MBtn, tb);
  if (tb) cmpsAct5M->setVisible(true);
  else cmpsAct5M->setVisible(false);
  cmpsBtn5M->setText("5m");

  rcfile.loadData(RcFile::ShowCmps6MBtn, tb);
  if (tb) cmpsAct6M->setVisible(true);
  else cmpsAct6M->setVisible(false);
  cmpsBtn6M->setText("6m");

  rcfile.loadData(RcFile::ShowCmps10MBtn, tb);
  if (tb) cmpsAct10M->setVisible(true);
  else cmpsAct10M->setVisible(false);
  cmpsBtn10M->setText("10m");

  rcfile.loadData(RcFile::ShowCmps12MBtn, tb);
  if (tb) cmpsAct12M->setVisible(true);
  else cmpsAct12M->setVisible(false);
  cmpsBtn12M->setText("12m");

  rcfile.loadData(RcFile::ShowCmps15MBtn, tb);
  if (tb) cmpsAct15M->setVisible(true);
  else cmpsAct15M->setVisible(false);
  cmpsBtn15M->setText("15m");

  rcfile.loadData(RcFile::ShowCmps20MBtn, tb);
  if (tb) cmpsAct20M->setVisible(true);
  else cmpsAct20M->setVisible(false);
  cmpsBtn20M->setText("20m");

  rcfile.loadData(RcFile::ShowCmps30MBtn, tb);
  if (tb) cmpsAct30M->setVisible(true);
  else cmpsAct30M->setVisible(false);
  cmpsBtn30M->setText("30m");

  rcfile.loadData(RcFile::ShowCmps45MBtn, tb);
  if (tb) cmpsAct45M->setVisible(true);
  else cmpsAct45M->setVisible(false);
  cmpsBtn45M->setText("45m");

  rcfile.loadData(RcFile::ShowCmps1HBtn, tb);
  if (tb) cmpsAct1H->setVisible(true);
  else cmpsAct1H->setVisible(false);
  cmpsBtn1H->setText("1h");

  rcfile.loadData(RcFile::ShowCmps2HBtn, tb);
  if (tb) cmpsAct2H->setVisible(true);
  else cmpsAct2H->setVisible(false);
  cmpsBtn2H->setText("2h");

  rcfile.loadData(RcFile::ShowCmps3HBtn, tb);
  if (tb) cmpsAct3H->setVisible(true);
  else cmpsAct3H->setVisible(false);
  cmpsBtn3H->setText("3h");

  rcfile.loadData(RcFile::ShowCmps4HBtn, tb);
  if (tb) cmpsAct4H->setVisible(true);
  else cmpsAct4H->setVisible(false);
  cmpsBtn4H->setText("4h");

  rcfile.loadData(RcFile::ShowCmps6HBtn, tb);
  if (tb) cmpsAct6H->setVisible(true);
  else cmpsAct6H->setVisible(false);
  cmpsBtn6H->setText("6h");

  rcfile.loadData(RcFile::ShowCmps8HBtn, tb);
  if (tb) cmpsAct8H->setVisible(true);
  else cmpsAct8H->setVisible(false);
  cmpsBtn8H->setText("8h");

  rcfile.loadData(RcFile::ShowCmps12HBtn, tb);
  if (tb) cmpsAct12H->setVisible(true);
  else cmpsAct12H->setVisible(false);
  cmpsBtn12H->setText("12h");

  rcfile.loadData(RcFile::ShowCmps1DBtn, tb);
  if (tb) cmpsAct1D->setVisible(true);
  else cmpsAct1D->setVisible(false);
  cmpsBtn1D->setText("1D");

  rcfile.loadData(RcFile::ShowCmps2DBtn, tb);
  if (tb) cmpsAct2D->setVisible(true);
  else cmpsAct2D->setVisible(false);
  cmpsBtn2D->setText("2D");

  rcfile.loadData(RcFile::ShowCmps3DBtn, tb);
  if (tb) cmpsAct3D->setVisible(true);
  else cmpsAct3D->setVisible(false);
  cmpsBtn3D->setText("3D");

  rcfile.loadData(RcFile::ShowCmps1WBtn, tb);
  if (tb) cmpsAct1W->setVisible(true);
  else cmpsAct1W->setVisible(false);
  cmpsBtn1W->setText("1W");

  rcfile.loadData(RcFile::ShowCmpsMNBtn, tb);
  if (tb) cmpsActMN->setVisible(true);
  else cmpsActMN->setVisible(false);
  cmpsBtnMN->setText("MN");

  rcfile.loadData(RcFile::ShowCmps2DwBtn, tb);
  if (tb) cmpsAct2Dw->setVisible(true);
  else cmpsAct2Dw->setVisible(false);
  cmpsBtn2Dw->setText("2Dw");

  rcfile.loadData(RcFile::ShowCmps3DwBtn, tb);
  if (tb) cmpsAct3Dw->setVisible(true);
  else cmpsAct3Dw->setVisible(false);
  cmpsBtn3Dw->setText("3Dw");

  rcfile.loadData(RcFile::ShowCmps5DwBtn, tb);
  if (tb) cmpsAct5Dw->setVisible(true);
  else cmpsAct5Dw->setVisible(false);
  cmpsBtn5Dw->setText("5Dw");

  rcfile.loadData(RcFile::ShowCmps8DwBtn, tb);
  if (tb) cmpsAct8Dw->setVisible(true);
  else cmpsAct8Dw->setVisible(false);
  cmpsBtn8Dw->setText("8Dw");

  rcfile.loadData(RcFile::ShowBarSpSpinbox, tb);
  if (tb) pixelspaceAction->setVisible(true);
  else pixelspaceAction->setVisible(false);
  pixelspace->setMaximumWidth(40); // FIXME:calc as a function of app font metrics

  rcfile.loadData(RcFile::PSButton, ps1ButtonText, 1);
  ts = QString::number(ps1ButtonText);
  QToolTip::add(ps1Button, tr("Set Bar Spacing to " + ts));
  ps1Button->setText(ts);

  ts = QString::number(ps2ButtonText);
  QToolTip::add(ps2Button, tr("Set Bar Spacing to " + ts));
  ps2Button->setText(ts);

  ts = QString::number(ps3ButtonText);
  QToolTip::add(ps3Button, tr("Set Bar Spacing to " + ts));
  ps3Button->setText(ts);

  rcfile.loadData(RcFile::ShowBarsToLoadField, tb);
  if (tb) barCountAction->setVisible(true);
  else barCountAction->setVisible(false);
  barCount->setMaximumWidth(40);// FIXME:calc as a function of app font metrics

  rcfile.loadData(RcFile::ShowSlider, tb);
  if (tb) sliderAction->setVisible(true);
  else sliderAction->setVisible(false);

}
void ChartToolbar::setFocus()
{
  compressionCombo->setFocus();
  focusFlag = BarLengthFocus;
}

void ChartToolbar::slotAccel(int id)
{
  switch(id)
  {
    case BarLengthFocus:
      compressionCombo->setFocus();
      focusFlag = BarLengthFocus;
      break;
    case ChartPannerFocus:
      slider->setFocus();
      focusFlag = ChartPannerFocus;
      break;
    case BarsLoadedFocus:
      barCount->setFocus();
      focusFlag = BarsLoadedFocus;
      break;
    case BarSpacingFocus:
      pixelspace->setFocus();
      focusFlag = BarSpacingFocus;
      break;
    case ToolbarFocus:
      setFocus();
      break;
    default:
      break;
  }
}

void ChartToolbar::doKeyPress(QKeyEvent *key)
{
  key->accept();

  if (! key->state())
  {
    switch(focusFlag)
    {
      case BarLengthFocus:
//        compressionCombo->doKeyPress(key);
	break;
      case BarSpacingFocus:
//        pixelspace->doKeyPress(key);
	break;
      case BarsLoadedFocus:
//        barCount->doKeyPress(key);
	break;
      case ChartPannerFocus:
//        slider->doKeyPress(key);
	break;
      default:
        break;
    }
  }
  else
  {
    if (key->state() == Qt::ControlButton)
    {
      switch(key->key())
      {
        case Qt::Key_Prior:
	  slotAccel(BarLengthFocus);
          break;
        case Qt::Key_Plus:
	  slotAccel(BarsLoadedFocus);
          break;
        case Qt::Key_End:
	  slotAccel(ChartPannerFocus);
          break;
        case Qt::Key_Minus:
	  slotAccel(BarSpacingFocus);
          break;
   //      case Qt::Key_B:
	  // slotAccel(ToolbarFocus);
   //        break;
        case Qt::Key_R:
          if (watcherOn)
            fileWatcherClicked(FALSE);
          else
            fileWatcherClicked(TRUE);
    // slotAccel(ToggleReload);
          break;          
        default:
          break;
      }
    }
  }
}

void ChartToolbar::barsChanged()
{
  bool ok;
  int t = barCount->text().toInt(&ok);

///
 //just trying to store settings for "SYMBOL.cpp" to use
  Config config;
  QString ts2;
  ts2 = QString::number(t);
  // qDebug() << ts2;
  config.setData(Config::Bars, ts2);
///

  if (ok)
    emit signalBarsChanged(t);
}

void ChartToolbar::barsChangedValidate()
{
  // The barCount validator ensures non-zero integer value, but if users moves
  // focus without completing the text entry, then app can try to load zero bars
  // which would cause grief.
  bool ok;
  int t = barCount->text().toInt(&ok);
  if (!ok || t == 0)
    barCount->setText("250");
}

void ChartToolbar::ps1ButtonClicked()
{
  int ti;
  rcfile.loadData(RcFile::PSButton, ti, 1);
  pixelspace->setValue(ti);
  emit slotPixelspaceChanged(ti);
}

void ChartToolbar::ps2ButtonClicked()
{
  int ti;
  rcfile.loadData(RcFile::PSButton, ti, 2);
  pixelspace->setValue(ti);
  emit slotPixelspaceChanged(ti);
}

void ChartToolbar::ps3ButtonClicked()
{
  int ti;
  rcfile.loadData(RcFile::PSButton, ti, 3);
  pixelspace->setValue(ti);
  emit slotPixelspaceChanged(ti);
}

void ChartToolbar::cmpsBtnCustomBarClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)0);
  compressionChanged(0);
  emit signalBarLengthChanged((int)0);
}

void ChartToolbar::cmpsBtn5SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)1);
  compressionChanged(1);
  emit signalBarLengthChanged((int)1);
}

void ChartToolbar::cmpsBtn10SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)2);
  compressionChanged(2);
  emit signalBarLengthChanged((int)2);
}

void ChartToolbar::cmpsBtn15SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)3);
  compressionChanged(3);
  emit signalBarLengthChanged((int)3);
}

void ChartToolbar::cmpsBtn20SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)4);
  compressionChanged(4);
  emit signalBarLengthChanged((int)4);
}

void ChartToolbar::cmpsBtn30SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)5);
  compressionChanged(5);
  emit signalBarLengthChanged((int)5);
}

void ChartToolbar::cmpsBtn45SClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)6);
  compressionChanged(6);
  emit signalBarLengthChanged((int)6);
}

void ChartToolbar::cmpsBtn1MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)7);
  compressionChanged(7);
  emit signalBarLengthChanged((int)7);
}

void ChartToolbar::cmpsBtn2MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)8);
  compressionChanged(8);
  emit signalBarLengthChanged((int)8);
}

void ChartToolbar::cmpsBtn3MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)9);
  compressionChanged(9);
  emit signalBarLengthChanged((int)9);
}

void ChartToolbar::cmpsBtn4MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)10);
  compressionChanged(10);
  emit signalBarLengthChanged((int)10);
}

void ChartToolbar::cmpsBtn5MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)11);
  compressionChanged(11);
  emit signalBarLengthChanged((int)11);
}

void ChartToolbar::cmpsBtn6MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)12);
  compressionChanged(12);
  emit signalBarLengthChanged((int)12);
}

void ChartToolbar::cmpsBtn10MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)13);
  compressionChanged(13);
  emit signalBarLengthChanged((int)13);
}

void ChartToolbar::cmpsBtn12MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)14);
  compressionChanged(14);
  emit signalBarLengthChanged((int)14);
}

void ChartToolbar::cmpsBtn15MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)15);
  compressionChanged(15);
  emit signalBarLengthChanged((int)15);
}

void ChartToolbar::cmpsBtn20MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)16);
  compressionChanged(16);
  emit signalBarLengthChanged((int)16);
}

void ChartToolbar::cmpsBtn30MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)17);
  compressionChanged(17);
  emit signalBarLengthChanged((int)17);
}

void ChartToolbar::cmpsBtn45MClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)18);
  compressionChanged(18);
  emit signalBarLengthChanged((int)18);
}

void ChartToolbar::cmpsBtn1HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)19);
  compressionChanged(19);
  emit signalBarLengthChanged((int)19);
}

void ChartToolbar::cmpsBtn2HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)20);
  compressionChanged(20);
  emit signalBarLengthChanged((int)20);
}

void ChartToolbar::cmpsBtn3HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)21);
  compressionChanged(21);
  emit signalBarLengthChanged((int)21);
}

void ChartToolbar::cmpsBtn4HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)22);
  compressionChanged(22);
  emit signalBarLengthChanged((int)22);
}

void ChartToolbar::cmpsBtn6HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)23);
  compressionChanged(23);
  emit signalBarLengthChanged((int)23);
}

void ChartToolbar::cmpsBtn8HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)24);
  compressionChanged(24);
  emit signalBarLengthChanged((int)24);
}

void ChartToolbar::cmpsBtn12HClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)25);
  compressionChanged(25);
  emit signalBarLengthChanged((int)25);
}

void ChartToolbar::cmpsBtn1DClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)26);
  compressionChanged(26);
  emit signalBarLengthChanged((int)26);
}

void ChartToolbar::cmpsBtn2DClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)27);
  compressionChanged(27);
  emit signalBarLengthChanged((int)27);
}

void ChartToolbar::cmpsBtn3DClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)28);
  compressionChanged(28);
  emit signalBarLengthChanged((int)28);
}

void ChartToolbar::cmpsBtn1WClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)29);
  compressionChanged(29);
  emit signalBarLengthChanged((int)29);
}

void ChartToolbar::cmpsBtnMNClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)30);
  compressionChanged(30);
  emit signalBarLengthChanged((int)30);
}

void ChartToolbar::cmpsBtn2DwClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)31);
  compressionChanged(31);
  emit signalBarLengthChanged((int)31);
}

void ChartToolbar::cmpsBtn3DwClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)32);
  compressionChanged(32);
  emit signalBarLengthChanged((int)32);
}

void ChartToolbar::cmpsBtn5DwClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)33);
  compressionChanged(33);
  emit signalBarLengthChanged((int)33);
}

void ChartToolbar::cmpsBtn8DwClicked()
{
  compressionCombo->setCurrentItem((BarData::BarLength)34);
  compressionChanged(34);
  emit signalBarLengthChanged((int)34);
}

void ChartToolbar::compressionChanged(int compression)
{
  compression == 0 ? cmpsBtnCustomBar->setOn(TRUE) : cmpsBtnCustomBar->setOn(FALSE);
  compression == 1 ? cmpsBtn5S->setOn(TRUE) : cmpsBtn5S->setOn(FALSE);
  compression == 2 ? cmpsBtn10S->setOn(TRUE) : cmpsBtn10S->setOn(FALSE);
  compression == 3 ? cmpsBtn15S->setOn(TRUE) : cmpsBtn15S->setOn(FALSE);
  compression == 4 ? cmpsBtn20S->setOn(TRUE) : cmpsBtn20S->setOn(FALSE);
  compression == 5 ? cmpsBtn30S->setOn(TRUE) : cmpsBtn30S->setOn(FALSE);
  compression == 6 ? cmpsBtn45S->setOn(TRUE) : cmpsBtn45S->setOn(FALSE);
  compression == 7 ? cmpsBtn1M->setOn(TRUE) : cmpsBtn1M->setOn(FALSE);
  compression == 8 ? cmpsBtn2M->setOn(TRUE) : cmpsBtn2M->setOn(FALSE);
  compression == 9 ? cmpsBtn3M->setOn(TRUE) : cmpsBtn3M->setOn(FALSE);
  compression == 10 ? cmpsBtn4M->setOn(TRUE) : cmpsBtn4M->setOn(FALSE);
  compression == 11 ? cmpsBtn5M->setOn(TRUE) : cmpsBtn5M->setOn(FALSE);
  compression == 12 ? cmpsBtn6M->setOn(TRUE) : cmpsBtn6M->setOn(FALSE);
  compression == 13 ? cmpsBtn10M->setOn(TRUE) : cmpsBtn10M->setOn(FALSE);
  compression == 14 ? cmpsBtn12M->setOn(TRUE) : cmpsBtn12M->setOn(FALSE);
  compression == 15 ? cmpsBtn15M->setOn(TRUE) : cmpsBtn15M->setOn(FALSE);
  compression == 16 ? cmpsBtn20M->setOn(TRUE) : cmpsBtn20M->setOn(FALSE);
  compression == 17 ? cmpsBtn30M->setOn(TRUE) : cmpsBtn30M->setOn(FALSE);
  compression == 18 ? cmpsBtn45M->setOn(TRUE) : cmpsBtn45M->setOn(FALSE);
  compression == 19 ? cmpsBtn1H->setOn(TRUE) : cmpsBtn1H->setOn(FALSE);
  compression == 20 ? cmpsBtn2H->setOn(TRUE) : cmpsBtn2H->setOn(FALSE);
  compression == 21 ? cmpsBtn3H->setOn(TRUE) : cmpsBtn3H->setOn(FALSE);
  compression == 22 ? cmpsBtn4H->setOn(TRUE) : cmpsBtn4H->setOn(FALSE);
  compression == 23 ? cmpsBtn6H->setOn(TRUE) : cmpsBtn6H->setOn(FALSE);
  compression == 24 ? cmpsBtn8H->setOn(TRUE) : cmpsBtn8H->setOn(FALSE);
  compression == 25 ? cmpsBtn12H->setOn(TRUE) : cmpsBtn12H->setOn(FALSE);
  compression == 26 ? cmpsBtn1D->setOn(TRUE) : cmpsBtn1D->setOn(FALSE);
  compression == 27 ? cmpsBtn2D->setOn(TRUE) : cmpsBtn2D->setOn(FALSE);
  compression == 28 ? cmpsBtn3D->setOn(TRUE) : cmpsBtn3D->setOn(FALSE);
  compression == 29 ? cmpsBtn1W->setOn(TRUE) : cmpsBtn1W->setOn(FALSE);
  compression == 30 ? cmpsBtnMN->setOn(TRUE) : cmpsBtnMN->setOn(FALSE);
  compression == 31 ? cmpsBtn2Dw->setOn(TRUE) : cmpsBtn2Dw->setOn(FALSE);
  compression == 32 ? cmpsBtn3Dw->setOn(TRUE) : cmpsBtn3Dw->setOn(FALSE);
  compression == 33 ? cmpsBtn5Dw->setOn(TRUE) : cmpsBtn5Dw->setOn(FALSE);
  compression == 34 ? cmpsBtn8Dw->setOn(TRUE) : cmpsBtn8Dw->setOn(FALSE);
}

void ChartToolbar::slotOrientationChanged(Qt::Orientation o)
{
    slider->setOrientation(o);
   // updateGeometry();

 // if(o)
 // {

 // }
}

void ChartToolbar::slotPixelspaceChanged(int ps)
{
  ps1Button->setOn(FALSE);
  ps2Button->setOn(FALSE);
  ps3Button->setOn(FALSE);

  if (ps == ps1ButtonText)
    ps1Button->setOn(TRUE);
  if (ps == ps2ButtonText)
    ps2Button->setOn(TRUE);
  if (ps == ps3ButtonText)
    ps3Button->setOn(TRUE);
}


// void ChartToolbar::refreshTimerClicked(bool d)
// {
//   if (d) {
//     refreshTimer->start(2000);
//     refresh();
//   }
//   else {
//     refreshTimer->stop();
//   }
// }

void ChartToolbar::slotDifferentFile(QString selection)
{
  watcher->removePaths(watcher->files());
  watcher->addPath(selection);
}

void ChartToolbar::fileWatcherClicked(bool d)
{
  //set flag to see if we should refresh or not
  if (d)
  {
    watcherOn = TRUE;
    refresh();
  }
  else watcherOn = FALSE;
}

void ChartToolbar::slotFileModDetected()
{
  if (watcherOn) refresh();
}


void ChartToolbar::refresh()
{
    if (!ptdAction->isVisible())
      emit signalPaperTradeNextBar();
    else
      paperTradeNextBar();
}



//*********************************************************************
//******************** paper trade stuff ******************************
//*********************************************************************

void ChartToolbar::getPaperTradeDate(QDateTime &d)
{
  d = ptDate;
}

void ChartToolbar::paperTradeDate()
{
  QString dl = tr("Last Date");
  QString tl = tr("Last Time");
  QString pl = "Date";

  PrefDialog *dialog = new PrefDialog(this);
  dialog->setCaption(tr("Paper trade date select."));
  dialog->createPage(pl);
  dialog->addDateItem(dl, pl, ptDate);
  dialog->addTimeItem(tl, pl, ptDate);

  int rc = dialog->exec();

  if (rc == QDialog::Accepted)
  {
    QDateTime dt;
    dialog->getDate(dl, dt);
    ptDate.setDate(dt.date());
    dialog->getTime(tl, dt); //bug: wasn't setting time
    ptDate.setTime(dt.time());

    emit signalPaperTradeNextBar();
  }

  delete dialog;
}

void ChartToolbar::paperTradeClicked(bool d)
{
  if (d)
  {
    ptdAction->setVisible(true);
    ptnAction->setVisible(true);
    barCount->setEnabled(FALSE);
  }
  else
  {
    ptdAction->setVisible(false);
    ptnAction->setVisible(false);
    barCount->setEnabled(TRUE);
  }
}

void ChartToolbar::paperTradeNextBar()
{
  if (!ptdAction->isVisible())
    return;

  // if (refreshTimer->isActive())
  //   ptDate = ptDate.addSecs(5);
  // else {
  switch (compressionCombo->currentItem())
    {
      case 0:
        ptDate = ptDate.addSecs(1);
        break;
      case 1:
      case 2:
      case 3:
      case 4:
        ptDate = ptDate.addSecs(5*compressionCombo->currentItem());
        break;
      case 5:
        ptDate = ptDate.addSecs(30);
        break;
      case 6:
        ptDate = ptDate.addSecs(45);
        break;
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
        ptDate = ptDate.addSecs((compressionCombo->currentItem()-6)*60);
        break;
      case 13:
        ptDate = ptDate.addSecs(600);
        break;
      case 14:
        ptDate = ptDate.addSecs(720);
        break;
      case 15:
        ptDate = ptDate.addSecs(900);
        break;
      case 17:
        ptDate = ptDate.addSecs(1800);
        break; 
      // case 15: // daily
      //   ptDate = ptDate.addDays(1);
      //   if (ptDate.date().dayOfWeek() == 6)
      //     ptDate = ptDate.addDays(2);
      //   else
      //   {
      //     if (ptDate.date().dayOfWeek() == 7)
      //       ptDate = ptDate.addDays(1);
      //   }
      //   break;
      case 29: // weekly
        ptDate = ptDate.addDays(7);
        break;
      case 30: // monthly
        ptDate = ptDate.addMonths(1);
        break;
      default: //default to daily
        ptDate = ptDate.addDays(1);
        if (ptDate.date().dayOfWeek() == 6)
          ptDate = ptDate.addDays(2);
        else
        {
          if (ptDate.date().dayOfWeek() == 7)
            ptDate = ptDate.addDays(1);
        }
        break;
    }

  emit signalPaperTradeNextBar();
}
