// Copyright (C) The Company

#include "stdafx.h"
#include "ShapeViewFactory.h"
#include "TriangleView.h"
#include "RectangleView.h"
#include "EllipseView.h"

namespace view
{

	IShapeView* CShapeViewFactory::CreateInstance(const std::string& typeName)
	{
		// TODO this factory should belong to a ICanvasView impl so that CDC was
		// passed to ctor and not exposed in the interfaces (such as Render(CDC* pDC)?

		if (typeName == "Triangle")
			return new CTriangleView();
		else if (typeName == "Rectangle")
			return new CRectangleView();
		else if (typeName == "Ellipse")
			return new CEllipseView();

		throw std::out_of_range("unknown shape view type");
	}

}
