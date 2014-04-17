/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#ifndef __pqLeapView_h
#define __pqLeapView_h

#include "pqRenderView.h"

class pqLeapView : public pqRenderView
{
  Q_OBJECT
  typedef pqRenderView Superclass;
public:
  static QString pqLeapViewType() { return "QuadView"; }
  static QString pqLeapViewTypeName() { return "Quad View";}

  pqLeapView(const QString& viewtype,
             const QString& group,
             const QString& name,
             vtkSMViewProxy* viewmodule,
             pqServer* server,
             QObject* p);
  virtual ~pqLeapView();



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
