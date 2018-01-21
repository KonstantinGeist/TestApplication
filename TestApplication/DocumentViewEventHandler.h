// Copyright (C) The Company

#pragma once

class CDocumentView;

// Generates CDocumentView-related events. Such events can be reported
// back to Presenter to update native (MFC) view (or anything else).
// TODO The interface can be more elaborate, uses what's exactly required
// at this point.
class IDocumentViewEventHandler
{
public:
	// Called whenever shape views of the document view change.
	virtual void OnDocumentViewChange(CDocumentView* docView) = 0;

	// Called when shape dragging begins.
	virtual void OnBeginShapeViewDrag(CDocumentView* docView) = 0;

	// Called when shape dragging ends.
	virtual void OnEndShapeViewDrag(CDocumentView* docView) = 0;

	// Called when hovering over a marker.
	virtual void OnBeginMarkerHover(CDocumentView* docView) = 0;

	// Called when hovering over a marker ended.
	virtual void OnEndMarkerHover(CDocumentView* docView) = 0;

	// Called when marker drag begins.
	virtual void OnBeginMarkerDrag(CDocumentView* docView) = 0;

	// Called when marker drag ends.
	virtual void OnEndMarkerDrag(CDocumentView* docView) = 0;
};

