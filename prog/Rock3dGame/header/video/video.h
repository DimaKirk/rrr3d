
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

#include <windows.h>
#include <dshow.h>
#include <d3d9.h>
#include <Vmr9.h>
#include <Evr.h>

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

HRESULT RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer, BOOL *pbRemoved);
HRESULT AddFilterByCLSID(IGraphBuilder *pGraph, REFGUID clsid, IBaseFilter **ppF, LPCWSTR wszName);

// Abstract class to manage the video renderer filter.
// Specific implementations handle the VMR-7, VMR-9, or EVR filter.

class CVideoRenderer
{
public:
	virtual ~CVideoRenderer() {};
	virtual BOOL    HasVideo() const = 0;
	virtual HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd) = 0;
	virtual HRESULT FinalizeGraph(IGraphBuilder *pGraph) = 0;
	virtual HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc) = 0;
	virtual HRESULT Repaint(HWND hwnd, HDC hdc) = 0;
	virtual HRESULT DisplayModeChanged() = 0;

	virtual bool GetFullScreen() const = 0;
	virtual void SetFullScreen(bool value) = 0;
};

// Manages the VMR-7 video renderer filter.

class CVMR7 : public CVideoRenderer
{
	IVMRWindowlessControl   *m_pWindowless;

public:
	CVMR7();
	~CVMR7();
	BOOL    HasVideo() const;
	HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd);
	HRESULT FinalizeGraph(IGraphBuilder *pGraph);
	HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc);
	HRESULT Repaint(HWND hwnd, HDC hdc);
	HRESULT DisplayModeChanged();

	bool GetFullScreen() const;
	void SetFullScreen(bool value);
};


// Manages the VMR-9 video renderer filter.

class CVMR9 : public CVideoRenderer
{
	IVMRWindowlessControl9 *m_pWindowless;
	IVideoWindow* _videoWindow;
public:
	CVMR9();
	~CVMR9();
	BOOL    HasVideo() const;
	HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd);
	HRESULT FinalizeGraph(IGraphBuilder *pGraph);
	HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc);
	HRESULT Repaint(HWND hwnd, HDC hdc);
	HRESULT DisplayModeChanged();

	bool GetFullScreen() const;
	void SetFullScreen(bool value);
};


// Manages the EVR video renderer filter.

class CEVR : public CVideoRenderer
{
	IBaseFilter            *m_pEVR;
	IMFVideoDisplayControl *m_pVideoDisplay;

public:
	CEVR();
	~CEVR();
	BOOL    HasVideo() const;
	HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd);
	HRESULT FinalizeGraph(IGraphBuilder *pGraph);
	HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc);
	HRESULT Repaint(HWND hwnd, HDC hdc);
	HRESULT DisplayModeChanged();

	bool GetFullScreen() const;
	void SetFullScreen(bool value);
};