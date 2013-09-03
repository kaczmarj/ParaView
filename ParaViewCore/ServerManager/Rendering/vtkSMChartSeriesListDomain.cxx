/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile$

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMChartSeriesListDomain.h"

#include "vtkObjectFactory.h"
#include "vtkPVArrayInformation.h"
#include "vtkPVDataInformation.h"
#include "vtkPVDataSetAttributesInformation.h"
#include "vtkPVXMLElement.h"
#include "vtkSMArrayListDomain.h"
#include "vtkSMSourceProxy.h"
#include "vtkSMUncheckedPropertyHelper.h"
#include "vtkStdString.h"

#include <vector>
#include <algorithm>
#include <assert.h>

vtkStandardNewMacro(vtkSMChartSeriesListDomain);
//----------------------------------------------------------------------------
vtkSMChartSeriesListDomain::vtkSMChartSeriesListDomain()
{
  this->HidePartialArrays = true;
}

//----------------------------------------------------------------------------
vtkSMChartSeriesListDomain::~vtkSMChartSeriesListDomain()
{
}

//----------------------------------------------------------------------------
void vtkSMChartSeriesListDomain::Update(vtkSMProperty*)
{
  vtkSMProperty* input = this->GetRequiredProperty("Input");
  vtkSMProperty* fieldDataSelection =
    this->GetRequiredProperty("FieldDataSelection");

  if (!input || !fieldDataSelection)
    {
    vtkWarningMacro("Missing required properties. Update failed.");
    return;
    }


  // build strings based on the current domain.
  vtkPVDataInformation* dataInfo = this->GetInputInformation();
  if (!dataInfo)
    {
    return;
    }

  std::vector<vtkStdString> strings;
  int fieldAssociation =
    vtkSMUncheckedPropertyHelper(fieldDataSelection).GetAsInt(0);
  vtkPVDataSetAttributesInformation* dsa =
    dataInfo->GetAttributeInformation(fieldAssociation);

  for (int cc=0; dsa != NULL && cc < dsa->GetNumberOfArrays(); cc++)
    {
    vtkPVArrayInformation* arrayInfo = dsa->GetArrayInformation(cc);
    if (arrayInfo &&
      (this->HidePartialArrays == false || arrayInfo->GetIsPartial() == 0))
      {
      if (arrayInfo->GetNumberOfComponents() > 1)
        {
        for (int kk=0; kk <= arrayInfo->GetNumberOfComponents(); kk++)
          {
          strings.push_back(vtkSMArrayListDomain::CreateMangledName(
              arrayInfo, kk));
          }
        }
      else
        {
        strings.push_back(arrayInfo->GetName());
        }
      }
    }
  this->SetStrings(strings);
}

//----------------------------------------------------------------------------
vtkPVDataInformation* vtkSMChartSeriesListDomain::GetInputInformation()
{
  vtkSMProperty* inputProperty = this->GetRequiredProperty("Input");
  assert(inputProperty);

  vtkSMUncheckedPropertyHelper helper(inputProperty);
  if (helper.GetNumberOfElements() > 0)
    {
    vtkSMSourceProxy* sp = vtkSMSourceProxy::SafeDownCast(helper.GetAsProxy(0));
    if (sp)
      {
      return sp->GetDataInformation(helper.GetOutputPort());
      }
    }
  return NULL;
}

//----------------------------------------------------------------------------
int vtkSMChartSeriesListDomain::ReadXMLAttributes(vtkSMProperty* prop, vtkPVXMLElement* element)
{
  if (!this->Superclass::ReadXMLAttributes(prop, element))
    {
    return 0;
    }

  int hide_partial_arrays;
  if (element->GetScalarAttribute("hide_partial_arrays", &hide_partial_arrays))
    {
    this->HidePartialArrays = (hide_partial_arrays == 1);
    }

  if (!this->GetRequiredProperty("Input"))
    {
    vtkWarningMacro("Missing 'Input' property. Domain may not work correctly.");
    }
  if (!this->GetRequiredProperty("FieldDataSelection"))
    {
    vtkWarningMacro("Missing 'FieldDataSelection' property. Domain may not work correctly.")
    }

  return 1;
}

//----------------------------------------------------------------------------
int vtkSMChartSeriesListDomain::SetDefaultValues(vtkSMProperty* prop)
{
  const char* strings_to_check[] =
    { "bin_extents",
      "Time",
      "time",
      "arc_length",
      NULL
    };

  const std::vector<vtkStdString> domain_strings = this->GetStrings();
  for (int cc=0; strings_to_check[cc] != NULL; cc++)
    {
    if (std::find(domain_strings.begin(), domain_strings.end(),
      std::string(strings_to_check[cc])) != domain_strings.end())
      {
      vtkSMPropertyHelper(prop).Set(strings_to_check[cc]);
      return 1;
      }
    }

  return this->Superclass::SetDefaultValues(prop);
}

//----------------------------------------------------------------------------
void vtkSMChartSeriesListDomain::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "HidePartialArrays: " << this->HidePartialArrays << endl;
}
