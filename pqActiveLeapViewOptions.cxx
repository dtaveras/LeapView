/*=========================================================================

=========================================================================*/

#include "pqActiveLeapViewOptions.h"

#include <QPointer>

#include "pqOptionsDialog.h"
#include "pqRenderViewOptions.h"

class pqActiveLeapViewOptions::pqInternal
{
public:
  QPointer<pqOptionsDialog> Dialog;
  pqRenderViewOptions* GeneralOptions;
};

pqActiveLeapViewOptions::pqActiveLeapViewOptions(QObject *parentObject)
  : pqActiveViewOptions(parentObject)
{
  this->Internal = new pqInternal;
}

pqActiveLeapViewOptions::~pqActiveLeapViewOptions()
{
  delete this->Internal;
}

void pqActiveLeapViewOptions::showOptions(
    pqView *view, const QString &page, QWidget *widgetParent)
{
  if(!this->Internal->Dialog)
    {
    this->Internal->Dialog = new pqOptionsDialog(widgetParent);
    this->Internal->Dialog->setApplyNeeded(true);
    this->Internal->Dialog->setObjectName("ActiveRenderViewOptions");
    this->Internal->Dialog->setWindowTitle("View Settings (Render View)");
    this->Internal->GeneralOptions = new pqRenderViewOptions;
    this->Internal->Dialog->addOptions(this->Internal->GeneralOptions);
    if(page.isEmpty())
      {
      QStringList pages = this->Internal->GeneralOptions->getPageList();
      if(pages.size())
        {
        this->Internal->Dialog->setCurrentPage(pages[0]);
        }
      }
    else
      {
      this->Internal->Dialog->setCurrentPage(page);
      }

    this->connect(this->Internal->Dialog, SIGNAL(finished(int)),
        this, SLOT(finishDialog()));
    }

  this->changeView(view);
  this->Internal->Dialog->show();
}

void pqActiveLeapViewOptions::changeView(pqView *view)
{
  this->Internal->GeneralOptions->setView(view);
}

void pqActiveLeapViewOptions::closeOptions()
{
  if(this->Internal->Dialog)
    {
    this->Internal->Dialog->accept();
    }
}

void pqActiveLeapViewOptions::finishDialog()
{
  emit this->optionsClosed(this);
}
