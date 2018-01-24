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

bool CEndDragAction::Do(const CUndoContext& ctx)
{
	auto docView = ctx.DocumentView;

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

void CEndDragAction::Redo(const CUndoContext& ctx)
{
	assert(m_shapeView);

	auto docView = ctx.DocumentView;

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

void CEndDragAction::Undo(const CUndoContext& ctx)
{
	auto docView = ctx.DocumentView;

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

bool CDeleteAction::Do(const CUndoContext& ctx)
{
	auto docView = ctx.DocumentView;

	m_shapeView = docView->RemoveSelected(&m_index);
	return m_shapeView != nullptr;
}

void CDeleteAction::Redo(const CUndoContext& ctx)
{
	assert(m_index != -1);
	ctx.DocumentView->RemoveShapeViewAt(m_index);
}

void CDeleteAction::Undo(const CUndoContext& ctx)
{
	assert(m_shapeView && m_index != -1);

	auto docView = ctx.DocumentView;

	docView->InsertShapeView(m_shapeView, m_index);
	docView->Select(m_shapeView);
}
