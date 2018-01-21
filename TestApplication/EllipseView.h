// Copyright (C) The Company

#pragma once

#include "ConvexShapeView.h"

class CEllipseView final : public CConvexShapeView
{
public:
	virtual const std::string GetTypeName() const override;
	virtual bool HitTest(const CPoint& p) const override;
	virtual void Render(CDC* pDC, ShapeViewLayer layer)  override;
};
