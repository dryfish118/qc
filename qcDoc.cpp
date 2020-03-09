
// qcDoc.cpp: CqcDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "qc.h"
#endif

#include "qcDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CqcDoc

IMPLEMENT_DYNCREATE(CqcDoc, CDocument)

BEGIN_MESSAGE_MAP(CqcDoc, CDocument)
END_MESSAGE_MAP()


// CqcDoc 构造/析构

CqcDoc::CqcDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
}

CqcDoc::~CqcDoc()
{
}

static bool getSinglePlusSingle(CStringArray& formulas)
{
	const CString formula_single_plus_single[] =
	{
		L"1 x 1 =       ",	L"1 x 2 =       ",	L"2 x 1 =       ",	L"1 x 3 =       ",
		L"3 x 1 =       ",	L"1 x 4 =       ",	L"4 x 1 =       ",	L"1 x 5 =       ",
		L"5 x 1 =       ",	L"1 x 6 =       ",	L"6 x 1 =       ",	L"1 x 7 =       ",
		L"7 x 1 =       ",	L"1 x 8 =       ",	L"8 x 1 =       ",	L"1 x 9 =       ",
		L"9 x 1 =       ",	L"2 x 2 =       ",	L"2 x 3 =       ",	L"3 x 2 =       ",
		L"2 x 4 =       ",	L"4 x 2 =       ",	L"2 x 5 =       ",	L"5 x 2 =       ",
		L"2 x 6 =       ",	L"6 x 2 =       ",	L"2 x 7 =       ",	L"7 x 2 =       ",
		L"2 x 8 =       ",	L"8 x 2 =       ",	L"2 x 9 =       ",	L"9 x 2 =       ",
		L"3 x 3 =       ",	L"3 x 4 =       ",	L"4 x 3 =       ",	L"3 x 5 =       ",
		L"5 x 3 =       ",	L"3 x 6 =       ",	L"6 x 3 =       ",	L"3 x 7 =       ",
		L"7 x 3 =       ",	L"3 x 8 =       ",	L"8 x 3 =       ",	L"3 x 9 =       ",
		L"9 x 3 =       ",	L"4 x 4 =       ",	L"4 x 5 =       ",	L"5 x 4 =       ",
		L"4 x 6 =       ",	L"6 x 4 =       ",	L"4 x 7 =       ",	L"7 x 4 =       ",
		L"4 x 8 =       ",	L"8 x 4 =       ",	L"4 x 9 =       ",	L"9 x 4 =       ",
		L"5 x 5 =       ",	L"5 x 6 =       ",	L"6 x 5 =       ",	L"5 x 7 =       ",
		L"7 x 5 =       ",	L"5 x 8 =       ",	L"8 x 5 =       ",	L"5 x 9 =       ",
		L"9 x 5 =       ",	L"6 x 6 =       ",	L"6 x 7 =       ",	L"7 x 6 =       ",
		L"6 x 8 =       ",	L"8 x 6 =       ",	L"6 x 9 =       ",	L"9 x 6 =       ",
		L"7 x 7 =       ",	L"7 x 8 =       ",	L"8 x 7 =       ",	L"7 x 9 =       ",
		L"9 x 7 =       ",	L"8 x 8 =       ",	L"8 x 9 =       ",	L"9 x 8 =       ",
		L"9 x 9 =       "
	};

	for (int i = 0; i < cols * rows; i++)
	{
		formulas.Add(formula_single_plus_single[rand() % 81]);
	}

	return true;
}

static bool getMultiPlusSingle(CStringArray& formulas, bool bigFirst)
{
	std::set<int> f;
	f.insert(0);
	for (int i = 0; i < cols * rows; i++)
	{
		int x = 0, y = 0, c = 0;
		while (f.find(c) != f.end())
		{
			x = (rand() % 90) + 10;
			y = (rand() % 8) + 2;
			c = (x << 4) | y;
		}
		f.insert(c);

		CString formula;
		if (bigFirst)
		{
			formula.Format(L"%dx%d=          ", x, y);
		}
		else
		{
			formula.Format(L"%dx%d=          ", y, x);
		}
		formulas.Add(formula);
	}

	return true;
}

static bool getMultiDevideSingle(CStringArray& formulas)
{
	std::set<int> f;
	f.insert(0);
	for (int i = 0; i < cols * rows; i++)
	{
		int x = 0, y = 0, c = 0;
		while (f.find(c) != f.end())
		{
			x = (rand() % 90) + 10;
			y = (rand() % 8) + 2;
			c = (x << 4) | y;
		}
		f.insert(c);

		x *= y;
		CString formula;
		if (x >= 100)
		{
			formula.Format(L"%d÷%d=       ", x, y);
		}
		else
		{
			formula.Format(L"  %d÷%d=       ", x, y);
		}
		formulas.Add(formula);
	}

	return true;
}

BOOL CqcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_formula.RemoveAll();

	FormulaType ft = (FormulaType)((CWinAppEx*)AfxGetApp())->GetInt(L"Type");
	switch (ft)
	{
	case eSinglePlusSingle:
		getSinglePlusSingle(m_formula);
		break;
	case eMultiPlusSingle:
		getMultiPlusSingle(m_formula, true);
		break;
	case eSinglePlusMulti:
		getMultiPlusSingle(m_formula, false);
		break;
	case eMultiDivideSingle:
		getMultiDevideSingle(m_formula);
		break;
	default:
		break;
	}

	return TRUE;
}




// CqcDoc 序列化

void CqcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CqcDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CqcDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CqcDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CqcDoc 诊断

#ifdef _DEBUG
void CqcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CqcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CqcDoc 命令
