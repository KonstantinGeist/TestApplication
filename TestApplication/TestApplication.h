// Copyright (C) The Company

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "Presenter.h"

// Partly wizard-generated.
class CTestApplicationApp : public CWinAppEx
{
public:
	CTestApplicationApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	virtual void PreLoadState();

	DECLARE_MESSAGE_MAP()

public:
	// Use this if you want undoable actions.
	CPresenter& GetPresenter()
	{
		return m_presenter;
	}

private:
	CPresenter m_presenter;
};

extern CTestApplicationApp theApp;
