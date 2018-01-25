// Copyright (C) The Company

#pragma once

// Base class for shapes (model).
class IShape
{
public:
	virtual ~IShape() = default;

	// Poor man's reflection.
	// TODO Does Visual C++/MFC have something better? Is their RTTI reliable?
	virtual const std::string GetTypeName() const = 0;
};

// CConvexShape
class CConvexShape: public IShape
{
public:
	CRect Rect;
};
