#ifndef _LEAPCONTROLLER_H
#define _LEAPCONTROLLER_H

#include <QObject>
#include "vtkLeapListener.h"
#include "pqLeapView.h"

class vtkLeapListener;
class pqLeapView;

using namespace Leap;

class vtkLeapController : public QObject
{
  Q_OBJECT

public:
  vtkLeapController();
  vtkLeapController(pqLeapView* view);
  ~vtkLeapController();
private:
  vtkLeapListener* leapListener;
  Leap::Controller* leapController;
  pqLeapView* view;

public:
  void newFrame2(Frame frame);
public slots:
  void newFrame(Frame frame);
};

#endif
