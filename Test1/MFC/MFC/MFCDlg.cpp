
// MFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC.h"
#include "MFCDlg.h"
#include <thread>
#include <chrono>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDlg 대화 상자



CMFCDlg::CMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_radius = 50;
	m_borderThickness = 5;
	m_bDragging = FALSE;
	m_dragIndex = -1;
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_RADIUS, m_editRadius);
	DDX_Control(pDX,IDC_EDIT_BORDER , m_editBorder );
	DDX_Control(pDX,IDC_STATIC_POINT_1 ,m_staticPoint1 );
	DDX_Control(pDX,IDC_STATIC_POINT_2 ,m_staticPoint2 );
	DDX_Control(pDX, IDC_STATIC_POINT_3,m_staticPoint3 );
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CMFCDlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_BORDER, &CMFCDlg::OnEnChangeEditBorder)
END_MESSAGE_MAP()


// CMFCDlg 메시지 처리기

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCDlg::OnPaint()
{
	CPaintDC dc(this);
	for (const auto& point : m_clickPoints)
	{
		DrawCircle(point, m_radius);
	}

	if (m_clickPoints.size() == 3)
	{
		DrawGarden();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Initialize();

}


void CMFCDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread t([this]() {
		for (int i = 0; i < 10; i++)
		{
			this->RandomMove();
			this->UpdateClickPointUI();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		});
	t.detach();
}

void CMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_clickPoints.size() < 3) 
	{
		CString strRadius, strBorder;
		m_editRadius.GetWindowText(strRadius);
		m_editRadius.GetWindowText(strBorder);

		int newRadius = _ttoi(strRadius);
		int newBorder = _ttoi(strBorder);

		if (newRadius > 0) m_radius = newRadius;
		if (newBorder > 0) m_borderThickness = newBorder;

		m_clickPoints.push_back(point);
		UpdateClickPointUI();
		Invalidate();
		if (m_clickPoints.size() == 3)
		{
			CalculateCircle();
		}

	}

	else 
	{
		for (int i = 0; i < 3; i++)
		{
			int dx = m_clickPoints[i].x - point.x;
			int dy = m_clickPoints[i].y - point.y;
			if (dx * dx + dy * dy <= m_radius * m_radius)
			{
				m_bDragging = TRUE;
				m_dragIndex = i;
				m_dragStartPoint = point;
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);

}

void CMFCDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bDragging && nFlags & MK_LBUTTON )
	{
		UpdateClickPointUI();
		UpdateGardenPosition(point,m_dragIndex);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragging = FALSE;
	m_dragIndex = -1;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCDlg::DrawCircle(CPoint center, int radius)
{
	CClientDC dc(this);
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	CBrush* pOldBrush = dc.SelectObject(&brush);

	dc.Ellipse(center.x - radius, center.y - radius, center.x + radius, center.y + radius);
	dc.SelectObject(pOldBrush);
}

void CMFCDlg::DrawGarden()
{
	if (m_clickPoints.size() < 3)
		return;

	CClientDC dc(this);
	CPen pen(PS_SOLID, m_borderThickness, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen);

	dc.Ellipse(m_circleCenter.x - m_circleRadius, m_circleCenter.y - m_circleRadius, m_circleCenter.x + m_circleRadius, m_circleCenter.y + m_circleRadius);

	dc.SelectObject(pOldPen);

}

void CMFCDlg::Initialize()
{
	m_clickPoints.clear();
	UpdateClickPointUI();
	Invalidate();
}

void CMFCDlg::RandomMove()
{
	if (m_clickPoints.size() < 3) return;

	srand((unsigned int)time(NULL));
	CRect rect;
	GetClientRect(&rect);

	for (int i = 0; i < 3; i++)
	{
		m_clickPoints[i].x = rand() % rect.Width();
		m_clickPoints[i].y = rand() % rect.Height();
	}
	CalculateCircle();
	Invalidate();
	DrawGarden();
}

void CMFCDlg::UpdateClickPointUI()
{
	CString str;
	for (int i = 0; i < m_clickPoints.size(); i++)
	{
		str.Format(_T("%d:(x:%d, y:%d)"), i + 1, m_clickPoints[i].x, m_clickPoints[i].y);
		if (i == 0) m_staticPoint1.SetWindowText(str);
		else if (i == 1) m_staticPoint2.SetWindowText(str);
		else if (i == 2) m_staticPoint3.SetWindowTextW(str);
	}
}

void CMFCDlg::UpdateGardenPosition(CPoint point, int index)
{
	if (index < 0 || index >= 3) return;

	CPoint offset = point - m_dragStartPoint;
	m_clickPoints[index] += offset;
	m_dragStartPoint = point;

	
	CalculateCircle();
	Invalidate();
}

void CMFCDlg::ClearCanvas()
{
	CPaintDC dc(this);
	dc.FillSolidRect(0, 0, 500, 500, RGB(255, 255, 255));
}

void CMFCDlg::CalculateCircle()
{
	CPoint p1 = m_clickPoints[0];
	CPoint p2 = m_clickPoints[1];
	CPoint p3 = m_clickPoints[2];

	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	double A = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);
	double B = (x1 * x1 + y1 * y1) * (y3 - y2) + (x2 * x2 + y2 * y2) * (y1 - y3) + (x3 * x3 + y3 * y3) * (y2 - y1);
	double C = (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * x2 + y2 * y2) * (x3 - x1) + (x3 * x3 + y3 * y3) * (x1 - x2);

	m_circleCenter.x = -B / (2 * A);
	m_circleCenter.y = -C / (2 * A);
	m_circleRadius = sqrt(pow(m_circleCenter.x - x1, 2) + pow(m_circleCenter.y - y1, 2));

}


void CMFCDlg::OnEnChangeEditRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCDlg::OnEnChangeEditBorder()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
