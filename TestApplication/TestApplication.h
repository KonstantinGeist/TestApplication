// Copyright (C) The Company

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "Canvas.h"
#include "CanvasPresentation.h"
#include "CanvasPresenter.h"

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
	// TODO I'm not sure where all these objects should be better put in MFC,
	// so I put them here, as theApp is a global variable which can be
	// accessed from anywhere.
	std::unique_ptr<domain_model::CCanvas> Canvas;
	std::unique_ptr<pres_model::CCanvasPresentation> CanvasPresentation;
	std::unique_ptr<presenter::CCanvasPresenter> CanvasPresenter;
};

extern CTestApplicationApp theApp;
