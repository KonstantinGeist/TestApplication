// Copyright (C) The Company

#pragma once

#include "Action.h"
#include "DocumentModel.h"
#include "DocumentView.h"
#include "DocumentViewEventHandler.h"
#include "ShapeMapper.h"

class INativeView;
class INativeDocument;

// Glues view/model + provides undo/redo functionality.
// TODO undo/redo could be separated into CHistory if required,
// so far the class is small enough to incorporate it.
// Also manages serialization/deserialization as we reuse shape mappers for that
// (to avoid polluting model data with serialization code and implementation details
// such as tinyxml2).
class CPresenter final: private IDocumentViewEventHandler
{
public:
	// *******************
	//   Initialization.
	// *******************

	CPresenter();

	// Binds the presenter to a document model object.
	void SetDocumentModel(std::shared_ptr<IDocumentModel> docModel);

	// Binds the presenter to a document view object.
	void SetDocumentView(std::shared_ptr<IDocumentView> docView);

	// Connects to the native view (such as CView) to report changes back to it.
	void SetNativeView(INativeView* nativeView);

	// Same as SetNativeView(..) except for the document (such as CDocument).
	void SetNativeDocument(INativeDocument* nativeDocument);

	// *******************
	//      Do/redo.
	// *******************

	// Performs an undoable action which is added to the list.
	void Do(std::shared_ptr<CAction> action);

	// Undoes the previous action. Returns false if nothing was undone.
	bool Undo();

	// Redoes the previously undone action. Returns false if nothing was redone.
	bool Redo();

	// Useful for UI.
	bool CanUndo() const;
	bool CanRedo() const;

	void ClearHistory();

	// *******************
	//   Shape mappers.
	// *******************

	// See IShapeMapper for more details.
	void RegisterShapeMapper(std::unique_ptr<IShapeMapper> mapper);

	// *******************
	//    Serialization.
	// *******************

	// Saves the current file to disk. Returns false on error.
	bool Serialize(const std::string& path);

	// Loads a file from disk. Returns false on error.
	bool Deserialize(const std::string& path);

	// *******************
	//    Known actions.
	// *******************

	// Undoable actions that are added to history (prefix Do-).

	void DoDelete();
	void DoAddRectangle();
	void DoAddTriangle();
	void DoAddEllipse();
	void DoEndDrag(const CPoint& p);

	// Actions that by-pass history: dragging, hovering etc.

	void Select(const CPoint& p);
	void Drag(const CPoint& p);
	void Hover(const CPoint& p);

private:
	// Updates the view with the data from the model.
	void modelToView();
	// Updates the model with the data from the view.
	void viewToModel();

	// Implements the IDocumentViewEventHandler interface to respond to changes
	// in the document view.
	virtual void OnDocumentViewChange(IDocumentView* docView) override;
	virtual void OnBeginShapeViewDrag(IDocumentView* docView) override;
	virtual void OnEndShapeViewDrag(IDocumentView* docView) override;
	virtual void OnBeginMarkerHover(IDocumentView* docView) override;
	virtual void OnEndMarkerHover(IDocumentView* docView) override;
	virtual void OnBeginMarkerDrag(IDocumentView* docView) override;
	virtual void OnEndMarkerDrag(IDocumentView* docView) override;

// Fields
private:
	// TODO can be moved to pimpl

	std::shared_ptr<IDocumentModel> m_docModel;
	std::shared_ptr<IDocumentView> m_docView;
	INativeView* m_nativeView;
	INativeDocument* m_nativeDocument;

	std::list<std::shared_ptr<CAction>> m_actionsToRedo, m_actionsToUndo;
	std::unordered_map<std::string, std::unique_ptr<IShapeMapper>> m_shapeMappers;
	bool m_docViewChangeEnabled;
};
