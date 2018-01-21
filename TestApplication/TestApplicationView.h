// Copyright (C) The Company

#pragma once

#include "DocumentView.h"
#include "NativeView.h"

// MFC's View which redirects Windows-specific events to CDocumentView.
// To talk back, we use INativeView to decouple MFC-specific things
// from Presenter.
// Partly wizard-generated.
class CTestApplicationView : public CView, public INativeView
{
protected: // create from serialization only
	CTestApplicationView();
	DECLARE_DYNCREATE(CTestApplicationView)

// Overrides
public:
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
	afx_msg void OnAddRectangle();
	afx_msg void OnAddTriangle();
	afx_msg void OnAddEllipse();

	afx_msg void OnUndo();
	afx_msg void OnRedo();

// Implements INativeView
public:
	virtual void SetDefaultCursor() override;
	virtual void SetHandCursor() override;
	virtual void SetCapture(bool b) override;
	virtual void SetMarkerCursor(MarkerCursor markerCursor) override;
	virtual void Refresh() override;

private:
	std::shared_ptr<CDocumentView> m_docView;
	LPTSTR m_curIcon;
	bool m_mouseDown;
};
