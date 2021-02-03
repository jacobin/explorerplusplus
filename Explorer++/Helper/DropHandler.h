// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "ReferenceCount.h"
#include <list>

/* TODO: Switch to IReferenceCount in the future.
IUnknown needed to support CShellBrowser. */
__interface IDropFilesCallback : public IUnknown
{
	void OnDropFile(const std::list<std::wstring> &PastedFileList, const POINT *ppt);
};

class DropHandler : public ReferenceCount
{
public:

	/* As this class is reference counted, the constructor
	and destructor are both private. Use this method to
	get a new instance of this class. */
	static DropHandler	*CreateNew();

	static HRESULT		GetDropFormats(std::list<FORMATETC> &ftcList);

	void	CopyClipboardData(IDataObject *pDataObject,HWND hwndDrop,const TCHAR *szDestDirectory,IDropFilesCallback *pDropFilesCallback);

private:

	DropHandler() = default;
	~DropHandler() = default;

	void	HandleLeftClickDrop(IDataObject *pDataObject,POINT *pt);

	BOOL	CheckDropFormatSupported(IDataObject *pDataObject,FORMATETC *pftc);

	HRESULT	CopyUnicodeTextData(IDataObject *pDataObject,std::list<std::wstring> &PastedFileList);
	HRESULT	CopyAnsiTextData(IDataObject *pDataObject,std::list<std::wstring> &PastedFileList);
	HRESULT	CopyDIBV5Data(IDataObject *pDataObject,std::list<std::wstring> &PastedFileList);

	HRESULT	CopyTextToFile(const TCHAR *pszDestDirectory, const WCHAR *pszText, TCHAR *pszFullFileNameOut, size_t outLen);

	/* Holds the drop formats supported. */
	static FORMATETC	m_ftcText;
	static FORMATETC	m_ftcUnicodeText;
	static FORMATETC	m_ftcDIBV5;

	IDataObject			*m_pDataObject;
	IDropFilesCallback	*m_pDropFilesCallback;
	POINT				m_pt;
	DWORD				m_dwEffect;
	HWND				m_hwndDrop;
	std::wstring		m_destDirectory;
};