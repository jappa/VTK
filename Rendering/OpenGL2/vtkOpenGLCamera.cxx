/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkOpenGLCamera.h"

#include "vtkMatrix4x4.h"
#include "vtkMatrix3x3.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkOutputWindow.h"
#include "vtkOpenGLRenderWindow.h"
#include "vtkOpenGLError.h"

#include <math.h>

vtkStandardNewMacro(vtkOpenGLCamera);


vtkOpenGLCamera::vtkOpenGLCamera()
{
  this->WCVCMatrix = vtkMatrix4x4::New();
  this->NormalMatrix = vtkMatrix3x3::New();
  this->VCDCMatrix = vtkMatrix4x4::New();
}

vtkOpenGLCamera::~vtkOpenGLCamera()
{
  this->WCVCMatrix->Delete();
  this->NormalMatrix->Delete();
  this->VCDCMatrix->Delete();
}

// Implement base class method.
void vtkOpenGLCamera::Render(vtkRenderer *ren)
{
  vtkOpenGLClearErrorMacro();

  int lowerLeft[2];
  int usize, vsize;
  vtkMatrix4x4 *matrix = vtkMatrix4x4::New();

  vtkOpenGLRenderWindow *win = vtkOpenGLRenderWindow::SafeDownCast(ren->GetRenderWindow());

  // find out if we should stereo render
  this->Stereo = (ren->GetRenderWindow())->GetStereoRender();
  ren->GetTiledSizeAndOrigin(&usize, &vsize, lowerLeft, lowerLeft+1);

  // if were on a stereo renderer draw to special parts of screen
  if (this->Stereo)
    {
    switch ((ren->GetRenderWindow())->GetStereoType())
      {
      case VTK_STEREO_CRYSTAL_EYES:
        if (this->LeftEye)
          {
          if (ren->GetRenderWindow()->GetDoubleBuffer())
            {
            glDrawBuffer(static_cast<GLenum>(win->GetBackLeftBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetBackLeftBuffer()));
            }
          else
            {
            glDrawBuffer(static_cast<GLenum>(win->GetFrontLeftBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetFrontLeftBuffer()));
            }
          }
        else
          {
           if (ren->GetRenderWindow()->GetDoubleBuffer())
            {
            glDrawBuffer(static_cast<GLenum>(win->GetBackRightBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetBackRightBuffer()));
            }
          else
            {
            glDrawBuffer(static_cast<GLenum>(win->GetFrontRightBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetFrontRightBuffer()));
            }
          }
        break;
      case VTK_STEREO_LEFT:
        this->LeftEye = 1;
        break;
      case VTK_STEREO_RIGHT:
        this->LeftEye = 0;
        break;
      default:
        break;
      }
    }
  else
    {
    if (ren->GetRenderWindow()->GetDoubleBuffer())
      {
      glDrawBuffer(static_cast<GLenum>(win->GetBackBuffer()));

      // Reading back buffer means back left. see OpenGL spec.
      // because one can write to two buffers at a time but can only read from
      // one buffer at a time.
      glReadBuffer(static_cast<GLenum>(win->GetBackBuffer()));
      }
    else
      {
      glDrawBuffer(static_cast<GLenum>(win->GetFrontBuffer()));

      // Reading front buffer means front left. see OpenGL spec.
      // because one can write to two buffers at a time but can only read from
      // one buffer at a time.
      glReadBuffer(static_cast<GLenum>(win->GetFrontBuffer()));
      }
    }

  glViewport(lowerLeft[0], lowerLeft[1], usize, vsize);
  glEnable(GL_SCISSOR_TEST);
  glScissor(lowerLeft[0], lowerLeft[1], usize, vsize);

  if ((ren->GetRenderWindow())->GetErase() && ren->GetErase()
      && !ren->GetIsPicking())
    {
    ren->Clear();
    }

  matrix->Delete();

  vtkOpenGLCheckErrorMacro("failed after Render");
}

//----------------------------------------------------------------------------
void vtkOpenGLCamera::UpdateViewport(vtkRenderer *ren)
{
  vtkOpenGLClearErrorMacro();

  int lowerLeft[2];
  int usize, vsize;
  ren->GetTiledSizeAndOrigin(&usize, &vsize, lowerLeft, lowerLeft+1);

  glViewport(lowerLeft[0], lowerLeft[1], usize, vsize);
  glEnable(GL_SCISSOR_TEST);
  glScissor(lowerLeft[0], lowerLeft[1], usize, vsize);

  vtkOpenGLCheckErrorMacro("failed after UpdateViewport");
}

//----------------------------------------------------------------------------
void vtkOpenGLCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

void vtkOpenGLCamera::GetKeyMatrices(vtkRenderer *ren, vtkMatrix4x4 *&wcvc,
        vtkMatrix3x3 *&normMat, vtkMatrix4x4 *&vcdc)
{
  // has the camera changed?
  if (this->MTime > this->KeyMatrixTime ||
      ren->GetMTime() > this->KeyMatrixTime)
    {
    this->WCVCMatrix->DeepCopy(this->GetModelViewTransformMatrix());

    for(int i = 0; i < 3; ++i)
      {
      for (int j = 0; j < 3; ++j)
        {
        this->NormalMatrix->SetElement(i, j, this->WCVCMatrix->GetElement(i, j));
        }
      }
    this->NormalMatrix->Invert();

    this->WCVCMatrix->Transpose();

    double aspect[2];
    int  lowerLeft[2];
    int usize, vsize;
    ren->GetTiledSizeAndOrigin(&usize, &vsize, lowerLeft, lowerLeft+1);

    ren->ComputeAspect();
    ren->GetAspect(aspect);
    double aspect2[2];
    ren->vtkViewport::ComputeAspect();
    ren->vtkViewport::GetAspect(aspect2);
    double aspectModification = aspect[0] * aspect2[1] / (aspect[1] * aspect2[0]);

    if (usize && vsize)
      {
      this->VCDCMatrix->DeepCopy(this->GetProjectionTransformMatrix(
                         aspectModification * usize / vsize, -1, 1));
      this->VCDCMatrix->Transpose();
      }

    this->KeyMatrixTime.Modified();
    }

  wcvc = this->WCVCMatrix;
  normMat = this->NormalMatrix;
  vcdc = this->VCDCMatrix;
}