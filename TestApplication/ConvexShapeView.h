// Copyright (C) The Company

#pragma once

#include "ShapeView.h"

class IDocumentView;

// The parent of basic shapes such as triangles, ellipses and rectangles.
class CConvexShapeView: public IShapeView
{
public:
	CConvexShapeView();
	
	virtual void InitInstance(IDocumentView* docView) override;
	virtual bool HitTest(const CPoint& p) const override;
	virtual CRect GetRect() const override;
	virtual void SetRect(const CRect& rect) override;
	virtual void SetPosition(const CPoint& p) override;
	virtual CPoint GetPosition() const override;
	virtual bool IsSelected() override;
	virtual void SetSelected(bool value) override;
	virtual int GetMarkerCount() const override;
	virtual CRect GetMarkerRect(int index) const override;
	virtual MarkerCursor GetMarkerCursor(int index) const override;
	virtual void SetMarkerPosition(int index, const CPoint& p);
	virtual CPoint GetMarkerPosition(int index) const override;
	virtual void Render(CDC* pDC, ShapeViewLayer layer) override;

private:
	CRect m_rect;
	bool m_isSelected;
};
