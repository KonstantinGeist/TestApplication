// Copyright (C) The Company

#include "stdafx.h"
#include "TestApplication.h"

#include "TestApplicationDoc.h"
#include "TestApplicationView.h"

#include "DoubleBufferedDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestApplicationView

IMPLEMENT_DYNCREATE(CTestApplicationView, CView)

BEGIN_MESSAGE_MAP(CTestApplicationView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ADD_RECTANGLE, &CTestApplicationView::OnAddRectangle)
	ON_COMMAND(ID_ADD_ELLIPSE, &CTestApplicationView::OnAddEllipse)
	ON_COMMAND(ID_ADD_TRIANGLE, &CTestApplicationView::OnAddTriangle)
	ON_COMMAND(ID_EDIT_UNDO, &CTestApplicationView::OnUndo)
	ON_COMMAND(ID_EDIT_REDO, &CTestApplicationView::OnRedo)
END_MESSAGE_MAP()

// CTestApplicationView construction/destruction

CTestApplicationView::CTestApplicationView()
	: m_docView (new CDocumentView()),
	  m_curIcon(IDC_ARROW),
	  m_mouseDown(false)
{
	auto& presenter = theApp.GetPresenter();
	presenter.SetDocumentView(m_docView);
	presenter.SetNativeView(this);
}

// CTestApplicationView drawing

BOOL CTestApplicationView::OnEraseBkgnd(CDC* pDC)
{
	// Disables background fill we don't need as we blit our doubble-buffered
	// bitmap all over the client area anyway.
	return TRUE;
}

// TODO could use antialiasing
void CTestApplicationView::OnDraw(CDC* pDC)
{
	pDC->SetBkColor(RGB(217, 217, 217)); // gray

	CDoubleBufferedDC pDC2(pDC);

	// **********************************
	//   Renders the border/background.
	// **********************************

	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC2->SelectObject(&brush);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC2->SelectObject(&penBlack);

	pDC2->Rectangle(m_docView->GetRect());

	pDC2->SelectObject(pOldBrush);
	pDC2->SelectObject(pOldPen);

	// ******************

	m_docView->Render(pDC2);
}

// Reroutes events to Presenter.

void CTestApplicationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
		theApp.GetPresenter().DoDelete();
}

void CTestApplicationView::OnAddRectangle()
{
	theApp.GetPresenter().DoAddRectangle();
}

void CTestApplicationView::OnAddTriangle()
{
	theApp.GetPresenter().DoAddTriangle();
}

void CTestApplicationView::OnAddEllipse()
{
	theApp.GetPresenter().DoAddEllipse();
}

void CTestApplicationView::OnUndo()
{
	if (!theApp.GetPresenter().Undo())
		MessageBox((LPCTSTR)L"Nothing to undo.", NULL, MB_ICONINFORMATION);
}

void CTestApplicationView::OnRedo()
{
	if(!theApp.GetPresenter().Redo())
		MessageBox((LPCTSTR)L"Nothing to redo.", NULL, MB_ICONINFORMATION);
}

void CTestApplicationView::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
	theApp.GetPresenter().Select(point);
	m_mouseDown = true;
}

void CTestApplicationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags == MK_LBUTTON)
	{
		if(m_mouseDown) // to prevent a spurious UP event after double-clicking inside a shell dialog
			theApp.GetPresenter().Drag(point);
	}
	else
	{
		theApp.GetPresenter().Hover(point);
	}
}

void CTestApplicationView::OnLButtonUp(UINT /* nFlags */, CPoint point)
{
	if (m_mouseDown) // to prevent a spurious UP event after double-clicking inside a shell dialog
	{
		theApp.GetPresenter().DoEndDrag(point);

		m_mouseDown = false;
	}
}

// Implements INativeView

// Windows tries to reset it back, we force it not to.
BOOL CTestApplicationView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(m_curIcon));
	return TRUE;
}

void CTestApplicationView::SetDefaultCursor()
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	m_curIcon = IDC_ARROW;
}

void CTestApplicationView::SetHandCursor()
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	m_curIcon = IDC_HAND;
}

void CTestApplicationView::SetMarkerCursor(MarkerCursor markerCursor)
{
	switch (markerCursor)
	{
	case MarkerCursor::SizeNESW:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		m_curIcon = IDC_SIZENESW;
		break;
	case MarkerCursor::SizeNWSE:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		m_curIcon = IDC_SIZENWSE;
		break;
	default:
		throw std::out_of_range("unknown marker");
	}
}

void CTestApplicationView::SetCapture(bool b)
{
	if (b)
		CView::SetCapture();
	else
		::ReleaseCapture();
}

void CTestApplicationView::Refresh()
{
	this->Invalidate();
}
