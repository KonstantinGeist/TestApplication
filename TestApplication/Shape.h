// Copyright (C) The Company

#pragma once

namespace domain_model
{

	class IShapeListener
	{
	public:
		virtual void OnRectChanged(const CRect& newRect) = 0;

		virtual ~IShapeListener() = default;
	};

	class CShape
	{
	public:
		CShape();
		virtual ~CShape() = default;

		virtual std::string GetTypeName() const = 0;

		CRect GetRect() const;
		void SetRect(const CRect& rect);

		// NOTE Previously, HitTest was part of IShapeView; now that views are "dumb" UI
		// without any logic, I figured it's not good to retrieve any kind of state from
		// views and rely on it -- instead, now HitTest(..) has been moved to the model
		// layer so now this information can be queryable by all interested parties without
		// relying on UI. Indeed, there are already GetRect()/SetRect() to get an approximate
		// rectangle around a shape; ContainsPoint is a natural extension of it which defines
		// a shape more precisely (purely mathematically).
		virtual bool ContainsPoint(const CPoint& p) const;

		void SetListener(IShapeListener* listener);

	private:
		CRect m_rect;
		IShapeListener* m_listener;
	};

	class IShapeCollection
	{
	public:
		virtual void InsertShape(std::shared_ptr<CShape> shape, int index) = 0;
		virtual void RemoveShape(std::shared_ptr<CShape> shape) = 0;
		virtual int GetShapeCount() const = 0;
		virtual std::shared_ptr<CShape> GetShape(int index) const = 0;
		virtual void Clear() = 0;

		virtual ~IShapeCollection() = default;
	};

}
