// Copyright (C) The Company

#include "stdafx.h"
#include "ShapeViewFactory.h"
#include "TriangleView.h"
#include "RectangleView.h"
#include "EllipseView.h"

namespace view
{

	CConcreteShapeViewBase* CShapeViewFactory::CreateInstance(const std::string& typeName)
	{
		// TODO something more clever

		if (typeName == "Triangle")
			return new CTriangleView();
		else if (typeName == "Rectangle")
			return new CRectangleView();
		else if (typeName == "Ellipse")
			return new CEllipseView();

		throw std::out_of_range("unknown shape view type");
	}

}
