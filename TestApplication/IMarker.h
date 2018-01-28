// Copyright (C) The Company

#pragma once

namespace pres_model
{

	// Kinds of markers known to the application model.
	enum class EMarkerKind
	{
		// Resizing markers.

		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight

		// + add other markers
	};

	class IMarkerListener
	{
	public:
		virtual void OnRectChanged(const CRect& newRect) = 0;

		virtual ~IMarkerListener() = default;
	};

	// Markers are helper objects that can be used to manipulate shapes. The most
	// common type of marker is the resizing marker.
	// Presenters/views are free to ignore markers they do not understand/not interested in.
	//
	// There can be other kinds of markers other than the usual resizing markers,
	// so they were abstracted into their own category as well. Selection itself
	// is part of the application model, why not have markers here too?
	class IMarker
	{
	public:
		virtual EMarkerKind GetKind() const = 0;

		virtual CRect GetRect() const = 0;
		virtual void SetRect(const CRect& rect) = 0;

		virtual void SetListener(IMarkerListener* listener) = 0;

		virtual ~IMarker() = default;
	};

}
