// Copyright (C) The Company

#include "stdafx.h"
#include "Canvas.h"

namespace domain_model
{

	CCanvas::CCanvas()
		: m_rect(0, 0, 640, 480),
		  m_listener(nullptr)
	{
	}

	CRect CCanvas::GetRect() const
	{
		return m_rect;
	}

	void CCanvas::SetRect(const CRect& rect)
	{
		m_rect = rect;

		if (m_listener)
			m_listener->OnRectChanged(rect);
	}

	void CCanvas::InsertShape(std::shared_ptr<CShape> shape, int index)
	{
		m_shapes.insert(m_shapes.begin() + index, shape);

		if (m_listener)
			m_listener->OnShapeAdded(shape, index);
	}

	void CCanvas::RemoveShape(std::shared_ptr<CShape> shape)
	{
		auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
		assert(it != m_shapes.end());

		int index = it - m_shapes.begin();
		m_shapes.erase(it);

		if (m_listener)
			m_listener->OnShapeRemoved(shape, index);
	}

	int CCanvas::GetShapeCount() const
	{
		return m_shapes.size();
	}

	std::shared_ptr<CShape> CCanvas::GetShape(int index) const
	{
		return m_shapes[index];
	}

	void CCanvas::Clear()
	{
		auto shapesCopy = m_shapes;

		for (auto& shape : shapesCopy)
		{
			this->RemoveShape(shape);
		}
	}

	void CCanvas::SetListener(ICanvasListener* listener)
	{
		m_listener = listener;
	}

}
