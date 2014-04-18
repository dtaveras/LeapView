#include "vtkPVLeapRenderView.h"
#include "vtkObjectFactory.h"
#include "vtkNew.h"
#include "vtkTextActor.h"
#include "vtkRenderer.h"
#include "vtkTextProperty.h"

#include <QDebug>

vtkStandardNewMacro(vtkPVLeapRenderView);

vtkPVLeapRenderView::vtkPVLeapRenderView(){
  qDebug() << "Render View Constructor";
  this->SetOrientationAxesVisibility(false);
  
  vtkNew<vtkTextActor> text;
  text->SetInput("QUANTMD");
  text->SetPosition(10.0,10.0);
  text->GetTextProperty()->SetBold(1);
  text->GetTextProperty()->SetColor(1.0,0.0,0.0);
  this->GetNonCompositedRenderer()->AddActor(text.GetPointer());

}

vtkPVLeapRenderView::~vtkPVLeapRenderView(){

}

void vtkPVLeapRenderView::Update()
{
  this->Superclass::Update();
}

void vtkPVLeapRenderView::SetBackground(double r, double g, double b)
{
  this->Superclass::SetBackground(r,g,b);
}

