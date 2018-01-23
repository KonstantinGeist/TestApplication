// Copyright (C) The Company

#pragma once

class IDocumentView;

// Generates IDocumentView-related events. Such events can be reported
// back to Presenter to update native (MFC) view (or anything else).
// TODO The interface can be more elaborate, uses what's exactly required
// at this point.
class IDocumentViewEventHandler
{
public:
	// Called whenever shape views of the document view change.
	virtual void OnDocumentViewChange(IDocumentView* docView) = 0;

	// Called when shape dragging begins.
	virtual void OnBeginShapeViewDrag(IDocumentView* docView) = 0;

	// Called when shape dragging ends.
	virtual void OnEndShapeViewDrag(IDocumentView* docView) = 0;

	// Called when hovering over a marker.
	virtual void OnBeginMarkerHover(IDocumentView* docView) = 0;

	// Called when hovering over a marker ended.
	virtual void OnEndMarkerHover(IDocumentView* docView) = 0;

	// Called when marker drag begins.
	virtual void OnBeginMarkerDrag(IDocumentView* docView) = 0;

	// Called when marker drag ends.
	virtual void OnEndMarkerDrag(IDocumentView* docView) = 0;
};

