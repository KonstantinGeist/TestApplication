// Copyright (C) The Company

#pragma once

#include "Shape.h"

class CEllipse : public CConvexShape
{
public:
	virtual const std::string GetTypeName() const override
	{
		return "Ellipse";
	}
};
