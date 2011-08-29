//
//// CardGameDlg.h : ͷ�ļ�
////
//
//#pragma once
//
//#include "ClientSocketMgr.h"
//#include "path.h"
////#include <afxext.h>
//
//// CCardGameDlg �Ի���
//class CCardGameDlg : public CDialog
//{
//// ����
//public:
//	CCardGameDlg(CWnd* pParent = NULL);	// ��׼���캯��
//
//// �Ի�������
//	enum { IDD = IDD_CARDGAME_DIALOG };
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
//
//
//// ʵ��
//protected:
//	HICON m_hIcon;
//
//	// ���ɵ���Ϣӳ�亯��
//	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg HCURSOR OnQueryDragIcon();
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg void OnBnClickedButton1();
//private:
//	clinetnet::CClientSocketMgr m_CSM;
//	void _DisposeNetMsg();
//	void _ProcessMsg(MSG_BASE & rMsg);
//	void _DrawPath(int nPathID);
//	bool _AddPlayer(int nPlayerID);
//	bool _RemovePlayer(int nPlayerID);
//	void _ClearAll();
//	void _ResetGame();
//
//	int m_nPlayerID;
//	int m_nCardType;
//	int m_nCard1Type;
//	int m_nCard1Instruction;
//	int m_nCard2Type;
//	int m_nCard2Instruction;
//
//	int m_nPlayer1PlayerID;
//	int m_nPlayer2PlayerID;
//	int m_nPlayer3PlayerID;
//	int m_nPlayer4PlayerID;
//	
//	bool m_bPlayer1Turn;
//	bool m_bPlayer2Turn;
//	bool m_bPlayer3Turn;
//	bool m_bPlayer4Turn;
//
//	Path m_Path[10];
//public:
//	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButtonCard1();
//	afx_msg void OnBnClickedButtonCard2();
//	afx_msg void OnBnClickedButtonStartgame();
//
//	CBitmapButton m_LoginButton;
//};
