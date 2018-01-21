// Copyright (C) The Company

#pragma once

// Presenter is abstracted not to touch anything MFC-related directly.
// Instead, it accesses TestApplicationView via INativeView.
// 1) Clean interfaces without garbage.
// 2) Easier to port outside of MFC.
// 3) Testing?
class INativeView
{
public:
	virtual void SetDefaultCursor() = 0;
	virtual void SetHandCursor() = 0;
	virtual void SetMarkerCursor(MarkerCursor markerCursor) = 0;
	virtual void SetCapture(bool b) = 0;
	virtual void Refresh() = 0;
};
