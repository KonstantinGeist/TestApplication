// Copyright (C) The Company

#pragma once

// Similar to INativeView (see).
class INativeDocument
{
public:
	virtual ~INativeDocument() = default;

	// Tells the native document object that the document was modified.
	// Modification flag is reset on save.
	virtual void SetModified(bool b) = 0;
};
