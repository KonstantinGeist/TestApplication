// Copyright (C) The Company

#include "stdafx.h"
#include "TestApplication.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	//ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()

	ON_COMMAND(ID_ADD_RECTANGLE, &CMainFrame::OnAddRectangle)
	ON_COMMAND(ID_ADD_ELLIPSE, &CMainFrame::OnAddEllipse)
	ON_COMMAND(ID_ADD_TRIANGLE, &CMainFrame::OnAddTriangle)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnRedo)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// set the visual style to be used the by the visual manager
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);

	return 0;
}

/*void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 700;
	lpMMI->ptMinTrackSize.y = 500;
}*/

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	/*cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;*/

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame event handlers

// event handlers
void CMainFrame::OnAddRectangle()
{
	theApp.CanvasPresentation->AddShape("Rectangle");
}

void CMainFrame::OnAddTriangle()
{
	theApp.CanvasPresentation->AddShape("Triangle");
}

void CMainFrame::OnAddEllipse()
{
	theApp.CanvasPresentation->AddShape("Ellipse");
}

void CMainFrame::OnUndo()
{
	auto history = theApp.CanvasPresentation->GetHistory();

	if (!history->CanUndo())
	{
		MessageBox((LPCTSTR)L"Nothing to undo.");
		return;
	}

	history->Undo();
}

void CMainFrame::OnRedo()
{
	auto history = theApp.CanvasPresentation->GetHistory();

	if (!history->CanRedo())
	{
		MessageBox((LPCTSTR)L"Nothing to redo.");
		return;
	}

	history->Redo();
}
