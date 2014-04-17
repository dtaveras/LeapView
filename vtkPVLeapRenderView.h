#ifndef _VTKPVLEAP_RENDERVIEW
#define _VTKPVLEAP_RENDERVIEW

#include "vtkPVRenderView.h"

class vtkPVLeapRenderView : public vtkPVRenderView
{
public:
  static vtkPVLeapRenderView* New();
  vtkTypeMacro(vtkPVLeapRenderView, vtkPVRenderView);

  virtual void Update();
  virtual void SetBackground(double r, double g, double b);

protected:  
  vtkPVLeapRenderView();
  ~vtkPVLeapRenderView();
};

#endif
