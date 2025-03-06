
// MFCDlg.h: 헤더 파일
//

#pragma once
#include <vector>
#include <random>

using namespace std;

// CMFCDlg 대화 상자
class CMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	vector<CPoint> m_clickPoints;
	int m_radius;
	int m_borderThickness;
	BOOL m_bDragging;
	int m_dragIndex;
	CPoint m_dragStartPoint;
	CPoint m_circleCenter;
	double m_circleRadius;

	CEdit m_editRadius;
	CEdit m_editBorder;
	CStatic m_staticPoint1;
	CStatic m_staticPoint2;
	CStatic m_staticPoint3;

	void DrawCircle(CPoint center, int radius);
	void DrawGarden();
	void Initialize();
	void RandomMove();
	void UpdateClickPointUI();
	void UpdateGardenPosition(CPoint point,int index);
	void ClearCanvas();
	void CalculateCircle();




public:
	afx_msg void OnEnChangeEditRadius();
	afx_msg void OnEnChangeEditBorder();
};
