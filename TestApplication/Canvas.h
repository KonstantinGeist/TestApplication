// Copyright (C) The Company

#pragma once

#include "Shape.h"

namespace domain_model
{

	class ICanvasListener
	{
	public:
		virtual void OnShapeAdded(std::shared_ptr<CShape> shape, int index) = 0;
		virtual void OnShapeRemoved(std::shared_ptr<CShape> shape, int index) = 0;
		virtual void OnRectChanged(const CRect& newRect) = 0;

		virtual ~ICanvasListener() = default;
	};

	class CCanvas final : public IShapeCollection
	{
	public:
		CCanvas();

		CRect GetRect() const;
		void SetRect(const CRect& rect);

		// Implements domain_model::IShapeCollection

		virtual void InsertShape(std::shared_ptr<CShape> shape, int index) override;
		virtual void RemoveShape(std::shared_ptr<CShape> shape) override;
		virtual int GetShapeCount() const override;
		virtual std::shared_ptr<CShape> GetShape(int index) const override;
		virtual void Clear() override;

		void SetListener(ICanvasListener* listener);

	private:
		std::vector<std::shared_ptr<CShape>> m_shapes;
		CRect m_rect;
		ICanvasListener* m_listener;
	};

}
