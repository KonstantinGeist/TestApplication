// Copyright (C) The Company

#include "stdafx.h"

#include "TestApplication.h"
#include "TestApplicationDoc.h"

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

	m_docModel = std::make_shared<CDocumentModel>();
	auto& presenter = theApp.GetPresenter();
	presenter.SetDocumentModel(m_docModel);
	presenter.SetNativeDocument(this);

	presenter.ClearHistory();

	return TRUE;
}

BOOL CTestApplicationDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	const std::string path = CT2A(lpszPathName);
	if (!theApp.GetPresenter().Deserialize(path))
		return FALSE;

	SetModifiedFlag(FALSE);

	return TRUE;
}

BOOL CTestApplicationDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	const std::string path = CT2A(lpszPathName);
	if (!theApp.GetPresenter().Serialize(path))
		return FALSE;

	SetModifiedFlag(FALSE);

	return TRUE;
}

void CTestApplicationDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();
}

// *******************
//   INativeDocument
// *******************

void CTestApplicationDoc::SetModified(bool b)
{
	CDocument::SetModifiedFlag((BOOL)b);
}
