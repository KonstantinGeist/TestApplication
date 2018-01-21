// Copyright (C) The Company

#include "stdafx.h"

#include "ConvexShapeView.h"
#include "DocumentView.h"

CConvexShapeView::CConvexShapeView()
	: m_isSelected(false)
{
}

void CConvexShapeView::InitInstance(CDocumentView* docView)
{
	const CRect rect(docView->GetRect());

	int left = rect.left + (rect.right - rect.left) / 2 - DEFAULT_SIZE / 2;
	int top = rect.top + (rect.bottom - rect.top) / 2 - DEFAULT_SIZE / 2;

	m_rect = CRect(left, top, left + DEFAULT_SIZE, top + DEFAULT_SIZE);
}

bool CConvexShapeView::HitTest(const CPoint& p) const
{
	return m_rect.PtInRect(p);
}

CRect CConvexShapeView::GetRect() const
{
	return m_rect;
}

void CConvexShapeView::SetRect(const CRect& rect)
{
	m_rect = rect;
}

void CConvexShapeView::SetPosition(const CPoint& _p)
{
	int center_x = (m_rect.right - m_rect.left) / 2;
	int center_y = (m_rect.bottom - m_rect.top) / 2;

	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	CPoint p = _p;
	p.x -= center_x;
	p.y -= center_y;

	m_rect.left = p.x;
	m_rect.top = p.y;

	m_rect.right = p.x + width;
	m_rect.bottom = p.y + height;
}

CPoint CConvexShapeView::GetPosition() const
{
	return m_rect.CenterPoint();
}

bool CConvexShapeView::IsSelected()
{
	return m_isSelected;
}

void CConvexShapeView::SetSelected(bool value)
{
	m_isSelected = value;
}

int CConvexShapeView::GetMarkerCount() const
{
	return 4;
}

#define MARKER_SIZE 10
inline CRect getRectAroundPoint(const CPoint& p)
{
	return CRect(p.x - MARKER_SIZE / 2,
		p.y - MARKER_SIZE / 2,
		p.x + MARKER_SIZE / 2,
		p.y + MARKER_SIZE / 2);
}

CRect CConvexShapeView::GetMarkerRect(int index) const
{
	switch (index)
	{
		case 0: return getRectAroundPoint(CPoint(m_rect.left, m_rect.top));
		case 1: return getRectAroundPoint(CPoint(m_rect.right, m_rect.top));
		case 2: return getRectAroundPoint(CPoint(m_rect.right, m_rect.bottom));
		case 3: return getRectAroundPoint(CPoint(m_rect.left, m_rect.bottom));
		default:
			throw std::out_of_range("marker index out of range");
	}
}

MarkerCursor CConvexShapeView::GetMarkerCursor(int index) const
{
	switch (index)
	{
		case 0: return MarkerCursor::SizeNWSE;
		case 1: return MarkerCursor::SizeNESW;
		case 2: return MarkerCursor::SizeNWSE;
		case 3: return MarkerCursor::SizeNESW;
		default:
			throw std::out_of_range("marker index out of range");
	}
}

#define MIN_SHAPE_SIZE 20
void CConvexShapeView::SetMarkerPosition(int index, const CPoint& p)
{
	CRect newRect = m_rect;

	switch (index)
	{
		case 0:
			if(newRect.right - p.x > MIN_SHAPE_SIZE)
				newRect.left = p.x;
			if(newRect.bottom - p.y > MIN_SHAPE_SIZE)
				newRect.top = p.y;
			break;

		case 1:
			if(p.x - newRect.left > MIN_SHAPE_SIZE)
				newRect.right = p.x;
			if (newRect.bottom - p.y > MIN_SHAPE_SIZE)
				newRect.top = p.y;
			break;

		case 2:
			if (p.x - newRect.left > MIN_SHAPE_SIZE)
				newRect.right = p.x;
			if(p.y - newRect.top > MIN_SHAPE_SIZE)
				newRect.bottom = p.y;
			break;

		case 3:
			if (newRect.right - p.x > MIN_SHAPE_SIZE)
				newRect.left = p.x;
			if (p.y - newRect.top > MIN_SHAPE_SIZE)
				newRect.bottom = p.y;
			break;

		default:
			throw std::out_of_range("marker index out of range");
	}

	m_rect = newRect;
}

CPoint CConvexShapeView::GetMarkerPosition(int index) const
{
	return GetMarkerRect(index).CenterPoint();
}

void CConvexShapeView::Render(CDC* pDC, ShapeViewLayer layer)
{
	// Renders the selection rectangle with markers.
	// If more complex shapes are introduced, then selection areas can be different
	// from a simple rect -- hence it's specialized per-class.
	if (layer == ShapeViewLayer::Helper && m_isSelected)
	{
		// Selection rectangle.

		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(58, 95, 139));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle(GetRect());

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		// Markers.

		CBrush brush(RGB(58, 95, 139));
		pOldBrush = pDC->SelectObject(&brush);
		pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

		int markerCount = GetMarkerCount();
		for (int i = 0; i < markerCount; i++)
		{
			pDC->Ellipse(GetMarkerRect(i));
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}
