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

#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include <qcolor.h>
#include <qfont.h>
#include <qstringlist.h>
#include <qstring.h>
#include <q3tabdialog.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qsize.h>
#include "ColorButton.h"
#include "FontButton.h"


class Preferences : public Q3TabDialog
{
  Q_OBJECT

  signals:
    void signalMenubar(bool);
    void signalExtraToolbar(bool);
    void signalBackgroundColor(QColor);
    void signalBorderColor(QColor);
    void signalGridColor(QColor);
    void signalPlotFont(QFont);
    void signalAppFont(QFont);
    void signalLoadChart();
    void signalReloadToolBars();

  public:
    Preferences(QWidget *);
    ~Preferences();
    void createGeneralPage();
    void createColorPage();
    void createFontPage();
    void createMTPage(); // main tool bar
    void createCTPage(); // chart tool bar
    void createETPage(); // extra tool bar
    void loadSettings();

  public slots:
    void slotHelp();
    void slotSave();
    void slotModified();

  private:
    QSize sz;
    QString helpFile;
    bool menubar;
    bool extraToolbar;
    int ps1Button;
    int ps2Button;
    int ps3Button;
    QColor backgroundColor;
    QColor borderColor;
    QColor gridColor;
    QFont plotFont;
    QFont appFont;
    QCheckBox *menubarCheck;
    QCheckBox *extraToolbarCheck;
    QSpinBox *bs1Spinner;
    QSpinBox *bs2Spinner;
    QSpinBox *bs3Spinner;
    ColorButton *backgroundColorButton;
    ColorButton *borderColorButton;
    ColorButton *gridColorButton;
    FontButton *plotFontButton;
    FontButton *appFontButton;
    // on MainToolbar
    QCheckBox *quitBtnCheck;
    QCheckBox *prefBtnCheck;
    QCheckBox *sidePanelBtnCheck;
    QCheckBox *gridBtnCheck;
    QCheckBox *scaleToScreenBtnCheck;
    QCheckBox *crosshairBtnCheck;
    QCheckBox *paperTradeBtnCheck;
    QCheckBox *drawModeBtnCheck;
    QCheckBox *newIndicatorBtnCheck;
    QCheckBox *dataWindowBtnCheck;
    QCheckBox *mainQuoteBtnCheck;
    QCheckBox *helpButtonCheck;
    // on ChartToolbar
    QCheckBox *sliderCheck;
    QCheckBox *barsToLoadFieldCheck;
    QCheckBox *barSpSpinboxCheck;
    QCheckBox *cmpsCustomBarBtnCheck;
    QCheckBox *cmps5SBtnCheck;
    QCheckBox *cmps10SBtnCheck;
    QCheckBox *cmps15SBtnCheck;
    QCheckBox *cmps20SBtnCheck;
    QCheckBox *cmps30SBtnCheck;
    QCheckBox *cmps45SBtnCheck;
    QCheckBox *cmps1MBtnCheck;
    QCheckBox *cmps2MBtnCheck;
    QCheckBox *cmps3MBtnCheck;
    QCheckBox *cmps4MBtnCheck;
    QCheckBox *cmps5MBtnCheck;
    QCheckBox *cmps6MBtnCheck;
    QCheckBox *cmps10MBtnCheck;
    QCheckBox *cmps12MBtnCheck;
    QCheckBox *cmps15MBtnCheck;
    QCheckBox *cmps20MBtnCheck;
    QCheckBox *cmps30MBtnCheck;
    QCheckBox *cmps45MBtnCheck;
    QCheckBox *cmps1HBtnCheck;
    QCheckBox *cmps2HBtnCheck;
    QCheckBox *cmps3HBtnCheck;
    QCheckBox *cmps4HBtnCheck;
    QCheckBox *cmps6HBtnCheck;
    QCheckBox *cmps8HBtnCheck;
    QCheckBox *cmps12HBtnCheck;
    QCheckBox *cmps1DBtnCheck;
    QCheckBox *cmps2DBtnCheck;
    QCheckBox *cmps3DBtnCheck;
    QCheckBox *cmps1WBtnCheck;
    QCheckBox *cmpsMNBtnCheck;
    QCheckBox *cmps2DwBtnCheck;
    QCheckBox *cmps3DwBtnCheck;
    QCheckBox *cmps5DwBtnCheck;
    QCheckBox *cmps8DwBtnCheck;
    QCheckBox *cmpsComboBoxCheck;
    QCheckBox *recentComboBoxCheck;
};

#endif
