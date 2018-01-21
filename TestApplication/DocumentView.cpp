// Copyright (C) The Company

#include "stdafx.h"

#include "DocumentView.h"
#include "DocumentViewEventHandler.h"

CDocumentView::CDocumentView()
	: m_rect(0, 0, 640, 480), // default rect as per the spec
	  m_selShapeView(nullptr), m_handler(nullptr), m_isDragging(false),
	  m_curMarkerIndex(-1), m_curMarkerCursor(MarkerCursor::None)
{
}

void CDocumentView::SetEventHandler(IDocumentViewEventHandler* handler)
{
	m_handler = handler;
}

void CDocumentView::AddShapeView(std::shared_ptr<IShapeView> shapeView)
{
	assert(shapeView);

	m_shapeViews.push_back(shapeView);

	if (m_handler)
		m_handler->OnDocumentViewChange(this);
}

void CDocumentView::InsertShapeView(std::shared_ptr<IShapeView> shapeView, int index)
{
	assert(shapeView);

	m_shapeViews.insert(m_shapeViews.begin() + index, shapeView);

	if (m_handler)
		m_handler->OnDocumentViewChange(this);
}

void CDocumentView::RemoveShapeView(std::shared_ptr<IShapeView> shapeView)
{
	auto it = std::find(m_shapeViews.begin(), m_shapeViews.end(), shapeView);
	assert(it != m_shapeViews.end());
	if ((*it) == m_selShapeView)
	{
		m_selShapeView->SetSelected(false);
		m_selShapeView = nullptr;
	}
	m_shapeViews.erase(it);

	if (m_handler)
		m_handler->OnDocumentViewChange(this);
}

void CDocumentView::RemoveShapeViewAt(int index)
{
	assert(index >= 0 && index < m_shapeViews.size());

	auto shapeToRemove = m_shapeViews[index];
	if (shapeToRemove == m_selShapeView)
	{
		m_selShapeView->SetSelected(false);
		m_selShapeView = nullptr;
	}

	m_shapeViews.erase(m_shapeViews.begin() + index);

	if (m_handler)
		m_handler->OnDocumentViewChange(this);
}

void CDocumentView::Clear()
{
	m_shapeViews.clear();

	m_isDragging = false;

	m_curMarkerIndex = -1;
	m_curMarkerCursor = MarkerCursor::None;
}

const std::vector<std::shared_ptr<IShapeView>>& CDocumentView::GetShapeViews() const
{
	return m_shapeViews;
}

std::shared_ptr<IShapeView> CDocumentView::GetSelected() const
{
	return m_selShapeView;
}

bool CDocumentView::Select(const CPoint& p)
{
	if (m_curMarkerIndex == -1)
	{
		// Selecting a shape.

		std::shared_ptr<IShapeView> newSelection;
		for (auto shapeView : m_shapeViews)
		{
			if (shapeView->HitTest(p))
				newSelection = shapeView;
		}

		if (newSelection)
		{
			const CRect rect(newSelection->GetRect());
			m_dragOffset = rect.CenterPoint() - p;
		}

		if (newSelection != m_selShapeView)
		{
			if (m_selShapeView)
				m_selShapeView->SetSelected(false);

			if (newSelection)
				newSelection->SetSelected(true);

			m_selShapeView = newSelection;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		// Selecting a marker.

		// TODO can theoretically call the selected shape's something like IShapeView->SelectMarker(..),
		// not required at this point.
		//assert(m_selShapeView);

		return false;
	}
}

void CDocumentView::Select(std::shared_ptr<IShapeView> shapeView)
{
	if (m_selShapeView)
		m_selShapeView->SetSelected(false);

	if (shapeView)
		shapeView->SetSelected(true);

	m_selShapeView = shapeView;
}

// We can drag a shape or one of its markers.
bool CDocumentView::Drag(const CPoint& p)
{
	// Dragging cannot happen without a selected shape.
	if (!m_selShapeView)
		return false;

	// First dragging event?
	if (!m_isDragging)
	{
		m_isDragging = true;

		if (m_curMarkerIndex == -1)
		{
			// Dragging a shape.

			m_dragStartPosition = m_selShapeView->GetPosition();

			if (m_handler)
				m_handler->OnBeginShapeViewDrag(this);
		}
		else
		{
			// Dragging a marker.

			m_dragStartPosition = m_selShapeView->GetMarkerPosition(m_curMarkerIndex);

			if (m_handler)
				m_handler->OnBeginMarkerDrag(this);
		}
	}

	if (m_curMarkerIndex == -1)
	{
		// Dragging a shape.

		const CPoint pWithOffset = p + m_dragOffset;
		// Cannot move outside of the working area according to the spec.
		m_lastDragPosition = restrictToRect(
			pWithOffset,
			m_selShapeView->GetRect(),
			m_rect);

		m_selShapeView->SetPosition(m_lastDragPosition);
	}
	else
	{
		// Dragging a marker.

		const CPoint restrictedP = restrictToRect(p, m_rect);
		m_selShapeView->SetMarkerPosition(m_curMarkerIndex, restrictedP);

		// NOTE can't use restrictedP because ->SetMarkerPosition can also
		// update the position to make the size correct
		m_lastDragPosition = m_selShapeView->GetMarkerPosition(m_curMarkerIndex);
	}

	return true;
}

bool CDocumentView::EndDrag(const CPoint& p)
{
	if (m_isDragging)
	{
		m_isDragging = false;

		if (m_handler)
		{
			if (m_curMarkerIndex == -1)
			{
				// Dragging a shape.

				m_handler->OnEndShapeViewDrag(this);
			}
			else
			{
				// Dragging a marker.

				m_handler->OnEndMarkerDrag(this);
			}

			if(m_handler)
				m_handler->OnDocumentViewChange(this);
		}

		return true;
	}
	else
	{
		return false;
	}
}

CPoint CDocumentView::GetDragStartPosition() const
{
	return m_dragStartPosition;
}

CPoint CDocumentView::GetLastDragPosition() const
{
	return m_lastDragPosition;
}

bool CDocumentView::Hover(const CPoint& p)
{
	// Currently, hovering works only with selected shapes.
	if (m_selShapeView)
	{
		int markerCount = m_selShapeView->GetMarkerCount();
		for (int i = 0; i < markerCount; i++)
		{
			const CRect markerRect = m_selShapeView->GetMarkerRect(i);
			if (markerRect.PtInRect(p))
			{
				auto curMarkerCursor = m_selShapeView->GetMarkerCursor(i);
				if (curMarkerCursor != m_curMarkerCursor)
				{
					m_curMarkerIndex = i;
					m_curMarkerCursor = curMarkerCursor;

					if (m_handler)
						m_handler->OnBeginMarkerHover(this);
				}

				return true;
			}
		}
	}

	if (m_curMarkerCursor != MarkerCursor::None)
	{
		if(m_handler)
			m_handler->OnEndMarkerHover(this);

		m_curMarkerIndex = -1;
		m_curMarkerCursor = MarkerCursor::None;
	}

	return false;
}

std::shared_ptr<IShapeView> CDocumentView::RemoveSelected(int* out_index)
{
	std::shared_ptr<IShapeView> r (nullptr);

	// NOTE does not allow deleting while dragging
	if (m_selShapeView && !m_isDragging)
	{
		*out_index = find(m_shapeViews.begin(), m_shapeViews.end(), m_selShapeView) - m_shapeViews.begin();

		r = m_selShapeView;
		RemoveShapeView(m_selShapeView);
	}
	else
	{
		*out_index = -1;
	}

	return r;
}

void CDocumentView::Render(CDC* pDC)
{
	// Renders the default shape view layer.
	for (auto shapeView : m_shapeViews)
	{
		shapeView->Render(pDC, ShapeViewLayer::Default);
	}

	// Renders helpers.
	for (auto shapeView : m_shapeViews)
	{
		shapeView->Render(pDC, ShapeViewLayer::Helper);
	}
}

CRect CDocumentView::GetRect() const
{
	return m_rect;
}

void CDocumentView::SetRect(const CRect& rect)
{
	m_rect = rect;
}

MarkerCursor CDocumentView::GetCurrentMarkerCursor() const
{
	return m_curMarkerCursor;
}

int CDocumentView::GetCurrentMarkerIndex() const
{
	return m_curMarkerIndex;
}

void CDocumentView::MarkChanged()
{
	if (m_handler)
		m_handler->OnDocumentViewChange(this);
}

CPoint CDocumentView::restrictToRect(const CPoint& targetPos, const CRect& shapeViewRect, const CRect& documentRect)
{
	const CPoint dif = targetPos - shapeViewRect.CenterPoint();
	CRect shapeViewRectWithOffset = shapeViewRect; // OffsetRect(..)'s mutability...
	shapeViewRectWithOffset.OffsetRect(dif);

	CPoint r = targetPos;
	if (shapeViewRectWithOffset.left < documentRect.left)
		r.x += documentRect.left - shapeViewRectWithOffset.left;
	else if(shapeViewRectWithOffset.right > documentRect.right)
		r.x -= shapeViewRectWithOffset.right - documentRect.right;
	if (shapeViewRectWithOffset.top < documentRect.top)
		r.y += documentRect.top - shapeViewRectWithOffset.top;
	else if (shapeViewRectWithOffset.bottom > documentRect.bottom)
		r.y -= shapeViewRectWithOffset.bottom - documentRect.bottom;
	return r;
}

CPoint CDocumentView::restrictToRect(const CPoint& targetPos, const CRect& documentRect)
{
	CPoint r = targetPos;

	if (r.x < documentRect.left)
		r.x = documentRect.left;
	else if (r.x > documentRect.right)
		r.x = documentRect.right;
	if (r.y < documentRect.top)
		r.y = documentRect.top;
	else if (r.y > documentRect.bottom)
		r.y = documentRect.bottom;

	return r;
}
