// Copyright (C) The Company

#pragma once

// Implements double buffering.
// TODO the bitmap can be reused IIRC
class CDoubleBufferedDC final : public CDC {
public:
	CDoubleBufferedDC(CDC* pDC, const CRect* pRect = NULL);
	~CDoubleBufferedDC();

	CDoubleBufferedDC* operator->()
	{
		return this;
	}

	operator CDoubleBufferedDC*()
	{
		return this;
	}

private:
	CBitmap    m_bitmap;
	CBitmap*   m_oldBitmap;
	CDC*       m_pDC;
	CRect      m_rect;
	BOOL       m_bMemDC;
};
