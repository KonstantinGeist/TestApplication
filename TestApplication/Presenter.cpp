// Copyright (C) The Company

#include "stdafx.h"

#include "Presenter.h"
#include "Rectangle.h"
#include "RectangleView.h"
#include "TriangleView.h"
#include "EllipseView.h"
#include "NativeView.h"
#include "NativeDocument.h"

// Uses same license as boost.
// Used it because it's a simple two-file dependency.
#include "tinyxml2.h"

// *********************
//    Initialization.
// *********************

CPresenter::CPresenter()
	: m_nativeView(nullptr), m_nativeDocument(nullptr),
	  m_docViewChangeEnabled(true)
{
	// Registers built-in mappers.
	RegisterShapeMapper(std::make_unique<CRectangleMapper>());
	RegisterShapeMapper(std::make_unique<CTriangleMapper>());
	RegisterShapeMapper(std::make_unique<CEllipseMapper>());
}

void CPresenter::RegisterShapeMapper(std::unique_ptr<IShapeMapper> mapper)
{
	m_shapeMappers[mapper->GetTypeName()] = std::move(mapper);
}

void CPresenter::SetDocumentView(std::shared_ptr<IDocumentView> docView)
{
	m_docView = docView;
	m_docView->SetEventHandler(this);
}

void CPresenter::SetDocumentModel(std::shared_ptr<IDocumentModel> docModel)
{
	m_docModel = docModel;

	modelToView();
}

void CPresenter::SetNativeView(INativeView* nativeView)
{
	m_nativeView = nativeView;
}

void CPresenter::SetNativeDocument(INativeDocument* nativeDocument)
{
	m_nativeDocument = nativeDocument;
}

// *********************
//      Undo/Redo.
// *********************

const CUndoContext CPresenter::getUndoContext() const
{
	assert(m_docView);
	return CUndoContext(m_docView.get());
}

void CPresenter::Do(std::shared_ptr<CAction> action)
{
	assert(m_docModel);

	const CUndoContext ctx (getUndoContext());

	if (!action->Do(ctx))
		return; // cancelled mid-through

	m_actionsToUndo.push_back(action);
	m_actionsToRedo.clear(); // NOTE _doing_ something destroys our Redo queue

	m_nativeView->Refresh();
}

bool CPresenter::Undo()
{
	assert(m_docModel);

	if (!CanUndo())
		return false;

	auto action = m_actionsToUndo.back();
	m_actionsToUndo.pop_back();

	const CUndoContext ctx (getUndoContext());
	action->Undo(ctx);
	m_actionsToRedo.push_back(action);

	m_nativeView->Refresh();

	return true;
}

bool CPresenter::Redo()
{
	assert(m_docModel);

	if (!CanRedo())
		return false;

	auto action = m_actionsToRedo.back();
	m_actionsToRedo.pop_back();

	const CUndoContext ctx (getUndoContext());
	action->Redo(ctx);
	m_actionsToUndo.push_back(action);

	m_nativeView->Refresh();

	return true;
}

bool CPresenter::CanUndo() const
{
	return !m_actionsToUndo.empty();
}

bool CPresenter::CanRedo() const
{
	return !m_actionsToRedo.empty();
}

void CPresenter::ClearHistory()
{
	m_actionsToUndo.clear();
	m_actionsToRedo.clear();
}

// *******************
//   Known actions.
// *******************

void CPresenter::DoDelete()
{
	Do(std::make_shared<CDeleteAction>());
}

void CPresenter::DoAddRectangle()
{
	Do(std::make_shared<CAddAction<CRectangleView>>());
}

void CPresenter::DoAddTriangle()
{
	Do(std::make_shared<CAddAction<CTriangleView>>());
}

void CPresenter::DoAddEllipse()
{
	Do(std::make_shared<CAddAction<CEllipseView>>());
}

void CPresenter::Select(const CPoint& p)
{
	if (m_docView->Select(p))
		m_nativeView->Refresh();
}

void CPresenter::Drag(const CPoint& p)
{
	assert(m_docView);
	if (m_docView->Drag(p))
	{
		// Dragging does not register in the history and therefore Refresh(..)
		// as part of the usual routine is not called. So we call it here.
		m_nativeView->Refresh();
	}
}

void CPresenter::DoEndDrag(const CPoint& p)
{
	Do(std::make_shared<CEndDragAction>(p));
}

void CPresenter::Hover(const CPoint& p)
{
	m_docView->Hover(p);
}

// *******************
//    Conversions.
// *******************

void CPresenter::modelToView()
{
	// See viewToModel()

	// to prevent infinite recursion when we change document view
	m_docViewChangeEnabled = false;

	m_docView->Clear();

	const auto& shapes = m_docModel->GetShapes();
	for (auto shape : shapes)
	{
		std::string typeName = shape->GetTypeName();
		auto shapeMapper = m_shapeMappers[typeName].get();

		auto shapeView = std::shared_ptr<IShapeView>(shapeMapper->ConvertModelToView(shape.get()));
		assert(shapeView);
		m_docView->AddShapeView(shapeView);
	}

	m_docViewChangeEnabled = true;
}

void CPresenter::viewToModel()
{
	// Currently, we simply re-fill the document model on every change.
	// Theoretically, can be more sophisticated, but is enough for our current task.

	m_docModel->Clear();

	const auto& shapeViews = m_docView->GetShapeViews();
	for (auto shapeView : shapeViews)
	{
		std::string typeName = shapeView->GetTypeName();
		auto shapeMapper = m_shapeMappers[typeName].get();

		auto shape = std::shared_ptr<IShape>(shapeMapper->ConvertViewToModel(shapeView.get()));
		assert(shape);
		m_docModel->AddShape(shape);
	}
}

// ************************************
// Implements IDocumentViewEventHandler
// ************************************

void CPresenter::OnDocumentViewChange(IDocumentView* /*docView*/)
{
	if (!m_docViewChangeEnabled)
		return;

	viewToModel();

	assert(m_nativeDocument);
	m_nativeDocument->SetModified(true);
}

void CPresenter::OnBeginShapeViewDrag(IDocumentView* /*docView*/)
{
	assert(m_nativeView);
	m_nativeView->SetHandCursor();
	m_nativeView->SetCapture(true);
}

void CPresenter::OnEndShapeViewDrag(IDocumentView* /*docView*/)
{
	assert(m_nativeView);
	m_nativeView->SetDefaultCursor();
	m_nativeView->SetCapture(false);
}

void CPresenter::OnBeginMarkerHover(IDocumentView* docView)
{
	m_nativeView->SetMarkerCursor(docView->GetCurrentMarkerCursor());
}

void CPresenter::OnEndMarkerHover(IDocumentView* docView)
{
	m_nativeView->SetDefaultCursor();
}

void CPresenter::OnBeginMarkerDrag(IDocumentView* docView)
{
	m_nativeView->SetCapture(true);
}

void CPresenter::OnEndMarkerDrag(IDocumentView* docView)
{
	m_nativeView->SetCapture(false);
}

// ******************
//   Serialization.
// ******************

// TODO in a perfect world, serialization logic could also be abstracted away from XML
// but we've already over-engineered our little program too much
bool CPresenter::Serialize(const std::string& path)
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLNode* shapesNode = xmlDoc.NewElement("Shapes");
	xmlDoc.InsertFirstChild(shapesNode);

	const auto& shapes = m_docModel->GetShapes();
	for (const auto shape : shapes)
	{
		std::string typeName = shape->GetTypeName();
		auto shapeMapper = m_shapeMappers[typeName].get();

		tinyxml2::XMLElement* element = shapeMapper->ConvertModelToXMLElement(shape.get(), xmlDoc);
		assert(element);

		shapesNode->InsertEndChild(element);
	}

	tinyxml2::XMLError eResult = xmlDoc.SaveFile(path.c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
		return false;

	return true;
}

bool CPresenter::Deserialize(const std::string& path)
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError eResult = xmlDoc.LoadFile(path.c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLNode* shapesNode = xmlDoc.FirstChild();
	if (!shapesNode)
		return false;

	m_docModel->Clear();

	for (auto e = shapesNode->FirstChildElement();
		 e != nullptr;
		 e = e->NextSiblingElement())
	{
		std::string typeName = e->Name();
		auto shapeMapper = m_shapeMappers[typeName].get();
		auto shape = std::shared_ptr<IShape>(shapeMapper->ConvertXMLElementToModel(e));
		assert(shape);
		m_docModel->AddShape(shape);
	}

	modelToView();

	m_nativeView->Refresh();

	this->ClearHistory();

	return true;
}
