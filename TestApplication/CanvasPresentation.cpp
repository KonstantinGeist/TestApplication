// Copyright (C) The Company

#include "stdafx.h"
#include "CanvasPresentation.h"
#include "ShapeFactory.h"
#include "AddShapeCommand.h"
#include "RemoveShapeCommand.h"
#include "ConcreteMarker.h"

namespace pres_model
{

	CCanvasPresentation::CCanvasPresentation(domain_model::CCanvas* canvas)
		: m_canvas(canvas),
		  m_history(std::make_unique<CHistory>()),
		  m_listener(nullptr)
	{
		m_canvas->SetListener(this);
	}

	// Implements ICanvas.

	void CCanvasPresentation::AddShape(const std::string& typeName)
	{
		auto shape = std::shared_ptr<domain_model::CShape>(domain_model::CShapeFactory::CreateInstance(typeName));

		auto rect = m_canvas->GetRect();
		int center_x = rect.left + (rect.right - rect.left) / 2;
		int center_y = rect.top + (rect.bottom - rect.top) / 2;
		#define DEFAULT_SIZE 100
		shape->SetRect(CRect(
			center_x - DEFAULT_SIZE / 2,
			center_y - DEFAULT_SIZE / 2,
			center_x + DEFAULT_SIZE / 2,
			center_y + DEFAULT_SIZE / 2));

		auto index = m_canvas->GetShapeCount();
		auto command = std::make_unique<CAddShapeCommand>(shape, index, this, m_canvas);
		m_history->AddAndExecute(std::move(command));
	}

	void CCanvasPresentation::RemoveShape(std::shared_ptr<IShape> shape)
	{
		int index = GetShapeIndex(shape.get());
		auto shapePresentation = dynamic_cast<CShapePresentation*>(shape.get()); // TODO
		assert(shapePresentation);

		auto command = std::make_unique<CRemoveShapeCommand>(shapePresentation->GetDomainModel(), index, this, m_canvas);
		m_history->AddAndExecute(std::move(command));
	}

	std::shared_ptr<IShape> CCanvasPresentation::GetShape(int index) const
	{
		return m_shapeToPresentation.at(m_canvas->GetShape(index));
	}

	int CCanvasPresentation::GetShapeIndex(IShape* shape) const
	{
		for(auto it = m_shapePresentations.begin(); it != m_shapePresentations.end(); ++it)
		{
			if (static_cast<IShape*>((*it).get()) == shape)
				return it - m_shapePresentations.begin();
		}

		return -1;
	}

	int CCanvasPresentation::GetShapeCount() const
	{
		return m_canvas->GetShapeCount();
	}

	std::shared_ptr<IShape> CCanvasPresentation::GetSelection() const
	{
		return m_selection;
	}

	void CCanvasPresentation::SetSelection(std::shared_ptr<IShape> newSelection)
	{
		if (m_selection != newSelection)
		{
			m_selection = newSelection;

			if (m_listener)
				m_listener->OnSelectionChanged(m_selection);
		}
	}

	void CCanvasPresentation::AddMarker(EMarkerKind kind, const CRect& rect)
	{
		auto concreteMarker = std::make_shared<CConcreteMarker>(kind, rect);
		m_markers.push_back(concreteMarker);

		if (m_listener)
			m_listener->OnMarkerAdded(concreteMarker);
	}

	void CCanvasPresentation::RemoveMarker(std::shared_ptr<IMarker> marker)
	{
		auto it = std::find(m_markers.begin(), m_markers.end(), marker);
		assert(it != m_markers.end());
		m_markers.erase(it);

		if (m_listener)
			m_listener->OnMarkerRemoved(marker);
	}

	int CCanvasPresentation::GetMarkerCount()  const
	{
		return m_markers.size();
	}

	std::shared_ptr<IMarker> CCanvasPresentation::GetMarker(int index) const
	{
		return m_markers[index];
	}

	CRect CCanvasPresentation::GetRect() const
	{
		return m_canvas->GetRect();
	}

	IHistory* CCanvasPresentation::GetHistory() const
	{
		return m_history.get();
	}

	void CCanvasPresentation::SetListener(pres_model::ICanvasListener* listener)
	{
		m_listener = listener;
	}

	// Implements IShapeSelector.

	void CCanvasPresentation::SelectShape(std::shared_ptr<domain_model::CShape> shape)
	{
		std::shared_ptr<CShapePresentation> newSelection;

		if (shape)
		{
			auto it = m_shapeToPresentation.find(shape);
			if (it != m_shapeToPresentation.end())
				newSelection = it->second;
		}

		SetSelection(newSelection);
	}

	// Implements DomainModel.ICanvasListener

	void CCanvasPresentation::OnShapeAdded(std::shared_ptr<domain_model::CShape> shape, int index)
	{
		auto shapePresentation = std::make_shared<CShapePresentation>(shape, m_history.get(), this);

		m_shapePresentations.insert(m_shapePresentations.begin() + index, shapePresentation);
		m_shapeToPresentation[shape] = shapePresentation;

		if (m_listener)
			m_listener->OnShapeAdded(shapePresentation, index);
	}

	void CCanvasPresentation::OnShapeRemoved(std::shared_ptr<domain_model::CShape> shape, int index)
	{
		auto shapePresentation = m_shapeToPresentation[shape];
		m_shapePresentations.erase(m_shapePresentations.begin() + index);
		m_shapeToPresentation.erase(shape);

		if (m_listener)
			m_listener->OnShapeRemoved(shapePresentation, index);
	}

	void CCanvasPresentation::OnRectChanged(const CRect& newRect)
	{
		if (m_listener)
			m_listener->OnRectChanged(newRect);
	}

}
