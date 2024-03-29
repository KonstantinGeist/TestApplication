// Copyright (C) The Company

#include "stdafx.h"
#include "TestApplication.h"

#include "TestApplicationDoc.h"
#include "TestApplicationView.h"

#include "DoubleBufferedDC.h"

#include "ShapeViewFactory.h"
#include "ConcreteMarkerView.h"
#include "ConcreteShapeViewBase.h"

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
END_MESSAGE_MAP()

// CTestApplicationView construction/destruction

CTestApplicationView::CTestApplicationView()
	: m_listener(nullptr), m_curIcon(IDC_ARROW), m_cursor(view::ECanvasCursor::Arrow)
{
	// TODO ?
	theApp.CanvasPresenter = std::make_unique<presenter::CCanvasPresenter>(theApp.CanvasPresentation.get(), this);
}

CTestApplicationView::~CTestApplicationView()
{
	theApp.CanvasPresenter = nullptr;
}

// CTestApplicationView drawing

BOOL CTestApplicationView::OnEraseBkgnd(CDC* pDC)
{
	// Disables background fill we don't need as we blit our doubble-buffered
	// bitmap all over the client area anyway.
	return TRUE;
}

// TODO could use antialiasing
void CTestApplicationView::OnDraw(CDC* pDC_)
{
	pDC_->SetBkColor(RGB(217, 217, 217)); // gray

	CDoubleBufferedDC pDC(pDC_);

	// **********************************
	//   Renders the border/background.
	// **********************************

	{
		CBrush brush(RGB(255, 255, 255)); // white
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // black
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle(m_rect);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	// **********************************
	//     Renders the shape views.
	// **********************************

	for(auto& shapeView: m_shapeViews)
	{
		// InsertShapeView(..) generates CConcreteShapeViewBase's which are cast to IShapeView
		// to hide CDC* from public interfaces.
		// TODO Or maybe make m_shapeViews a list of CConcreteShapeViewBase's to avoid
		// this dirty error-prone cast.
		static_cast<view::CConcreteShapeViewBase*>(shapeView.get())->Render(pDC);
	}

	// **********************************
	//       Renders selection.
	// **********************************

	if (m_selected)
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(58, 95, 139));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle(m_selected->GetRect());

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	// **********************************
	//      Renders the marker views.
	// **********************************

	{

		CBrush brush(RGB(58, 95, 139));
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

		for (auto& markerView : m_markerViews)
		{
			pDC->Ellipse(markerView->GetRect());
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	}
}

// Input events.

void CTestApplicationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
	{
		if (m_listener)
			m_listener->OnDeleteKey();
	}
}

void CTestApplicationView::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
	if (m_listener)
		m_listener->OnMouseDown(point);
}

void CTestApplicationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_listener)
		m_listener->OnMouseMove(point);
}

void CTestApplicationView::OnLButtonUp(UINT /* nFlags */, CPoint point)
{
	if (m_listener)
		m_listener->OnMouseUp(point);
}

// Implements view::ICanvasView

std::shared_ptr<view::IShapeView> CTestApplicationView::InsertShapeView(const std::string& typeName, int index)
{
	auto shapeView = std::shared_ptr<view::IShapeView>(view::CShapeViewFactory::CreateInstance(typeName));
	m_shapeViews.insert(m_shapeViews.begin() + index, shapeView);
	return shapeView;
}

void CTestApplicationView::RemoveShapeView(std::shared_ptr<view::IShapeView> view)
{
	auto it = std::find(m_shapeViews.begin(), m_shapeViews.end(), view);
	assert(it != m_shapeViews.end());
	m_shapeViews.erase(it);

	if (view == m_selected)
		m_selected = nullptr;
}

void CTestApplicationView::SelectShapeView(std::shared_ptr<view::IShapeView> view)
{
	assert(!view || std::find(m_shapeViews.begin(), m_shapeViews.end(), view) != m_shapeViews.end());

	m_selected = view;
}

std::shared_ptr<view::IMarkerView> CTestApplicationView::InsertMarkerView()
{
	auto markerView = std::make_shared<view::CConcreteMarkerView>();
	m_markerViews.push_back(markerView);
	return markerView;
}

void CTestApplicationView::RemoveMarkerView(std::shared_ptr<view::IMarkerView> view)
{
	auto it = std::find(m_markerViews.begin(), m_markerViews.end(), view);
	assert(it != m_markerViews.end());
	m_markerViews.erase(it);
}

// Windows tries to reset it back, we force it not to.
BOOL CTestApplicationView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(m_curIcon));
	return TRUE;
}

void CTestApplicationView::SetCursor(view::ECanvasCursor cursor)
{
	if (m_cursor == cursor)
		return;

	switch (cursor)
	{
	case view::ECanvasCursor::Arrow:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		m_curIcon = IDC_ARROW;
		break;
	case view::ECanvasCursor::Hand:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		m_curIcon = IDC_ARROW;
		break;
	case view::ECanvasCursor::SizeNESW:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		m_curIcon = IDC_SIZENESW;
		break;
	case view::ECanvasCursor::SizeNWSE:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		m_curIcon = IDC_SIZENWSE;
		break;
	default:
		throw std::out_of_range("unknown marker");
	}

	m_cursor = cursor;
}

view::ECanvasCursor CTestApplicationView::GetCursor() const
{
	return m_cursor;
}

void CTestApplicationView::SetMouseCapture(bool b)
{
	if (b)
		SetCapture();
	else
		::ReleaseCapture();
}

void CTestApplicationView::SetRect(const CRect& rect)
{
	m_rect = rect;
}

void CTestApplicationView::Invalidate()
{
	CView::Invalidate();
}

void CTestApplicationView::SetListener(view::ICanvasViewListener* listener)
{
	m_listener = listener;
}
