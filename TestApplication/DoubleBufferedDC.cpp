// Copyright (C) The Company

#include "stdafx.h"

#include "DoubleBufferedDC.h"

CDoubleBufferedDC::CDoubleBufferedDC(CDC* pDC, const CRect* pRect)
{
	ASSERT(pDC != NULL);

	// Some initialization.
	m_pDC = pDC;
	m_oldBitmap = NULL;
	m_bMemDC = !pDC->IsPrinting();

	// Get the rectangle to draw.
	if (pRect == NULL) {
		pDC->GetClipBox(&m_rect);
	}
	else {
		m_rect = *pRect;
	}

	if (m_bMemDC) {
		// Creates a memory DC.
		CreateCompatibleDC(pDC);
		pDC->LPtoDP(&m_rect);

		m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(),
			m_rect.Height());
		m_oldBitmap = SelectObject(&m_bitmap);

		SetMapMode(pDC->GetMapMode());

		SetWindowExt(pDC->GetWindowExt());
		SetViewportExt(pDC->GetViewportExt());

		pDC->DPtoLP(&m_rect);
		SetWindowOrg(m_rect.left, m_rect.top);
	}
	else {
		// Make a copy of the relevent parts of the current 
		// DC for printing.
		m_bPrinting = pDC->m_bPrinting;
		m_hDC = pDC->m_hDC;
		m_hAttribDC = pDC->m_hAttribDC;
	}

	// Fill background 
	FillSolidRect(m_rect, pDC->GetBkColor());
}

CDoubleBufferedDC::~CDoubleBufferedDC()
{
	if (m_bMemDC) {
		// Copies the offscreen bitmap onto the screen.
		m_pDC->BitBlt(m_rect.left, m_rect.top,
			m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);

		//Swaps back the original bitmap.
		SelectObject(m_oldBitmap);
	}
	else {
		// All we need to do is replace the DC with an illegal
		// value, this keeps us from accidentally deleting the 
		// handles associated with the CDC that was passed to 
		// the constructor.              
		m_hDC = m_hAttribDC = NULL;
	}
}
