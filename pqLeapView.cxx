/*=========================================================================

========================================================================*/
#include "pqLeapView.h"

#include "pqCoreUtilities.h"
#include "pqProxy.h"
#include "pqQVTKWidget.h"
#include "pqRepresentation.h"
#include "pqUndoStack.h"
#include "vtkPVDataInformation.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMRepresentationProxy.h"
#include "vtkSMViewProxy.h"
#include "vtkPVRenderView.h"
#include "vtkCamera.h"
#include "vtkMatrix4x4.h"

#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>

#define L_BOUND 0.020
#define U_BOUND 4.000
#define ROT_SCL .2
#define CHECK_BOUNDS_P(P) ((U_BOUND > P) && (P > L_BOUND))
#define CHECK_BOUNDS_N(P) ((-U_BOUND < P) && (P < -L_BOUND))

namespace
{
  /// override QWidget to update the "ViewSize" property whenever the widget
  /// resizes.
  class pqSizableWidget : public QWidget
  {
  typedef QWidget Superclass;
  vtkWeakPointer<vtkSMProxy> Proxy;
public:
  pqSizableWidget(vtkSMProxy* proxy) : Proxy(proxy) { }
protected:
  virtual void resizeEvent(QResizeEvent* evt)
    {
    this->Superclass::resizeEvent(evt);
    if (this->Proxy)
      {
      BEGIN_UNDO_EXCLUDE();
      int view_size[2];
      view_size[0] = this->size().width();
      view_size[1] = this->size().height();
      vtkSMPropertyHelper(this->Proxy, "ViewSize").Set(view_size, 2);
      this->Proxy->UpdateProperty("ViewSize");
      END_UNDO_EXCLUDE();
      }
    }
  };
}

//-----------------------------------------------------------------------------
pqLeapView::pqLeapView(
   const QString& viewType, const QString& group, const QString& name,
    vtkSMViewProxy* viewProxy, pqServer* server, QObject* p)
  : Superclass(viewType, group, name, viewProxy, server, p)
{
  qDebug() << "pqLeapView Constructor Initializing Leap";
  leap = new vtkLeapController(this);
  this->start = 0;
  this->gest = 0;
  this->axis = 0;
  this->camObj = 1;
}

//-----------------------------------------------------------------------------
pqLeapView::~pqLeapView()
{
  delete this->leap;
}

//-----------------------------------------------------------------------------
QWidget* pqLeapView::createWidget()
{
  qDebug() << "Creating 3D View Widget";
  vtkSMProxy* viewProxy = this->getProxy();
  vtkPVRenderView* clientView = vtkPVRenderView::SafeDownCast(
    viewProxy->GetClientSideObject());

  QWidget* container = new pqSizableWidget(viewProxy);
  container->setObjectName("QuadView");
  //#373247
  container->setStyleSheet("background-color: #D0CFD1");
  container->setAutoFillBackground(true);

  QVBoxLayout* vLayout = new QVBoxLayout(container);

  QPushButton *button = new QPushButton("Yaw");
  connect(button, SIGNAL(clicked()), this, SLOT(changeGestureY()) );
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->addWidget(button);

  button = new QPushButton("Roll");
  connect(button, SIGNAL(clicked()), this, SLOT(changeGestureR()) );
  hLayout->addWidget(button);

  button = new QPushButton("Pitch");
  connect(button, SIGNAL(clicked()), this, SLOT(changeGestureP()) );
  hLayout->addWidget(button);

  button = new QPushButton("Change Rotation Axis");
  connect(button, SIGNAL(clicked()), this, SLOT(changeAxis()) );
  hLayout->addWidget(button);

  button = new QPushButton("Toggle Camera/Object ");
  connect(button, SIGNAL(clicked()), this, SLOT(toggleCam()) );
  hLayout->addWidget(button);

  pqQVTKWidget* vtkwidget = new pqQVTKWidget();
  vtkwidget->setViewProxy(viewProxy);
  vtkwidget->SetRenderWindow(clientView->GetRenderWindow());
  vLayout->addLayout(hLayout);

  vLayout->addWidget(vtkwidget);

  vtkCamera* clientCamera = clientView->GetActiveCamera();
  return container;
}

void pqLeapView::toggleCam(){
  this->camObj = this->camObj ? 0 : 1;
}

void pqLeapView::changeAxis(){
  this->axis = this->axis < 2 ? this->axis+1 : 0;
}

void pqLeapView::changeGestureY(){
  this->gest = 0;  
}

void pqLeapView::changeGestureR(){
  this->gest = 1;  
}

void pqLeapView::changeGestureP(){
  this->gest = 2;  
}

void pqLeapView::newFrame(Frame frame){
  qDebug() << "Frame id: " << frame.id()
              << ", timestamp: " << frame.timestamp()
              << ", hands: " << frame.hands().count()
              << ", fingers: " << frame.fingers().count()
              << ", tools: " << frame.tools().count()
	      << ", gestures: " << frame.gestures().count();
  qDebug() << "Framerate: " << frame.currentFramesPerSecond();
  qDebug() << "-----------------------------------------------------------";

  vtkSMProxy* viewProxy = this->getProxy();
  vtkPVRenderView* clientView = vtkPVRenderView::SafeDownCast(
    viewProxy->GetClientSideObject());
  vtkCamera* clientCamera = clientView->GetActiveCamera();
  
  //When we first start just save the frame
  if(this->start == 0){
    this->start = 1;
    this->prevframe = frame;
    return;
  }


  //clientCamera->SetEyeTransformMatrix(resMat);
  /*for(int j=0; j< 4; j++){
    qDebug() << rotMat->Element[j][0] << rotMat->Element[j][1]
	     << rotMat->Element[j][2] << rotMat->Element[j][3];
	     }*/
  
  double* camPos = clientCamera->GetPosition();
  double curPalmPos = frame.hands()[0].palmPosition().y;
  double prvPalmPos = prevframe.hands()[0].palmPosition().y;
  double chngPos = curPalmPos - prvPalmPos;
  if( CHECK_BOUNDS_P(chngPos) || CHECK_BOUNDS_N(chngPos) ){
    if(this->camObj == 0){
      if(this->gest == 0){
	clientCamera->Yaw(chngPos);
      }
      else if(this->gest == 1){
	clientCamera->Roll(chngPos);
      }
      else if(this->gest == 2){
	clientCamera->Pitch(chngPos);
      }
    }
    else if(this->camObj == 1){
      double arr[16];
      Leap::Matrix mat;
      if(this->axis == 0){
	mat.setRotation(Leap::Vector::xAxis(), chngPos*ROT_SCL);
      }
      else if(this->axis == 1){
	mat.setRotation(Leap::Vector::yAxis(), chngPos*ROT_SCL);
      }
      else if(this->axis == 2){
	mat.setRotation(Leap::Vector::zAxis(), chngPos*ROT_SCL);
      }
      mat.toArray4x4(arr);
      vtkMatrix4x4* rotMat = vtkMatrix4x4::New();
      rotMat->DeepCopy(arr);
      vtkMatrix4x4* mdMat = clientCamera->GetModelTransformMatrix();
      vtkMatrix4x4* resMat = vtkMatrix4x4::New();
      vtkMatrix4x4::Multiply4x4(rotMat,mdMat,rotMat);
      clientCamera->SetModelTransformMatrix(rotMat);
    }
  }

  qDebug() << "curPalmPos:" << curPalmPos;
  qDebug() << "prvPalmPos:" << prvPalmPos;
  qDebug() << "Position Change:" << chngPos;
  //qDebug() <<"Palm Position:"<< palmPos[0]<<" "<< palmPos[1]<<" "<< palmPos[2];
  
  //Save frame
  this->prevframe = frame;
  this->render();
}

//-----------------------------------------------------------------------------
void pqLeapView::resetDefaultSettings()
{

}

//-----------------------------------------------------------------------------
void pqLeapView::setCursor(const QCursor &c)
{

}

//-----------------------------------------------------------------------------
bool pqLeapView::getCubeAxesVisibility()
{
  return vtkSMPropertyHelper(this->getViewProxy(), "ShowCubeAxes").GetAsInt() != 0;
}

//-----------------------------------------------------------------------------
void pqLeapView::setCubeAxesVisibility(bool visible)
{
  vtkSMPropertyHelper(this->getViewProxy(), "ShowCubeAxes").Set(visible ? 1 : 0);
  this->getViewProxy()->UpdateVTKObjects();
}

//-----------------------------------------------------------------------------
bool pqLeapView::getOutlineVisibility()
{
  return vtkSMPropertyHelper(this->getViewProxy(), "ShowOutline").GetAsInt() != 0;
}

//-----------------------------------------------------------------------------
void pqLeapView::setOutlineVisibility(bool visible)
{
  vtkSMPropertyHelper(this->getViewProxy(), "ShowOutline").Set(visible ? 1 : 0);
  this->getViewProxy()->UpdateVTKObjects();
}
