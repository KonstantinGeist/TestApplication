// Copyright (C) The Company

#pragma once

#include "Canvas.h"
#include "ICanvas.h"
#include "IShapeSelector.h"
#include "History.h"
#include "ShapePresentation.h"

namespace pres_model
{

	class CCanvasPresentation :
		public ICanvas,
		public IShapeSelector,
		public domain_model::ICanvasListener
	{
	public:
		CCanvasPresentation(domain_model::CCanvas* canvas);

		// Implements ICanvas.

		virtual void AddShape(const std::string& typeName) override;
		virtual void RemoveShape(std::shared_ptr<IShape> shape) override;
		virtual std::shared_ptr<IShape> GetShape(int index) const override;
		virtual int GetShapeIndex(IShape* shape) const override;
		virtual int GetShapeCount() const override;
		virtual std::shared_ptr<IShape> GetSelection() const override;
		virtual void SetSelection(std::shared_ptr<IShape> newSelection) override;
		virtual void AddMarker(EMarkerKind kind, const CRect& rect) override;
		virtual void RemoveMarker(std::shared_ptr<IMarker> marker) override;
		virtual int GetMarkerCount()  const override;
		virtual std::shared_ptr<IMarker> GetMarker(int index) const override;
		virtual CRect GetRect() const override;
		virtual void SetListener(pres_model::ICanvasListener* listener) override;
		virtual IHistory* GetHistory() const override;

	private:

		// Implements IShapeSelector

		virtual void SelectShape(std::shared_ptr<domain_model::CShape> shape) override;

		// Implements DomainModel.ICanvasListener

		virtual void OnShapeAdded(std::shared_ptr<domain_model::CShape> shape, int index) override;
		virtual void OnShapeRemoved(std::shared_ptr<domain_model::CShape> shape, int index) override;
		virtual void OnRectChanged(const CRect& newRect) override;

	private:
		domain_model::CCanvas* m_canvas;
		std::unique_ptr<CHistory> m_history;
		pres_model::ICanvasListener* m_listener;
		std::vector<std::shared_ptr<CShapePresentation>> m_shapePresentations;
		std::unordered_map<std::shared_ptr<domain_model::CShape>, std::shared_ptr<CShapePresentation>> m_shapeToPresentation;
		std::vector<std::shared_ptr<IMarker>> m_markers;
		std::shared_ptr<IShape> m_selection;
	};

}
