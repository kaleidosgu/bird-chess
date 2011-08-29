//
//// CardGameDlg.cpp : 实现文件
////
//
//#include "stdafx.h"
//#include "CardGame.h"
//#include "CardGameDlg.h"
////#include "dialog.h"
//#include "cardgamemsg.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
//
//
//// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//
//class CAboutDlg : public CDialog
//{
//public:
//	CAboutDlg();
//
//// 对话框数据
//	enum { IDD = IDD_ABOUTBOX };
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//// 实现
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
//{
//}
//
//void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//END_MESSAGE_MAP()
//
//
//// CCardGameDlg 对话框
//
//
//
//
//CCardGameDlg::CCardGameDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CCardGameDlg::IDD, pParent)
//{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	m_CSM.Init();
//}
//
//void CCardGameDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CCardGameDlg, CDialog)
//	ON_WM_SYSCOMMAND()
//	ON_WM_PAINT()
//	ON_WM_TIMER()
//	ON_WM_CTLCOLOR()
//	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &CCardGameDlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &CCardGameDlg::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON_Card1, &CCardGameDlg::OnBnClickedButtonCard1)
//	ON_BN_CLICKED(IDC_BUTTON_Card2, &CCardGameDlg::OnBnClickedButtonCard2)
//	ON_BN_CLICKED(IDC_BUTTON_StartGame, &CCardGameDlg::OnBnClickedButtonStartgame)
//END_MESSAGE_MAP()
//
//
//// CCardGameDlg 消息处理程序
//
//BOOL CCardGameDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// 将“关于...”菜单项添加到系统菜单中。
//
//	// IDM_ABOUTBOX 必须在系统命令范围内。
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);
//
//	CMenu* pSysMenu = GetSystemMenu(FALSE);
//	if (pSysMenu != NULL)
//	{
//		BOOL bNameValid;
//		CString strAboutMenu;
//		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
//		ASSERT(bNameValid);
//		if (!strAboutMenu.IsEmpty())
//		{
//			pSysMenu->AppendMenu(MF_SEPARATOR);
//			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
//		}
//	}
//
//	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
//	//  执行此操作
//	SetIcon(m_hIcon, TRUE);			// 设置大图标
//	SetIcon(m_hIcon, FALSE);		// 设置小图标
//
//	// TODO: 在此添加额外的初始化代码
//
//	_ClearAll();
//	SetTimer(1, 25, NULL);
//	//((CBitmapButton *)GetDlgItem(IDC_BUTTON1))->AutoLoad(IDC_BUTTON1, this);
//	m_LoginButton.LoadBitmaps(connectD, connectF, connectU, connectX);
//	m_LoginButton.SubclassDlgItem(IDC_BUTTON1, this);
//	m_LoginButton.SizeToContent();
//	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
//}
//void CCardGameDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	if (nIDEvent == 1)
//	{
//		_DisposeNetMsg();
//	}
//}
//HBRUSH CCardGameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//	if (nCtlColor == CTLCOLOR_STATIC)
//	{
//		if (IDC_STATIC_StartFailed == pWnd->GetDlgCtrlID())
//		{
//			pDC->SetTextColor(RGB(255, 0, 0));
//		}
//		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_Player1 && m_bPlayer1Turn)
//		{
//			pDC->SetTextColor(RGB(255, 0, 0));
//		}
//		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_Player2 && m_bPlayer2Turn)
//		{
//			pDC->SetTextColor(RGB(255, 0, 0));
//		}
//		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_Player3 && m_bPlayer3Turn)
//		{
//			pDC->SetTextColor(RGB(255, 0, 0));
//		}
//		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_Player4 && m_bPlayer4Turn)
//		{
//			pDC->SetTextColor(RGB(255, 0, 0));
//		}
//	}
//
//	return hbr;
//}
//
//void CCardGameDlg::_DisposeNetMsg()
//{
//	while (true)
//	{
//		MSG_BASE * pMsg = m_CSM.GetMsg();
//		if (pMsg)
//		{
//			_ProcessMsg(*pMsg);
//			delete pMsg;
//			pMsg = NULL;
//		}
//		else
//		{
//			break;
//		}
//	}
//}
//void CCardGameDlg::_ProcessMsg(MSG_BASE & rMsg)
//{
//	if (rMsg.nMsg == MSGID_SYSTEM_Disconnect)
//	{
//		_ClearAll();
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_PlayerInfo)
//	{
//		MSG_CARDGAME_S2C_PlayerInfo & rPlayerInfoMsg = (MSG_CARDGAME_S2C_PlayerInfo &)rMsg;
//		m_nPlayerID = rPlayerInfoMsg.nPlayerID;
//		CString str;
//		str.Format("PlayerID: %d", rPlayerInfoMsg.nPlayerID);
//		SetDlgItemText(IDC_STATICMainPlayer, str);
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_StartGameFailed)
//	{
//		GetDlgItem(IDC_STATIC_StartFailed)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BUTTON_StartGame)->EnableWindow(true);
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_UseCardFailed)
//	{
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_PlayerEnter)
//	{
//		MSG_CARDGAME_S2C_PlayerEnter & rPlayerEnterMsg = (MSG_CARDGAME_S2C_PlayerEnter &)rMsg;
//		if (rPlayerEnterMsg.nPlayerID != m_nPlayerID)
//		{
//			_AddPlayer(rPlayerEnterMsg.nPlayerID);
//		}
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_MainPlayer)
//	{
//		MSG_CARDGAME_S2C_MainPlayer & rMainPlayerMsg = (MSG_CARDGAME_S2C_MainPlayer &)rMsg;
//		if (rMainPlayerMsg.nPlayerID == m_nPlayerID)
//		{
//			GetDlgItem(IDC_BUTTON_StartGame)->ShowWindow(SW_SHOW);
//			GetDlgItem(IDC_BUTTON_StartGame)->EnableWindow(true);
//		}
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_PlayerLeave)
//	{
//		MSG_CARDGAME_S2C_PlayerLeave & rPlayerLeaveMsg = (MSG_CARDGAME_S2C_PlayerLeave &)rMsg;
//		_RemovePlayer(rPlayerLeaveMsg.nPlayerID);
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_StartGame)
//	{
//		_ResetGame();
//		GetDlgItem(IDC_STATIC_StartFailed)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_StartGame)->EnableWindow(false);
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_SetCardType)
//	{
//		MSG_CARDGAME_S2C_SetCardType & rSetCardTypeMsg = (MSG_CARDGAME_S2C_SetCardType &)rMsg;
//		m_nCardType = rSetCardTypeMsg.nCardType;
//		CString str;
//		str.Format("CardType: %d", m_nCardType);
//		SetDlgItemText(IDC_STATIC_CardType, str);
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_AddNewCard)
//	{
//		MSG_CARDGAME_S2C_AddNewCard & rAddNewCardMsg = (MSG_CARDGAME_S2C_AddNewCard &)rMsg;
//		if (rAddNewCardMsg.nCardID == 1)
//		{
//			m_nCard1Type = rAddNewCardMsg.nCardType;
//			m_nCard1Instruction = rAddNewCardMsg.nCardInstruction;
//
//			CString str;
//			str.Format("Card: Type(%d) - %d", m_nCard1Type, m_nCard1Instruction);
//			SetDlgItemText(IDC_BUTTON_Card1, str);
//		}
//		else if (rAddNewCardMsg.nCardID == 2)
//		{
//			m_nCard2Type = rAddNewCardMsg.nCardType;
//			m_nCard2Instruction = rAddNewCardMsg.nCardInstruction;
//
//			CString str;
//			str.Format("Card: Type(%d) - %d", m_nCard2Type, m_nCard2Instruction);
//			SetDlgItemText(IDC_BUTTON_Card2, str);
//		}
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_CurrentTurnPlayer)
//	{
//		m_bPlayer1Turn = false;
//		m_bPlayer2Turn = false;
//		m_bPlayer3Turn = false;
//		m_bPlayer4Turn = false;
//		MSG_CARDGAME_S2C_CurrentTurnPlayer & rCurrentTurnPlayerMsg = (MSG_CARDGAME_S2C_CurrentTurnPlayer &)rMsg;
//		if (rCurrentTurnPlayerMsg.nPlayerID == m_nPlayerID)
//		{
//			GetDlgItem(IDC_BUTTON_Card1)->EnableWindow(true);
//			GetDlgItem(IDC_BUTTON_Card2)->EnableWindow(true);
//		}
//		else
//		{
//			if (rCurrentTurnPlayerMsg.nPlayerID == m_nPlayer1PlayerID)
//			{
//				m_bPlayer1Turn = true;
//			}
//			if (rCurrentTurnPlayerMsg.nPlayerID == m_nPlayer2PlayerID)
//			{
//				m_bPlayer2Turn = true;
//			}
//			if (rCurrentTurnPlayerMsg.nPlayerID == m_nPlayer3PlayerID)
//			{
//				m_bPlayer3Turn = true;
//			}
//			if (rCurrentTurnPlayerMsg.nPlayerID == m_nPlayer4PlayerID)
//			{
//				m_bPlayer4Turn = true;
//			}
//		}
//		GetDlgItem(IDC_STATIC_Player1)->Invalidate();
//		GetDlgItem(IDC_STATIC_Player2)->Invalidate();
//		GetDlgItem(IDC_STATIC_Player3)->Invalidate();
//		GetDlgItem(IDC_STATIC_Player4)->Invalidate();
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_UseCard)
//	{
//		MSG_CARDGAME_S2C_UseCard & rUseCard = (MSG_CARDGAME_S2C_UseCard &)rMsg;
//		CString str1;
//		str1.Format("Card: Type(%d) - %d", rUseCard.nCardType, rUseCard.nCardInstruction);
//		SetDlgItemText(IDC_STATIC_UsedCard, str1);
//		CString str2;
//		str2.Format("CardType(%d)", rUseCard.nTargetCardType);
//		SetDlgItemText(IDC_STATIC_UsedCardType, str2);
//		
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_MoveCard)
//	{
//		MSG_CARDGAME_S2C_MoveCard & rMoveCardMsg = (MSG_CARDGAME_S2C_MoveCard &)rMsg;
//		if (rMoveCardMsg.nSrcPathID >= 0 && rMoveCardMsg.nSrcPathID < 10 && rMoveCardMsg.nDesPathID >=0 && rMoveCardMsg.nDesPathID < 10)
//		{
//			m_Path[rMoveCardMsg.nSrcPathID].RemoveCardType((CardType)rMoveCardMsg.nCardType);
//			m_Path[rMoveCardMsg.nDesPathID].AddCardType((CardType)rMoveCardMsg.nCardType);
//			_DrawPath(rMoveCardMsg.nSrcPathID);
//			_DrawPath(rMoveCardMsg.nDesPathID);
//		}
//	}
//	else if (rMsg.nMsg == MSGID_CARDGAME_S2C_Win)
//	{
//		MSG_CARDGAME_S2C_Win & rWin = (MSG_CARDGAME_S2C_Win &)rMsg;
//		if (rWin.nPlayerID == -1)
//		{
//			MessageBox("Nobody win, but game over.");
//		}
//		else if (rWin.nPlayerID == m_nPlayerID)
//		{
//			MessageBox("You Win!");
//		}
//		else if (rWin.nPlayerID == m_nPlayer1PlayerID)
//		{
//			CString str;
//			str.Format("Player(%d) Win!", m_nPlayer1PlayerID);
//			MessageBox(str);
//		}
//		else if (rWin.nPlayerID == m_nPlayer2PlayerID)
//		{
//			CString str;
//			str.Format("Player(%d) Win!", m_nPlayer2PlayerID);
//			MessageBox(str);
//		}
//		else if (rWin.nPlayerID == m_nPlayer3PlayerID)
//		{
//			CString str;
//			str.Format("Player(%d) Win!", m_nPlayer3PlayerID);
//			MessageBox(str);
//		}
//		else if (rWin.nPlayerID == m_nPlayer4PlayerID)
//		{
//			CString str;
//			str.Format("Player(%d) Win!", m_nPlayer4PlayerID);
//			MessageBox(str);
//		}
//		else
//		{
//			CString str;
//			str.Format("Where is the Winner?", m_nPlayer4PlayerID);
//			MessageBox(str);
//		}
//		GetDlgItem(IDC_BUTTON_StartGame)->EnableWindow(true);
//	}
//}
//void CCardGameDlg::_ClearAll()
//{
//	m_nPlayerID = -1;
//	SetDlgItemText(IDC_STATICMainPlayer, "");
//	
//	GetDlgItem(IDC_BUTTON_StartGame)->ShowWindow(SW_HIDE);
//
//	m_nPlayer1PlayerID = -1;
//	m_nPlayer2PlayerID = -1;
//	m_nPlayer3PlayerID = -1;
//	m_nPlayer4PlayerID = -1;
//	SetDlgItemText(IDC_STATIC_Player1, "");
//	SetDlgItemText(IDC_STATIC_Player2, "");
//	SetDlgItemText(IDC_STATIC_Player3, "");
//	SetDlgItemText(IDC_STATIC_Player4, "");
//
//	GetDlgItem(IDC_STATIC_StartFailed)->ShowWindow(SW_HIDE);
//	
//	_ResetGame();
//}
//
//void CCardGameDlg::_ResetGame()
//{
//	m_nCardType = -1;
//	SetDlgItemText(IDC_STATIC_CardType, "");
//	
//	m_nCard1Type = -1;
//	SetDlgItemText(IDC_BUTTON_Card1, "");
//	m_nCard1Instruction = 0;
//	m_nCard2Type = -1;
//	m_nCard2Instruction = 0;
//	SetDlgItemText(IDC_BUTTON_Card2, "");
//	
//	GetDlgItem(IDC_BUTTON_Card1)->EnableWindow(false);
//	GetDlgItem(IDC_BUTTON_Card2)->EnableWindow(false);
//	
//	m_bPlayer1Turn = false;
//	m_bPlayer2Turn = false;
//	m_bPlayer3Turn = false;
//	m_bPlayer4Turn = false;
//
//	for (int i=0; i<10; i++)
//	{
//		m_Path[i].SetPathID(i);
//		m_Path[i].Reset();
//		_DrawPath(i);
//	}
//	m_Path[0].AddCardType(CardType_Red);
//	m_Path[0].AddCardType(CardType_Blue);
//	m_Path[0].AddCardType(CardType_Green);
//	m_Path[0].AddCardType(CardType_Purple);
//	m_Path[0].AddCardType(CardType_Yellow);
//	_DrawPath(0);
//}
//
//void CCardGameDlg::_DrawPath(int nPathID)
//{
//	if (nPathID >=0 && nPathID < 10)
//	{
//		stack< CardType > cardTypeStack = m_Path[nPathID].GetCardCollection().GetCardTypeStack();
//		CString str;
//		while (!cardTypeStack.empty())
//		{
//			CString strT;
//			strT.Format("%d", cardTypeStack.top());
//			str += strT;
//			cardTypeStack.pop();
//		}
//		SetDlgItemText(IDC_EDIT1 + nPathID, str);
//	}
//}
//
//bool CCardGameDlg::_AddPlayer(int nPlayerID)
//{
//	if (m_nPlayer1PlayerID == -1)
//	{
//		m_nPlayer1PlayerID = nPlayerID;
//		CString str;
//		str.Format("PlayerID: %d", nPlayerID);
//		SetDlgItemText(IDC_STATIC_Player1, str);
//		return true;
//	}
//	if (m_nPlayer2PlayerID == -1)
//	{
//		m_nPlayer2PlayerID = nPlayerID;
//		CString str;
//		str.Format("PlayerID: %d", nPlayerID);
//		SetDlgItemText(IDC_STATIC_Player2, str);
//		return true;
//	}
//	if (m_nPlayer3PlayerID == -1)
//	{
//		m_nPlayer3PlayerID = nPlayerID;
//		CString str;
//		str.Format("PlayerID: %d", nPlayerID);
//		SetDlgItemText(IDC_STATIC_Player3, str);
//		return true;
//	}
//	if (m_nPlayer4PlayerID == -1)
//	{
//		m_nPlayer4PlayerID = nPlayerID;
//		CString str;
//		str.Format("PlayerID: %d", nPlayerID);
//		SetDlgItemText(IDC_STATIC_Player4, str);
//		return true;
//	}
//	return false;
//}
//
//bool CCardGameDlg::_RemovePlayer(int nPlayerID)
//{
//	if (m_nPlayer1PlayerID == nPlayerID)
//	{
//		m_nPlayer1PlayerID = -1;
//		SetDlgItemText(IDC_STATIC_Player1, "");
//		return true;
//	}
//	if (m_nPlayer2PlayerID == nPlayerID)
//	{
//		m_nPlayer2PlayerID = -1;
//		SetDlgItemText(IDC_STATIC_Player2, "");
//		return true;
//	}
//	if (m_nPlayer3PlayerID == nPlayerID)
//	{
//		m_nPlayer3PlayerID = -1;
//		SetDlgItemText(IDC_STATIC_Player3, "");
//		return true;
//	}
//	if (m_nPlayer4PlayerID == nPlayerID)
//	{
//		m_nPlayer4PlayerID = -1;
//		SetDlgItemText(IDC_STATIC_Player4, "");
//		return true;
//	}
//	return false;
//}
//
//void CCardGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialog::OnSysCommand(nID, lParam);
//	}
//}
//
//// 如果向对话框添加最小化按钮，则需要下面的代码
////  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
////  这将由框架自动完成。
//
//void CCardGameDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 用于绘制的设备上下文
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 使图标在工作区矩形中居中
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 绘制图标
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialog::OnPaint();
//	}
//}
//
////当用户拖动最小化窗口时系统调用此函数取得光标
////显示。
//HCURSOR CCardGameDlg::OnQueryDragIcon()
//{
//	return static_cast<HCURSOR>(m_hIcon);
//}
//
//
//
//void CCardGameDlg::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_CSM.Connect("172.18.39.116", 8888);
//}
//
//
//void CCardGameDlg::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_CSM.Disconnect();
//	_ClearAll();
//}
//
//void CCardGameDlg::OnBnClickedButtonCard1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	GetDlgItem(IDC_BUTTON_Card1)->EnableWindow(false);
//	GetDlgItem(IDC_BUTTON_Card2)->EnableWindow(false);
//	
//	MSG_CARDGAME_C2S_UseCard msg;
//	msg.nCardID = 1;
//	// TODO
//	if (m_nCard1Type == 5)
//	{
//		msg.nCardType = 0;
//	}
//	else
//	{
//		msg.nCardType = m_nCard1Type;
//	}
//	m_CSM.SendMsg(msg);
//}
//
//
//void CCardGameDlg::OnBnClickedButtonCard2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	GetDlgItem(IDC_BUTTON_Card1)->EnableWindow(false);
//	GetDlgItem(IDC_BUTTON_Card2)->EnableWindow(false);
//	MSG_CARDGAME_C2S_UseCard msg;
//	msg.nCardID = 2;
//	// TODO
//	if (m_nCard2Type == 5)
//	{
//		msg.nCardType = 0;
//	}
//	else
//	{
//		msg.nCardType = m_nCard2Type;
//	}
//	m_CSM.SendMsg(msg);
//}
//
//
//void CCardGameDlg::OnBnClickedButtonStartgame()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	MSG_CARDGAME_C2S_StartGame msg;
//	m_CSM.SendMsg(msg);
//	GetDlgItem(IDC_BUTTON_StartGame)->EnableWindow(false);
//}
