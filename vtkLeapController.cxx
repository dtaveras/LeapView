#include "vtkLeapController.h"
#include <QDebug>

vtkLeapController::vtkLeapController()
{
  qDebug() << "vtkLeapController Constructor";
  this->leapController = new Leap::Controller();
  this->leapListener = new vtkLeapListener(this);
  this->leapController->addListener(*this->leapListener);
}

vtkLeapController::vtkLeapController(pqLeapView* view)
{
  qDebug() << "vtkLeapController pqQuadView Constructor";
  this->view = view;
  this->leapController = new Leap::Controller();
  this->leapListener = new vtkLeapListener(view);
  this->leapController->addListener(*this->leapListener);
}

vtkLeapController::~vtkLeapController()
{
  this->leapController->removeListener(*this->leapListener);
  delete this->leapListener;
  delete this->leapController;
}

void vtkLeapController::newFrame2(Frame frame){
  //qDebug() << "Using Function Call";
}

void vtkLeapController::newFrame(Frame frame){
  qDebug() << "Received Signal from Listener";
}
