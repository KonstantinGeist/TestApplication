// Copyright (C) The Company

#include "stdafx.h"

#include "TestApplication.h"
#include "TestApplicationDoc.h"

#include "CanvasSerializer.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestApplicationDoc

IMPLEMENT_DYNCREATE(CTestApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestApplicationDoc, CDocument)
END_MESSAGE_MAP()

BOOL CTestApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	theApp.Canvas->Clear();

	auto history = theApp.CanvasPresentation->GetHistory();
	history->Clear();
	history->SetListener(this);

	SetModifiedFlag(FALSE);

	return TRUE;
}

BOOL CTestApplicationDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	const std::string path = CT2A(lpszPathName);
	if (!domain_model::DeserializeCanvas(theApp.Canvas.get(), path))
		return FALSE;

	theApp.CanvasPresentation->GetHistory()->Clear();
	SetModifiedFlag(FALSE);

	return TRUE;
}

BOOL CTestApplicationDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	const std::string path = CT2A(lpszPathName);
	if (!domain_model::SerializeCanvas(theApp.Canvas.get(), path))
		return FALSE;

	SetModifiedFlag(FALSE);

	return TRUE;
}

void CTestApplicationDoc::OnExecute(pres_model::ICommand* newCommand)
{
	SetModifiedFlag(TRUE);
}

void CTestApplicationDoc::OnUnexecute(pres_model::ICommand* newCommand)
{
	SetModifiedFlag(TRUE);
}
