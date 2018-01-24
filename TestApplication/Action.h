// Copyright (C) The Company

#pragma once

#include "DocumentModel.h"
#include "DocumentView.h"

#include "Rectangle.h"

struct CUndoContext
{
	IDocumentView* DocumentView;

	explicit CUndoContext(IDocumentView* docView)
		: DocumentView(docView)
	{
	}
};

// Base class for implementing undoable actions.
// This system saves memory but is more error-prone.
// Another approach could be just take snapshots of the entire model.
// It depends on what kind of model we have (large/has open handles vs. small).
// Part of Presenter/Controller.
class CAction
{
public:
	// Returns false if the action was cancelled mid-through and should not
	// be registered in the history.
	virtual bool Do(const CUndoContext& ctx) = 0;

	// Defined same as ::Do(..) but some actions may require a Redo(..)
	// different from ::Do(..)
	virtual void Redo(const CUndoContext& ctx)
	{
		this->Do(ctx);
	}

	virtual void Undo(const CUndoContext& ctx) = 0;
};

// Base class for adding shapes of different types.
template <class V>
class CAddAction final: public CAction
{
public:
	virtual bool Do(const CUndoContext& ctx) override
	{
		auto docView = ctx.DocumentView;

		m_shapeView = std::make_shared<V>();
		m_shapeView->InitInstance(docView);

		docView->AddShapeView(m_shapeView);

		return true;
	}

	virtual void Redo(const CUndoContext& ctx) override
	{
		auto docView = ctx.DocumentView;

		docView->AddShapeView(m_shapeView);
		docView->Select(m_shapeView);
	}

	virtual void Undo(const CUndoContext& ctx) override
	{
		ctx.DocumentView->RemoveShapeView(m_shapeView);
	}

private:
	std::shared_ptr<V> m_shapeView;
};

// End drag event.
class CEndDragAction final : public CAction
{
public:
	CEndDragAction(const CPoint& dragEndPosition);

	virtual bool Do(const CUndoContext& ctx) override;
	virtual void Redo(const CUndoContext& ctx) override;
	virtual void Undo(const CUndoContext& ctx) override;

private:
	CPoint m_dragStartPosition, m_dragEndPosition;
	std::shared_ptr<IShapeView> m_shapeView;
	int m_curMarkerIndex;
};

// Delete event.
class CDeleteAction final : public CAction
{
public:
	CDeleteAction();

	virtual bool Do(const CUndoContext& ctx) override;
	virtual void Redo(const CUndoContext& ctx) override;
	virtual void Undo(const CUndoContext& ctx) override;

private:
	std::shared_ptr<IShapeView> m_shapeView;
	int m_index;
};
