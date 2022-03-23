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

#include "TrendLine.h"
#include "PrefDialog.h"
#include "Config.h"
#include <qpainter.h>
//Added by qt3to4:
#include <Q3PointArray>
#include <QPixmap>

#include <qmath.h>
#include <qdebug.h>

TrendLine::TrendLine (int cmp)
{
  defaultColor.setNamedColor("red");
  usebar = TRUE;
  autobar = TRUE;
  extend = TRUE;
  fan = TRUE;
  bar = "High-Low";
  helpFile = "trendline.html";
  date2 = date;
  type = "TrendLine";

  sdateLabel = "Start Date";
  edateLabel = "End Date";
  svalueLabel = "Start Value";
  evalueLabel = "End Value";
  fieldLabel = "Bar Field";
  usebarLabel = "Use Bar";
  autobarLabel = "Auto Adjust Line";
  extendLabel = "Extend Line";
  fanLabel = "Draw Fan";

  interval = cmp;
  intervalLabel = "Bar Length";

  loadDefaults();
}

TrendLine::~TrendLine ()
{
}

void TrendLine::draw (QPixmap &buffer, Scaler &scaler, int startIndex, int pixelspace, int startX)
{
  QPainter painter;
  painter.begin(&buffer);

  int x2 = data->getX(date);
  if (x2 == -1)
    return;

  int x = startX + (x2 * pixelspace) - (startIndex * pixelspace);
  if (x == -1)
    return;

  x2 = data->getX(date2);
  if (x2 == -1)
    return;

  x2 = startX + (x2 * pixelspace) - (startIndex * pixelspace);
  if (x2 == -1)
    return;


  // int y;
  // float y2;
  int y = scaler.convertToY(getValue());
  double y2 = scaler.convertToY(getValue2());
  //float range = getValue2() - getValue();

  // painter.setPen(getColor());
  painter.setPen(QPen(getColor(), 2));
  // painter.drawLine (x, y, x2, y2);

  // save starting values;
  int tx = x;
  int tx2 = x2;
  int ty = y;
  int ty2 = y2;

  // // find the slope of the angle and output it
  // double rise = getValue2() - getValue();
  // double run = data->getX(date2) - data->getX(date);
  // // qDebug() << run;
  // emit message(tr("rise: " + QString::number(rise) + "  run: " + QString::number(run) + "  slope: " + QString::number((rise/run))));

  //make a vector containing the number one
  std::vector<double> angles(1,1);
  //try to make a gann fan
  if (getFan()) {
    // angles.push_back(0.015625);
    // angles.push_back(0.03125);
    // angles.push_back(0.0625);
    angles.push_back(0.125);
    angles.push_back(0.25);
    angles.push_back(0.3333);
    angles.push_back(0.5);
    angles.push_back(2);
    angles.push_back(3);
    angles.push_back(4);
    angles.push_back(8);
    // angles.push_back(16);
    // angles.push_back(-1);
    // angles.push_back(-0.0625);
    // angles.push_back(-0.125);
    // angles.push_back(-0.25);
    // angles.push_back(-0.5);
    // angles.push_back(-2);
    // angles.push_back(-4);
    // angles.push_back(-8);
    // angles.push_back(-16);
    // angles = {0.0625,0.125,0.25,0.5,1,2,4,8,16};
  }
  
  //use the vector created above to draw one line, or a fan
  for (float ang : angles)
  {
    x = tx;
    y = ty;
    x2 = tx2;
    y2 = (ty2-y) * ang + y;
    //y2 = scaler.convertToY(range * ang + getValue());
    painter.drawLine (x, y, x2, y2);

    if (getExtend())
    {
      if (ang == float(3) or ang == float(0.3333))
        painter.setPen(QPen(getColor(), 1, Qt::DotLine));
      else
        painter.setPen(QPen(getColor(), 1, Qt::DashDotDotLine));
      float ydiff = y - y2;
      int xdiff = x2 - x;
      while (x2 < buffer.width())
      {
        x = x2;
        y = y2;
        x2 = x2 + xdiff;
        y2 = y2 - ydiff;
        painter.drawLine (x, y, x2, y2);
      }
    }
  }


  // store the selectable area the line occupies
  clearSelectionArea();
  Q3PointArray array;
  // array.putPoints(0, 4, tx, ty - 4, tx, ty + 4, x2, y2 + 4, x2, y2 - 4);
  array.putPoints(0, 4, tx, ty - 4, tx, ty + 4, tx2, ty2 + 4, tx2, ty2 - 4);
  setSelectionArea(new QRegion(array));

  if (getStatus() == COBase::Selected)
  {
    clearGrabHandles();

    setGrabHandle(new QRegion(tx,
           	  ty - (HANDLE_WIDTH / 2),
		  HANDLE_WIDTH,
		  HANDLE_WIDTH,
		  QRegion::Rectangle));
    painter.fillRect(tx, ty - (HANDLE_WIDTH / 2), HANDLE_WIDTH, HANDLE_WIDTH, getColor());

    setGrabHandle(new QRegion(tx2,
             	  ty2 - (HANDLE_WIDTH / 2),
		  HANDLE_WIDTH,
		  HANDLE_WIDTH,
		  QRegion::Rectangle));
    painter.fillRect(tx2, ty2 - (HANDLE_WIDTH / 2), HANDLE_WIDTH, HANDLE_WIDTH, getColor());
  }

  painter.end();
}

void TrendLine::prefDialog ()
{
  QStringList l;
  l.append(tr("High-Low"));
  l.append(tr("High-High"));
  l.append(tr("Low-High"));
  l.append(tr("Low-Low"));

  QString pl = tr("Details");
  QString cl = tr("Color");
  QString sd = tr("Set Default");
  QString ub = tr("Use Bars");
  QString ab = tr("Auto Adjust Line");
  QString el = tr("Extend Line");
  QString fl = tr("Draw Fan");
  QString bf = tr("Bar Field");
  QString sl = tr("Start Value");
  QString dl = tr("End Value");

  PrefDialog *dialog = new PrefDialog();
  dialog->setCaption(tr("Edit TrendLine"));
  dialog->createPage (pl);
  dialog->setHelpFile (helpFile);
  dialog->addColorPrefItem(cl, pl, color);
  dialog->addComboItem(bf, pl, l, bar);
  dialog->addCheckItem(ub, pl, usebar);
  dialog->addCheckItem(ab, pl, autobar);
  dialog->addCheckItem(fl, pl, fan);
  dialog->addCheckItem(el, pl, extend);
  dialog->addDoubleItem(sl, pl, getValue());
  dialog->addDoubleItem(dl, pl, getValue2());
  dialog->addCheckItem(sd, pl, FALSE);

  // //store values so we can check if they change
  // bool ab_old = autobar;
  // bool ub_old = usebar;
  // QString bf_old = bar;

  int rc = dialog->exec();

  if (rc == QDialog::Accepted)
  {
    dialog->getColor(cl, color);
    dialog->getCombo(bf, bar);
    usebar = dialog->getCheck(ub);
    autobar = dialog->getCheck(ab);
    extend = dialog->getCheck(el);
    fan = dialog->getCheck(fl);

    // //adjust values if autobar or usebar have changed
    // if ((usebar != ub_old) | (autobar != ab_old) | (bar != bf_old))
    // {
    if (autobar) {
      if (getValue2() <= getValue()) //going down
      {
        setValue(data->getHigh(data->getX(date)));
        setValue2(data->getLow(data->getX(date2)));
      }
      else //going up
      {
        setValue(data->getLow(data->getX(date)));
        setValue2(data->getHigh(data->getX(date2)));
      }
    }
    else if (usebar)
    {
      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("High-High")))
        setValue(data->getHigh(data->getX(date)));
      else if (! bar.compare(tr("Low-Low")) | ! bar.compare(tr("Low-High")))
        setValue(data->getLow(data->getX(date)));

      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("Low-Low")))
        setValue2(data->getLow(data->getX(date2)));
      else if (! bar.compare(tr("High-High")) | ! bar.compare(tr("Low-High")))
        setValue2(data->getHigh(data->getX(date2)));
    }
    else
    {
      setValue(dialog->getDouble(sl));
      setValue2(dialog->getDouble(dl));
    }
    // }


    setSaveFlag(TRUE);

    bool f = dialog->getCheck(sd);
    if (f)
    {
      dialog->getColor(cl, defaultColor);
      dialog->getCombo(bf, bar);
      usebar = dialog->getCheck(ub);
      autobar = dialog->getCheck(ab);
      extend = dialog->getCheck(el);
      fan = dialog->getCheck(fl);
      saveDefaults();
    }

    emit signalDraw();
  }

  delete dialog;
}

void TrendLine::newObject (QString &ind, QString &n)
{
  indicator = ind;
  plot = ind;
  name = n;
  mpx2 = -1;
  mpy2 = -1;
  status = ClickWait;
  emit message(tr("Select TrendLine starting point..."));
}

COBase::Status TrendLine::pointerClick (QPoint &point, QDateTime &x, double y)
{
  switch (status)
  {
    case None:
      if (isSelected(point))
      {
        status = Selected;
        // ty = y;
        emit signalDraw();
      }
      break;
    case Selected:
      moveFlag = isGrabSelected(point);
      if (moveFlag)
        status = Moving;
      else
      {
        if (! isSelected(point))
        {
          status = None;
          emit signalDraw();
        }
      }
      break;
    case Moving:
      status = Selected;
      break;
    case ClickWait: //first click, adding new trendline
      tx = x;
      ty = y;
      mpx = point.x();
      mpy = point.y();
      status = ClickWait2;
      emit message(tr("Select TrendLine ending point..."));
      break;
    case ClickWait2:
      if (x <= tx)
        break;

      setDate(tx);
      setDate2(x);

      setValue(ty);
      //check for autobar
      // setValue2(y);

      setSaveFlag(TRUE);
      setColor(defaultColor);
      emit signalDraw();
      status = None;
      emit message("");
      emit signalSave(name);
      break;
    default:
      break;
  }

  return status;
}

void TrendLine::pointerMoving (QPixmap &buffer, QPoint &point, QDateTime &x, double y)
{
  if (status == ClickWait2) //if this is the same as Fiboline, this is for when selecting second point for first time
  {
    double cursorVal = y;

    //find the angle of the line we are drawing based on pixels...
    // int dx = point.x()-mpx;
    // int dy = mpy-point.y();
    // double axy = 90-qAtan2(dx,dy)*180/3.14152;
    // // qDebug() << ("x: " + QString::number(dx) + "  y: " + QString::number(dy) + "  angle: " + QString::number(axy));// + "  slope: " + QString::number((rise/run))));
    // emit message(tr("x: " + QString::number(dx) + "  y: " + QString::number(dy) + "  angle: " + QString::number(axy)));


    // //use bar values
    // if (getAutoBar()) {
    //   if (y <= ty) //going down
    //   {
    //     ty = data->getHigh(data->getX(tx));
    //     y = data->getLow(data->getX(x));
    //   }
    //   else //going up
    //   {
    //     ty = data->getLow(data->getX(tx));
    //     y = data->getHigh(data->getX(x));
    //   }

    // try to use high or low based on which is closer
    if (getAutoBar()) {
      if (y <= ty) //going down
        ty = data->getHigh(data->getX(tx));
      else //going up
        ty = data->getLow(data->getX(tx));

      double l = data->getLow(data->getX(x));
      double h = data->getHigh(data->getX(x));
      if (((l-cursorVal)*(l-cursorVal))>((h-cursorVal)*(h-cursorVal)))
        y = h;
      else
        y = l;

      //but now we also have to adjust bar field and autobar accordingly....
      //...

      // point.setY(y);
      setValue2(y);
      setValue(ty);
    }

    drawMovingPointer(buffer, point);
    return;
  }

  if (! moveFlag || status != Moving)
    return;

  if (moveFlag == 1) //moving start point
  {
    if (x >= date2)
      return;

    double y2;
    //use bar values
    if (getAutoBar()) {
      if (y >= getValue2()) //going down
      {
        y = data->getHigh(data->getX(date));
        y2 = data->getLow(data->getX(date2));
      }
      else //going up
      {
        y = data->getLow(data->getX(date));
        y2 = data->getHigh(data->getX(date2));
      }

      setValue2(y2);
  
    } else if (getUseBar())
    {
      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("High-High")))
        y = data->getHigh(data->getX(x));
      else if (! bar.compare(tr("Low-Low")) | ! bar.compare(tr("Low-High")))
        y = data->getLow(data->getX(x));

      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("Low-Low")))
        y2 = data->getLow(data->getX(date2));
      else if (! bar.compare(tr("High-High")) | ! bar.compare(tr("Low-High")))
        y2 = data->getHigh(data->getX(date2));

      setValue2(y2);
    }

    setDate(x);
    setValue(y);
    setSaveFlag(TRUE);
    emit signalDraw();
    QString s = x.toString("yyyy-MM-dd hh:mm:ss") + " " + QString::number(y);
    emit message(s);
  }
  else //moving end point (Value2)
  {
    if (x <= date)
      return;

    double y2; //holding place for y value of start point
    //use bar values
    if (getAutoBar()) {
      if (y <= getValue()) //going down
      {
        y2 = data->getHigh(data->getX(date));
        y = data->getLow(data->getX(date2));
      }
      else //going up
      {
        y2 = data->getLow(data->getX(date));
        y = data->getHigh(data->getX(date2));
      }
      // point.setY(y);
      setValue(y2);
    
    } else if (getUseBar())
    {
      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("Low-Low")))
        y = data->getLow(data->getX(x));
      else if (! bar.compare(tr("High-High")) | ! bar.compare(tr("Low-High")))
        y = data->getHigh(data->getX(x));

      if (! bar.compare(tr("High-Low")) | ! bar.compare(tr("High-High")))
        y2 = data->getHigh(data->getX(date));
      else if (! bar.compare(tr("Low-Low")) | ! bar.compare(tr("Low-High")))
        y2 = data->getLow(data->getX(date));

      setValue(y2);
    }

    setDate2(x);
    setValue2(y);
    setSaveFlag(TRUE);
    emit signalDraw();
    QString s = x.toString("yyyy-MM-dd hh:mm:ss") + " " + QString::number(y);
    emit message(s);
  }
}

void TrendLine::drawMovingPointer (QPixmap &buffer, QPoint &point)
{
  if (point.x() < mpx)
    return;

  QPainter painter;
  painter.begin(&buffer);
  // FIXME: qt3to4: not sure what to do with this
  // painter.setRasterOp(Qt::XorROP);

  // erase the previous line drawn by drawing over it with black???
  painter.setPen(Qt::black);
  if (mpx2 != -1 && mpy2 != -1)
    painter.drawLine (mpx, mpy, mpx2, mpy2);

  painter.setPen(defaultColor);

  // draw the new line
  painter.drawLine (mpx, mpy, point.x(), point.y());

  mpx2 = point.x();
  mpy2 = point.y();

  painter.end();

  emit signalRefresh();
}

void TrendLine::loadDefaults ()
{
  Config settings;
  QString s2 = "DefaultTrendLineColor";
  QString s;
  settings.getData(s2, s);
  if (s.length())
    defaultColor.setNamedColor(s);

  s2 = "DefaultTrendLineBar";
  settings.getData(s2, s);
  if (s.length())
    bar = s;

  s2 = "DefaultTrendLineAutoBar";
  settings.getData(s2, s);
  if (s.length())
    autobar = s.toInt();

  s2 = "DefaultTrendLineExtend";
  settings.getData(s2, s);
  if (s.length())
    extend = s.toInt();

  s2 = "DefaultTrendLineFan";
  settings.getData(s2, s);
  if (s.length())
    fan = s.toInt();

  s2 = "DefaultTrendLineUseBar";
  settings.getData(s2, s);
  if (s.length())
    usebar = s.toInt();
}

void TrendLine::saveDefaults ()
{
  Config config;

  QString s = "DefaultTrendLineColor";
  QString s2 = defaultColor.name();
  config.setData(s, s2);

  s = "DefaultTrendLineBar";
  config.setData(s, bar);

  s = "DefaultTrendLineExtend";
  s2 = QString::number(extend);
  config.setData(s, s2);

  s = "DefaultTrendLineFan";
  s2 = QString::number(fan);
  config.setData(s, s2);

  s = "DefaultTrendLineAutoBar";
  s2 = QString::number(autobar);
  config.setData(s, s2);

  s = "DefaultTrendLineUseBar";
  s2 = QString::number(usebar);
  config.setData(s, s2);
}

double TrendLine::getHigh ()
{
  double high = -99999999.0;

  if (getValue() > high)
      high = getValue();
  if (getValue2() > high)
      high = getValue2();

  return high;
}

double TrendLine::getLow ()
{
  double low = 99999999.0;

  if (getValue() < low)
    low = getValue();
  if (getValue2() < low)
    low = getValue2();

  return low;
}

void TrendLine::setDate2 (QDateTime &d)
{
  date2 = d;
}

void TrendLine::getDate2 (QDateTime &d)
{
  d = date2;
}

void TrendLine::setValue2 (double d)
{
  value2 = d;
}

double TrendLine::getValue2 ()
{
  return value2;
}

int TrendLine::isGrabSelected (QPoint point)
{
  int loop;
  for (loop = 0; loop < (int) grabHandles.count(); loop++)
  {
    QRegion *r = grabHandles.at(loop);
    if (r->contains(point))
      return loop + 1;
  }

  return 0;
}

void TrendLine::getSettings (Setting &set)
{
  QString s = date.toString(dateFormat);
  set.setData(sdateLabel, s);
  s = date2.toString(dateFormat);
  set.setData(edateLabel, s);
  s = QString::number(value);
  set.setData(svalueLabel, s);
  s = QString::number(value2);
  set.setData(evalueLabel, s);
  set.setData(fieldLabel, bar);
  s = QString::number(usebar);
  set.setData(usebarLabel, s);
  s = QString::number(autobar);
  set.setData(autobarLabel, s);
  s = QString::number(extend);
  set.setData(extendLabel, s);
  s = QString::number(fan);
  set.setData(fanLabel, s);
  s = QString::number(interval);
  set.setData(intervalLabel, s);
  s = color.name();
  set.setData(colorLabel, s);
  set.setData(plotLabel, plot);
  set.setData(nameLabel, name);
  set.setData(typeLabel, type);
}

void TrendLine::setSettings (Setting &set)
{
  QString s;
  set.getData(colorLabel, s);
  color.setNamedColor(s);
  set.getData(sdateLabel, s);
  Bar b;
  b.setDate(s);
  b.getDate(date);
  set.getData(edateLabel, s);
  b.setDate(s);
  b.getDate(date2);
  value = set.getDouble(svalueLabel);
  value2 = set.getDouble(evalueLabel);
  set.getData(fieldLabel, bar);
  usebar = set.getInt(usebarLabel);
  autobar = set.getInt(autobarLabel);
  extend = set.getInt(extendLabel);
  interval = set.getInt(intervalLabel);
  fan = set.getInt(fanLabel);
  set.getData(plotLabel, plot);
  set.getData(nameLabel, name);
}

bool TrendLine::getUseBar ()
{
  return usebar;
}

bool TrendLine::getAutoBar ()
{
  return autobar;
}

void TrendLine::getBar (QString &d)
{
  d = bar;
}

bool TrendLine::getExtend ()
{
  return extend;
}

bool TrendLine::getFan ()
{
  return fan;
}

void TrendLine::adjustForSplit (QDateTime &dt, double d)
{
  if (date < dt)
    value = value * d;
  if (date2 < dt)
    value2 = value2 * d;
}
