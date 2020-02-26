
// qcView.cpp: CqcView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "qc.h"
#endif

#include "qcDoc.h"
#include "qcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CqcView

IMPLEMENT_DYNCREATE(CqcView, CView)

BEGIN_MESSAGE_MAP(CqcView, CView)
	ON_WM_STYLECHANGED()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CqcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CqcView 构造/析构

CqcView::CqcView() noexcept
{
	// TODO: 在此处添加构造代码

}

CqcView::~CqcView()
{
}

BOOL CqcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CqcView::OnDraw(CDC* pDC)
{
	CqcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_formula.IsEmpty())
	{
		return;
	}

	LOGFONT lf = { 0 };
	lf.lfHeight = -MulDiv(18, -pDC->GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfCharSet = GB2312_CHARSET;
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"微软雅黑");
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont* pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);

	CRect rc(0, 0, 35565, 35565);
	pDC->DrawText(pDoc->m_formula.GetAt(0), &rc, DT_SINGLELINE | DT_CALCRECT);
	int w = (int)(rc.Width() * 1.1f);
	int h = (int)(rc.Height() * 1.3f);

	CSize sz;
	if (pDC->IsPrinting())
	{
		sz.cx = pDC->GetDeviceCaps(HORZRES);
		sz.cy = pDC->GetDeviceCaps(VERTRES);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		sz.cx = rect.Width();
		sz.cy = rect.Height();
	}

	int left = (sz.cx - w * cols) / 2;
	int top = (sz.cy - h * rows) / 2;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			pDC->TextOut(left + j * w, top, pDoc->m_formula.GetAt(i * cols + j));
		}
		top += h;
	}

	pDC->SelectObject(pOldFont);
}


void CqcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
}


// CqcView 打印


void CqcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CqcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CqcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CqcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CqcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CqcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CqcView 诊断

#ifdef _DEBUG
void CqcView::AssertValid() const
{
	CView::AssertValid();
}

void CqcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CqcDoc* CqcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CqcDoc)));
	return (CqcDoc*)m_pDocument;
}
#endif //_DEBUG


// CqcView 消息处理程序
void CqcView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: 添加代码以响应用户对窗口视图样式的更改
	CView::OnStyleChanged(nStyleType,lpStyleStruct);
}
