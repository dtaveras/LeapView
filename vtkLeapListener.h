#ifndef _LEAPLISTENER_H
#define _LEAPLISTENER_H

#include <iostream>
#include <QObject>
#include "Leap/Include/Leap.h"
#include "vtkLeapController.h"
#include "pqLeapView.h"
#include <QMetaType>

class vtkLeapController;
class pqLeapView;

using namespace Leap;

class vtkLeapListener : public QObject, public Leap::Listener
{
  Q_OBJECT
public:
  vtkLeapListener();
  vtkLeapListener(vtkLeapController* leap);
  vtkLeapListener(pqLeapView* view);
  void onInit(const Leap::Controller&);
  void onConnect(const Leap::Controller&);
  void onDisconnect(const Leap::Controller&);
  void onExit(const Leap::Controller&);
  void onFrame(const Leap::Controller&);
  void onFocusGained(const Leap::Controller&);
  void onFocusLost(const Leap::Controller&);
private:
  vtkLeapController* leapController;
  pqLeapView* view;
signals:
  void frameChanged(Frame frame);
};
Q_DECLARE_METATYPE(Frame);

#endif
