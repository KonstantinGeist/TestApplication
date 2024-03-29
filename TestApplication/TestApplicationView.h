// Copyright (C) The Company

#pragma once

#include "ICanvasView.h"

class CTestApplicationView : public CView, public view::ICanvasView
{
protected: // create from serialization only
	CTestApplicationView();
	DECLARE_DYNCREATE(CTestApplicationView)

public:
	~CTestApplicationView();

protected:
	virtual void OnDraw(CDC* pDC) override;

// Generated message map functions
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()

public:

	// Implements view::ICanvasView

	virtual std::shared_ptr<view::IShapeView> InsertShapeView(const std::string& typeName, int index) override;
	virtual void RemoveShapeView(std::shared_ptr<view::IShapeView> view) override;
	virtual void SelectShapeView(std::shared_ptr<view::IShapeView> view) override;
	virtual std::shared_ptr<view::IMarkerView> InsertMarkerView() override;
	virtual void RemoveMarkerView(std::shared_ptr<view::IMarkerView> view) override;
	virtual void SetCursor(view::ECanvasCursor cursor) override;
	virtual view::ECanvasCursor GetCursor() const override;
	virtual void SetMouseCapture(bool b) override;
	virtual void SetRect(const CRect& rect) override;
	virtual void Invalidate() override;
	virtual void SetListener(view::ICanvasViewListener* listener) override;

private:

	std::vector<std::shared_ptr<view::IShapeView>> m_shapeViews;
	std::vector<std::shared_ptr<view::IMarkerView>> m_markerViews;
	CRect m_rect;
	std::shared_ptr<view::IShapeView> m_selected;
	view::ICanvasViewListener* m_listener;
	LPTSTR m_curIcon;
	view::ECanvasCursor m_cursor;
};
