// Copyright (C) The Company

#pragma once

#include "History.h"

// MFC's document which wraps our IDocumentModel.
// Partly wizard-generated.
class CTestApplicationDoc : public CDocument, public pres_model::IHistoryListener
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CTestApplicationDoc)

// Overrides
public:
	virtual BOOL OnNewDocument()  override;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual void OnExecute(pres_model::ICommand* newCommand) override;
	virtual void OnUnexecute(pres_model::ICommand* newCommand) override;
};
