/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"
#include "nuiImageAnimation.h"
#include "nuiTask.h"



/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  LoadCSS(_T("rsrc:/css/style.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  mpNav = new nuiNavigationController();
  AddChild(mpNav);
  
  MyViewController* pView = new MyViewController();
  mpNav->PushViewController(pView, false);
  
  
}







void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}











MyViewController::MyViewController()
: nuiViewController(), mTimer(0.4f), mEventSink(this)
{
  SetObjectClass(_T("MyViewController"));
  SetTitle(_T("Swipe Detector"));
  
//  mClicked = false;
  
  mpArrowsContainer = new nuiSimpleContainer();
  mpArrowsContainer->SetObjectName(_T("ArrowsContainer"));
  AddChild(mpArrowsContainer);
  
  mpArrows = new nuiImageAnimation(17, _T("rsrc:/decorations/directions.png"));
  mpArrows->SetObjectName(_T("Arrows"));
  mpArrowsContainer->AddChild(mpArrows);
  
  mEventSink.Connect(mTimer.Tick, &MyViewController::OnResetArrows);  
}


MyViewController::~MyViewController()
{

}


//// virtual 
//bool MyViewController::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
//{
//  bool res = nuiViewController::MouseClicked(X, Y, Button);
//  mClicked = true;
//  return res;
//}
//
//
//// virtual 
//bool MyViewController::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
//{
//  bool res = nuiViewController::MouseUnclicked(X, Y, Button);
//  mClicked = false;
//  
//  SetDelay(nuiMakeTask(this, &MyViewController::ResetArrows), 0.4f);
//  
//  return res;
//}
//
//
//// virtual 
//bool MyViewController::MouseMoved(nuiSize X, nuiSize Y)
//{
//  bool res = nuiViewController::MouseMoved(X, Y);
//  if (!mClicked)
//    return res;
//
//  
//  
//  return res;
//}



// virtual 
void MyViewController::SwipeBegan(nuiPosition swipeDirection)
{
  mTimer.Stop();

  uint32 index = 0;
  
  switch (swipeDirection)
  {
    case nuiTop:          index = 9; break;
    case nuiTopRight:     index = 10; break;
    case nuiRight:        index = 11; break;
    case nuiBottomRight:  index = 12; break;
    case nuiBottom:       index = 13; break;
    case nuiBottomLeft:   index = 14; break;
    case nuiLeft:         index = 15; break;
    case nuiTopLeft:      index = 16; break;
  }
  
  mpArrows->SetFrameIndex(index);
  mpArrows->Invalidate();
  
  mTimer.Start(false);
}


void MyViewController::SwipeEnd(nuiPosition swipeDirection)
{
  mTimer.Stop();

  uint32 index = 0;
  
  switch (swipeDirection)
  {
    case nuiTop: index = 1; break;
    case nuiTopRight: index = 2; break;
    case nuiRight: index = 3; break;
    case nuiBottomRight: index = 4; break;
    case nuiBottom: index = 5; break;
    case nuiBottomLeft: index = 6; break;
    case nuiLeft: index = 7; break;
    case nuiTopLeft: index = 8; break;
  }
  
  mpArrows->SetFrameIndex(index);
  mpArrows->Invalidate();
  
  mTimer.Start(false);
}



//SetDelay(nuiMakeTask(this, &MyViewController::ResetArrows), 0.4f);
//void MyViewController::SetDelay(nuiTask* pTask, float seconds)
//{
//  nuiAnimation::RunOnAnimationTick(pTask, seconds / nuiAnimation::GetTimer()->GetPeriod());      
//}


void MyViewController::OnResetArrows(const nuiEvent& rEvent)
{
  mTimer.Stop();
  
  mpArrows->SetFrameIndex(0);
  mpArrows->Invalidate();
}
