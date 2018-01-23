// Copyright (C) The Company

#pragma once

#include "DocumentModel.h"
#include "NativeDocument.h"

// MFC's document which wraps our IDocumentModel.
// Partly wizard-generated.
class CTestApplicationDoc : public CDocument, public INativeDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CTestApplicationDoc)

// Overrides
public:
	virtual BOOL OnNewDocument()  override;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
	virtual void OnCloseDocument() override;

	IDocumentModel* GetDocumentModel() const
	{
		return m_docModel.get();
	}

// Implements INativeDocument
public:
	virtual void SetModified(bool b) override;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	std::shared_ptr<IDocumentModel> m_docModel;
};
