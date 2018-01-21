// Copyright (C) The Company

#include "stdafx.h"
#include "Action.h"

// ******************
//   CSelectAction
// ******************

CSelectAction::CSelectAction(const CPoint& p)
	: m_p(p)
{
}

bool CSelectAction::Do(CDocumentView* docView)
{
	m_prevSelection = docView->GetSelected();
	bool b = docView->Select(m_p);
	if (b)
		m_newSelection = docView->GetSelected();
	return b;
}

void CSelectAction::Redo(CDocumentView* docView)
{
	docView->Select(m_newSelection);
}

void CSelectAction::Undo(CDocumentView* docView)
{
	docView->Select(m_prevSelection);
}

// ******************
//   CEndDragAction
// ******************

CEndDragAction::CEndDragAction(const CPoint& dragEndPosition)
	: m_dragEndPosition(dragEndPosition), m_curMarkerIndex(-1)
{

}

bool CEndDragAction::Do(CDocumentView* docView)
{
	bool b = docView->EndDrag(m_dragEndPosition);
	if (b)
	{
		m_shapeView = docView->GetSelected();
		assert(m_shapeView);
		m_dragStartPosition = docView->GetDragStartPosition();
		m_dragEndPosition = docView->GetLastDragPosition();
		m_curMarkerIndex = docView->GetCurrentMarkerIndex();
	}
	return b;
}

void CEndDragAction::Redo(CDocumentView* docView)
{
	assert(m_shapeView);

	if (m_curMarkerIndex == -1)
	{
		// Dragging the shape.

		m_shapeView->SetPosition(m_dragEndPosition);
	}
	else
	{
		// Dragging a marker.

		m_shapeView->SetMarkerPosition(m_curMarkerIndex, m_dragEndPosition);
	}

	docView->MarkChanged();
}

void CEndDragAction::Undo(CDocumentView* docView)
{
	if (m_curMarkerIndex == -1)
	{
		// Dragging the shape.

		m_shapeView->SetPosition(m_dragStartPosition);
	}
	else
	{
		// Dragging a marker.

		m_shapeView->SetMarkerPosition(m_curMarkerIndex, m_dragStartPosition);
	}

	docView->MarkChanged();
}

// ******************
//   CDeleteAction
// ******************

CDeleteAction::CDeleteAction()
	: m_index(-1), m_wasSelected(false)
{
}

bool CDeleteAction::Do(CDocumentView* docView)
{
	auto selected = docView->GetSelected();
	m_shapeView = docView->RemoveSelected(&m_index);
	// Selection is implicitly unset when an object is deleted.
	// We must restore it in Redo(..)
	m_wasSelected = m_shapeView == selected;
	return m_shapeView != nullptr;
}

void CDeleteAction::Redo(CDocumentView* docView)
{
	assert(m_index != -1);
	docView->RemoveShapeViewAt(m_index);
}

void CDeleteAction::Undo(CDocumentView* docView)
{
	assert(m_shapeView && m_index != -1);
	docView->InsertShapeView(m_shapeView, m_index);

	if (m_wasSelected)
		docView->Select(m_shapeView);
}
