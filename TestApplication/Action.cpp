// Copyright (C) The Company

#include "stdafx.h"
#include "Action.h"

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

	docView->Select(m_shapeView);

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

	docView->Select(m_shapeView);

	docView->MarkChanged();
}

// ******************
//   CDeleteAction
// ******************

CDeleteAction::CDeleteAction()
	: m_index(-1)
{
}

bool CDeleteAction::Do(CDocumentView* docView)
{
	m_shapeView = docView->RemoveSelected(&m_index);
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

	docView->Select(m_shapeView);
}
