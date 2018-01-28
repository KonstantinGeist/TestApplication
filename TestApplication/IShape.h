// Copyright (C) The Company

#pragma once

namespace pres_model
{

	class IShapeListener
	{
	public:
		virtual void OnRectChanged(const CRect& newRect) = 0;

		virtual ~IShapeListener() = default;
	};

	class IShape
	{
	public:
		virtual std::string GetTypeName() const = 0;

		virtual CRect GetRect() const = 0;
		virtual void SetRect(const CRect& rect) = 0;

		// Commits the current rect to history.
		// TODO is this OK? feels a bit dirty
		virtual void CommitRect(const CRect& prevRect) = 0;

		virtual bool ContainsPoint(const CPoint& p) const = 0;

		virtual void SetListener(IShapeListener* listener) = 0;

		virtual ~IShape() = default;
	};

}
