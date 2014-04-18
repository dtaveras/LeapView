/*=========================================================================

========================================================================*/
#ifndef __pqLeapView_h
#define __pqLeapView_h

#include "pqRenderView.h"
#include "vtkLeapController.h"
#include "vtkLeapListener.h"

using namespace Leap;

class vtkLeapController;
class pqLeapView : public pqRenderView
{
  Q_OBJECT
  typedef pqRenderView Superclass;
public:
  static QString pqLeapViewType() { return "LeapView"; }
  static QString pqLeapViewTypeName() { return "Leap View";}

  pqLeapView(const QString& viewtype,
             const QString& group,
             const QString& name,
             vtkSMViewProxy* viewmodule,
             pqServer* server,
             QObject* p);
  virtual ~pqLeapView();

  //LEAP MOTION Variables
  vtkLeapController* leap;
  Frame prevframe;
  int gest, start;
  int axis;
  int camObj;
public slots:
  void newFrame(Frame frame);
  void changeGestureY();
  void changeGestureR();
  void changeGestureP();
  void changeAxis();
  void toggleCam();
protected:  
  /// Reset Normal/ViewUp/Origin to their defaults
  void resetDefaultSettings();

  /// Override for custom management
  virtual void setCursor(const QCursor &);

  bool getCubeAxesVisibility();
  void setCubeAxesVisibility(bool visible);

  /// Update Slice CubeAxes visibility
  bool getOutlineVisibility();
  void setOutlineVisibility(bool visible);

protected:
  /// Override to decorate the QVTKWidget
  virtual QWidget* createWidget();

  // Store the property information and return the pointer to that location
  const double* getVector(const char* propertyName, int offset);
  const double* setVector(const char* propertyName, int offset, double x, double y, double z);
  double DataHolder[21];
  unsigned long ObserverId;

private:
  Q_DISABLE_COPY(pqLeapView)
};

#endif
