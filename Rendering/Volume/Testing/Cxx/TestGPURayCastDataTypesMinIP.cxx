/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestGPURayCastDataTypesMinIP.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// This test volume renders the same dataset using 8 different data types
// (unsigned char, signed char, unsigned short, signed short, unsigned int
// int, float, and double). It uses compositing and no shading. The extents of
// the dataset are (0, 114, 0, 100, 0, 74).
//
// Tests cell-data with a large data type (int).

#include "vtkAlgorithmOutput.h"
#include "vtkCellData.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkTestUtilities.h"
#include "vtkXMLImageDataReader.h"
#include "vtkImageShiftScale.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkTransform.h"
#include "vtkPointDataToCellData.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolumeProperty.h"
#include "vtkCamera.h"
#include "vtkRegressionTestImage.h"

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


namespace
{
  vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper[4][4];
  vtkSmartPointer<vtkVolumeProperty> volumeProperty[4][4];
  vtkSmartPointer<vtkVolume> volume[4][4];
  vtkSmartPointer<vtkTransform> userMatrix[4][4];
  vtkSmartPointer<vtkImageShiftScale> shiftScale[4][4];
  vtkSmartPointer<vtkAlgorithmOutput> algoOut[4][4];
  vtkSmartPointer<vtkColorTransferFunction> color[4][4];
  vtkSmartPointer<vtkPiecewiseFunction> opacity[4][4];

  void RegisterVolumeToRender(vtkRenderer* ren1, const int i, const int j)
  {
    volumeMapper[i][j] = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper[i][j]->SetBlendModeToMinimumIntensity();
    volumeMapper[i][j]->SetInputConnection(algoOut[i][j]);

    volumeProperty[i][j] = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty[i][j]->SetColor(color[i][j]);
    volumeProperty[i][j]->SetScalarOpacity(opacity[i][j]);

    volume[i][j] = vtkSmartPointer<vtkVolume>::New();
    volume[i][j]->SetMapper(volumeMapper[i][j]);
    volume[i][j]->SetProperty(volumeProperty[i][j]);

    userMatrix[i][j] = vtkSmartPointer<vtkTransform>::New();
    userMatrix[i][j]->PostMultiply();
    userMatrix[i][j]->Identity();
    userMatrix[i][j]->Translate(i * 120, j * 120,0);

    volume[i][j]->SetUserTransform(userMatrix[i][j]);
    ren1->AddViewProp(volume[i][j]);
  }
}

int TestGPURayCastDataTypesMinIP(int argc,
                                 char *argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;
  char *cfname=
    vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/vase_1comp.vti");

  vtkNew<vtkXMLImageDataReader> reader;
  reader->SetFileName(cfname);
  delete [] cfname;

// unsigned char
  shiftScale[0][0]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[0][0]->SetShift(-255);
  shiftScale[0][0]->SetScale(-1);
  shiftScale[0][0]->SetInputConnection(reader->GetOutputPort());
  shiftScale[0][0]->Update();
  algoOut[0][0] = shiftScale[0][0]->GetOutputPort();
  double range[2];
  shiftScale[0][0]->GetOutput()->GetPointData()->GetScalars()->GetRange(range);
  cout<<"range="<<range[0]<<","<<range[1]<<endl;

  color[0][0]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[0][0]->AddRGBPoint(0,0,0,1);
  color[0][0]->AddRGBPoint(255,0,1,0);

  opacity[0][0]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[0][0]->AddPoint(0,1);
  opacity[0][0]->AddPoint(255,0);

// unsigned char (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_02;
  pointsToCells_02->SetInputConnection(shiftScale[0][0]->GetOutputPort());
  algoOut[0][2] = pointsToCells_02->GetOutputPort();

  color[0][2]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[0][2]->AddRGBPoint(0,0,0,1);
  color[0][2]->AddRGBPoint(255,0,1,0);

  opacity[0][2]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[0][2]->AddPoint(0,1);
  opacity[0][2]->AddPoint(255,0);

// signed char
  shiftScale[0][1]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[0][1]->SetInputConnection(shiftScale[0][0]->GetOutputPort());
  shiftScale[0][1]->SetShift(-128);
  shiftScale[0][1]->SetOutputScalarType(15);
  algoOut[0][1] = shiftScale[0][1]->GetOutputPort();

  color[0][1]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[0][1]->AddRGBPoint(-128,0,0,1);
  color[0][1]->AddRGBPoint(127,0,1,0);

  opacity[0][1]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[0][1]->AddPoint(-128,1);
  opacity[0][1]->AddPoint(127,0);

// signed char (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_03;
  pointsToCells_03->SetInputConnection(shiftScale[0][1]->GetOutputPort());
  algoOut[0][3] = pointsToCells_03->GetOutputPort();

  color[0][3]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[0][3]->AddRGBPoint(-128,0,0,1);
  color[0][3]->AddRGBPoint(127,0,1,0);

  opacity[0][3]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[0][3]->AddPoint(-128,1);
  opacity[0][3]->AddPoint(127,0);

// unsigned short
  shiftScale[1][0]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[1][0]->SetInputConnection(shiftScale[0][0]->GetOutputPort());
  shiftScale[1][0]->SetScale(256);
  shiftScale[1][0]->SetOutputScalarTypeToUnsignedShort();
  algoOut[1][0] = shiftScale[1][0]->GetOutputPort();

  color[1][0]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[1][0]->AddRGBPoint(0,0,0,1);
  color[1][0]->AddRGBPoint(65535,0,1,0);

  opacity[1][0]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[1][0]->AddPoint(0,1);
  opacity[1][0]->AddPoint(65535,0);

// unsigned short (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_12;
  pointsToCells_12->SetInputConnection(shiftScale[1][0]->GetOutputPort());
  algoOut[1][2] = pointsToCells_12->GetOutputPort();

  color[1][2]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[1][2]->AddRGBPoint(0,0,0,1);
  color[1][2]->AddRGBPoint(65535,0,1,0);

  opacity[1][2]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[1][2]->AddPoint(0,1);
  opacity[1][2]->AddPoint(65535,0);

//  short
  shiftScale[1][1]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[1][1]->SetInputConnection(shiftScale[1][0]->GetOutputPort());
  shiftScale[1][1]->SetShift(-32768);
  shiftScale[1][1]->SetOutputScalarTypeToShort();
  algoOut[1][1] = shiftScale[1][1]->GetOutputPort();

  color[1][1]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[1][1]->AddRGBPoint(-32768,0,0,1);
  color[1][1]->AddRGBPoint(32767,0,1,0);

  opacity[1][1]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[1][1]->AddPoint(-32768,1);
  opacity[1][1]->AddPoint(32767,0);

// short (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_13;
  pointsToCells_13->SetInputConnection(shiftScale[1][1]->GetOutputPort());
  algoOut[1][3] = pointsToCells_13->GetOutputPort();

  color[1][3]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[1][3]->AddRGBPoint(-32768,0,0,1);
  color[1][3]->AddRGBPoint(32767,0,1,0);

  opacity[1][3]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[1][3]->AddPoint(-32768,1);
  opacity[1][3]->AddPoint(32767,0);

// unsigned int
  shiftScale[2][0]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[2][0]->SetInputConnection (shiftScale[0][0]->GetOutputPort());
  shiftScale[2][0]->SetScale(16777216);
  shiftScale[2][0]->SetOutputScalarTypeToUnsignedInt();
  algoOut[2][0] = shiftScale[2][0]->GetOutputPort();

  color[2][0]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[2][0]->AddRGBPoint(0,0,0,1);
  color[2][0]->AddRGBPoint(VTK_UNSIGNED_INT_MAX,0,1,0);

  opacity[2][0]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[2][0]->AddPoint(0,1);
  opacity[2][0]->AddPoint(VTK_UNSIGNED_INT_MAX,0);

// unsigned int (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_22;
  pointsToCells_22->SetInputConnection(shiftScale[2][0]->GetOutputPort());
  algoOut[2][2] = pointsToCells_22->GetOutputPort();

  color[2][2]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[2][2]->AddRGBPoint(0,0,0,1);
  color[2][2]->AddRGBPoint(VTK_UNSIGNED_INT_MAX,0,1,0);

  opacity[2][2]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[2][2]->AddPoint(0,1);
  opacity[2][2]->AddPoint(VTK_UNSIGNED_INT_MAX,0);

// int
  shiftScale[2][1]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[2][1]->SetInputConnection(shiftScale[2][0]->GetOutputPort());
  shiftScale[2][1]->SetShift(VTK_INT_MIN);
  shiftScale[2][1]->SetOutputScalarTypeToInt();
  shiftScale[2][1]->Update();
  algoOut[2][1] = shiftScale[2][1]->GetOutputPort();

  color[2][1]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[2][1]->AddRGBPoint(VTK_INT_MIN,0,0,1);
  color[2][1]->AddRGBPoint(VTK_INT_MAX,0,1,0);

  opacity[2][1]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[2][1]->AddPoint(VTK_INT_MIN,1);
  opacity[2][1]->AddPoint(VTK_INT_MAX,0);

// int (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells;
  pointsToCells->SetInputConnection(shiftScale[2][1]->GetOutputPort());
  algoOut[2][3] = pointsToCells->GetOutputPort();

  color[2][3]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[2][3]->AddRGBPoint(VTK_INT_MIN,0,0,1);
  color[2][3]->AddRGBPoint(VTK_INT_MAX,0,1,0);

  opacity[2][3]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[2][3]->AddPoint(VTK_INT_MIN,1);
  opacity[2][3]->AddPoint(VTK_INT_MAX,0);

// float [-1 1]
  vtkNew<vtkImageShiftScale> shiftScale_3_0_pre;
  shiftScale_3_0_pre->SetInputConnection(shiftScale[0][0]->GetOutputPort());
  shiftScale_3_0_pre->SetScale(0.0078125);
  shiftScale_3_0_pre->SetOutputScalarTypeToFloat();

  shiftScale[3][0]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[3][0]->SetInputConnection(shiftScale_3_0_pre->GetOutputPort());
  shiftScale[3][0]->SetShift(-1.0);
  shiftScale[3][0]->SetOutputScalarTypeToFloat();
  algoOut[3][0] = shiftScale[3][0]->GetOutputPort();

  color[3][0]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[3][0]->AddRGBPoint(-1.0,0,0,1);
  color[3][0]->AddRGBPoint(1.0,0,1,0);

  opacity[3][0]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[3][0]->AddPoint(-1.0,1);
  opacity[3][0]->AddPoint(1.0,0);

// float (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_32;
  pointsToCells_32->SetInputConnection(shiftScale[3][0]->GetOutputPort());
  algoOut[3][2] = pointsToCells_32->GetOutputPort();

  color[3][2]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[3][2]->AddRGBPoint(-1.0,0,0,1);
  color[3][2]->AddRGBPoint(1.0,0,1,0);

  opacity[3][2]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[3][2]->AddPoint(-1.0,1);
  opacity[3][2]->AddPoint(1.0,0);

// double [-1000 3000]
  vtkNew<vtkImageShiftScale> shiftScale_3_1_pre;
  shiftScale_3_1_pre->SetInputConnection(shiftScale[0][0]->GetOutputPort());
  shiftScale_3_1_pre->SetScale(15.625);
  shiftScale_3_1_pre->SetOutputScalarTypeToDouble();

  shiftScale[3][1]=vtkSmartPointer<vtkImageShiftScale>::New();
  shiftScale[3][1]->SetInputConnection(shiftScale_3_1_pre->GetOutputPort());
  shiftScale[3][1]->SetShift(-1000);
  shiftScale[3][1]->SetOutputScalarTypeToDouble();
  algoOut[3][1] = shiftScale[3][1]->GetOutputPort();

  color[3][1]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[3][1]->AddRGBPoint(-1000,0,0,1);
  color[3][1]->AddRGBPoint(3000,0,1,0);

  opacity[3][1]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[3][1]->AddPoint(-1000,1);
  opacity[3][1]->AddPoint(3000,0);

// double (cell data)
  vtkNew<vtkPointDataToCellData> pointsToCells_33;
  pointsToCells_33->SetInputConnection(shiftScale[3][1]->GetOutputPort());
  algoOut[3][3] = pointsToCells_33->GetOutputPort();

  color[3][3]=vtkSmartPointer<vtkColorTransferFunction>::New();
  color[3][3]->AddRGBPoint(-1000,0,0,1);
  color[3][3]->AddRGBPoint(3000,0,1,0);

  opacity[3][3]=vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacity[3][3]->AddPoint(-1000,1);
  opacity[3][3]->AddPoint(3000,0);


  vtkNew<vtkRenderer> ren1;
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren1);
  renWin->SetSize(600, 600);
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  renWin->Render();

  int i=0;
  while(i<4)
  {
    int j=0;
    while(j<4)
    {
      RegisterVolumeToRender(ren1, i, j);
      ++j;
    }
    ++i;
  }

  ren1->AddViewProp(volume[0][0]);
  int valid=volumeMapper[0][0]->IsRenderSupported(renWin,volumeProperty[0][0]);
  int retVal;
  if(valid)
  {
    iren->Initialize();
    ren1->SetBackground(0.1,0.4,0.2);
    ren1->ResetCamera();
    ren1->GetActiveCamera()->Zoom(1.25);
    renWin->Render();

    retVal = vtkTesting::Test(argc, argv, renWin, 75);
    if (retVal == vtkRegressionTester::DO_INTERACTOR)
    {
      iren->Start();
    }
  }
  else
  {
    retVal=vtkTesting::PASSED;
    cout << "Required extensions not supported." << endl;
  }

  if ((retVal == vtkTesting::PASSED) || (retVal == vtkTesting::DO_INTERACTOR))
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
