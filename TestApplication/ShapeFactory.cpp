// Copyright (C) The Company

#include "stdafx.h"
#include "ShapeFactory.h"

#include "Triangle.h"
#include "Rectangle.h"
#include "Ellipse.h"

namespace domain_model
{

	CShape* CShapeFactory::CreateInstance(const std::string& typeName)
	{
		// TODO use something more clever

		if (typeName == "Triangle")
			return new CTriangle();
		else if (typeName == "Rectangle")
			return new CRectangle();
		else if (typeName == "Ellipse")
			return new CEllipse();

		throw std::out_of_range("unknown shape type");
	}

}
