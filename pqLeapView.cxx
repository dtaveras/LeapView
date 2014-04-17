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
#include <QDebug>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>

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

}

//-----------------------------------------------------------------------------
pqLeapView::~pqLeapView()
{
  
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
  container->setStyleSheet("background-color: white");
  container->setAutoFillBackground(true);

  QVBoxLayout* vLayout = new QVBoxLayout(container);

  pqQVTKWidget* vtkwidget = new pqQVTKWidget();
  vtkwidget->setViewProxy(viewProxy);
  vtkwidget->SetRenderWindow(clientView->GetRenderWindow());
  vLayout->addWidget(vtkwidget);

  return container;
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
