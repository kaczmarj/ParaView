/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPVProbe.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1998-1999 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
// .NAME vtkPVProbe - A class to handle the UI for vtkProbeFilter
// .SECTION Description


#ifndef __vtkPVProbe_h
#define __vtkPVProbe_h

#include "vtkPVSource.h"
#include "vtkKWPushButton.h"
#include "vtkKWSelectPointInteractor.h"
#include "vtkKWLabeledEntry.h"

class VTK_EXPORT vtkPVProbe : public vtkPVSource
{
public:
  static vtkPVProbe* New();
  vtkTypeMacro(vtkPVProbe, vtkPVSource);
    
  // Description:
  // Set up the UI for this source
  void CreateProperties();

  // Description:
  // Set the interactor to use for choosing a point for probing.
  void SetInteractor();
  
  // Description:
  // Set/Get the selected point.
  vtkSetVector3Macro(SelectedPoint, float);
  vtkGetVector3Macro(SelectedPoint, float);
  
  // Description:
  // Called when the accept button is clicked.
  void UpdateProbe();

  // Description:
  // Methods to call when this pv source is selected/deselected
  void Deselect(vtkKWView *view);

  // Description:
  // Set/Get the ProbeSourceTclName
  vtkSetStringMacro(ProbeSourceTclName);
  vtkGetStringMacro(ProbeSourceTclName);
  
  // Description:
  // Called when the accept button is pressed.
  virtual void AcceptCallback();
  
  // Description:
  // Called when the delete button is pressed.
  virtual void DeleteCallback();
  
protected:
  vtkPVProbe();
  ~vtkPVProbe();
  vtkPVProbe(const vtkPVProbe&) {};
  void operator=(const vtkPVProbe&) {};

  char *ProbeSourceTclName;
  
  vtkKWLabel *SelectedPointLabel;
  vtkKWLabel *PointDataLabel;
  
  vtkKWPushButton *SelectPointsButton;
  
  vtkKWSelectPointInteractor *Interactor;
  
  float SelectedPoint[3];
};

#endif
