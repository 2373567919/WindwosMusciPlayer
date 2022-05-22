#include "fun.h"
//控件ID
UINT IDC_BUTTON_1 = 200;
UINT IDC_BUTTON_2 = 201;
UINT IDC_BUTTON_3 = 203;
UINT IDC_BUTTON_4 = 204;
UINT IDC_BUTTON_5 = 205;
UINT IDC_LIST_1 = 301;
UINT IDC_EDIT_1 = 401;
HWND hButtonwnd3 = NULL; //播放/暂停的控件
HWND hListCtrl = NULL;	 //列表控件的句柄
HWND hEditend1 = NULL;	 //编辑框控件的句柄
UINT p = 0;				 // p列表控件的行号
HSTREAM hMusicStream;	 //音乐播放句柄
std::vector<TCHAR *> MuiscPathList;
HWND h;

BOOL EumeFile(PTCHAR lpszPath)
{
	WIN32_FIND_DATA *FindData = new WIN32_FIND_DATA;
	PTCHAR path = new TCHAR[MAX_PATH];
	wsprintf(path, TEXT("%s\\*.*"), lpszPath);
	HANDLE hFind = FindFirstFile(path, FindData);
	delete[] path;
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	BOOL bRet = FALSE;
	do
	{
		bRet = FindNextFile(hFind, FindData);
		if (!bRet)
			break;
		PTCHAR ext = new TCHAR[10];
		PTCHAR filename = new TCHAR[MAX_PATH];
		_tsplitpath_s(FindData->cFileName, NULL, 0, NULL, 0, filename, MAX_PATH, ext, 10);

		if (!(_tcscmp(ext, TEXT(".mp3")) && _tcscmp(ext, TEXT(".wav")) && _tcscmp(ext, TEXT(".ogg")) && _tcscmp(ext, TEXT(".flac"))))
		{
			PTCHAR temp = new TCHAR[MAX_PATH];
			wsprintf(temp, TEXT("%s\\%s"), lpszPath, FindData->cFileName);

			MuiscPathList.push_back(temp);
			int iTotalRow = ListView_GetItemCount(hListCtrl);
			LV_ITEM row_item = {0};
			row_item.mask |= LVIF_TEXT;
			row_item.pszText = filename;
			row_item.iItem = iTotalRow;
			SendMessage(hListCtrl, LVM_INSERTITEM, iTotalRow, (LPARAM)&row_item);
		}
		delete[] filename;
		delete[] ext;
	} while (bRet);
	delete FindData;
	return TRUE;
}

void CALLBACK endCallback(HSYNC handle, DWORD channel, DWORD data, void *pTarget)
{
	p++;
	if (p == ListView_GetItemCount(hListCtrl)) //如果是最后一首,则下一曲是第一首
		p = 0;
	Open(MuiscPathList.at(p)); //打开音乐
	Play();					   //播放音乐
	PTCHAR temp = new TCHAR[MAX_PATH];
	ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
	SetWindowText(h, temp);
	delete[] temp;
}

void Open(PTCHAR szFileName)
{
	Stop();
	hMusicStream = BASS_StreamCreateFile(FALSE, szFileName, 0, 0, BASS_SAMPLE_FLOAT);
	BASS_ChannelSetSync(hMusicStream, BASS_SYNC_END, 0, endCallback, 0);
}

void Play()
{
	BASS_ChannelPlay(hMusicStream, FALSE);
	SetWindowText(hButtonwnd3, TEXT("暂停"));
}

void Pause()
{
	BASS_ChannelPause(hMusicStream);
	SetWindowText(hButtonwnd3, TEXT("继续"));
}

void Stop()
{
	BASS_ChannelStop(hMusicStream);
	BASS_ChannelFree(hMusicStream);
}
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		hFont = CreateFont(
			-15 /*高度*/, -7 /*宽度*/, 0, 0, 400 /*一般这个值设为400*/,
			FALSE /*不带斜体*/, FALSE /*不带下划线*/, FALSE /*不带删除线*/,
			DEFAULT_CHARSET,							 //使用默认字符集
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //这行参数不用管
			DEFAULT_QUALITY,							 //默认输出质量
			FF_DONTCARE,								 //不指定字体族*/
			TEXT("微软雅黑")							 //字体名
		);
		// HWND hButtonwnd1 = CreateWindow(WC_BUTTON /*窗口类*/, /*标题*/ TEXT("打开音乐目录"), /*控件样式*/ WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 2, 2, 102, 20, /*父窗口句柄*/ hWnd, /*控件ID*/ (HMENU)IDC_BUTTON_1, NULL, NULL);
		hButtonwnd3 = CreateWindow(WC_BUTTON /*窗口类*/, /*标题*/ TEXT("暂停"), /*控件样式*/ WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 10, 2, 90, 20, /*父窗口句柄*/ hWnd, /*控件ID*/ (HMENU)IDC_BUTTON_2, NULL, NULL);
		HWND hButtonwnd4 = CreateWindow(WC_BUTTON /*窗口类*/, /*标题*/ TEXT("停止"), /*控件样式*/ WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 110, 2, 90, 20, /*父窗口句柄*/ hWnd, /*控件ID*/ (HMENU)IDC_BUTTON_3, NULL, NULL);
		HWND hButtonwnd5 = CreateWindow(WC_BUTTON /*窗口类*/, /*标题*/ TEXT("上一曲"), /*控件样式*/ WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 210, 2, 90, 20, /*父窗口句柄*/ hWnd, /*控件ID*/ (HMENU)IDC_BUTTON_4, NULL, NULL);
		HWND hButtonwnd6 = CreateWindow(WC_BUTTON /*窗口类*/, /*标题*/ TEXT("下一曲"), /*控件样式*/ WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 310, 2, 90, 20, /*父窗口句柄*/ hWnd, /*控件ID*/ (HMENU)IDC_BUTTON_5, NULL, NULL);
		hEditend1 = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 24, 390, 48, hWnd, (HMENU)IDC_EDIT_1, NULL, NULL);
		hListCtrl = CreateWindow(WC_LISTVIEW /*窗口类*/, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS, 10, 74, 390, 700, hWnd, (HMENU)IDC_LIST_1, NULL, NULL);
		ListView_SetExtendedListViewStyle(hListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);
		LVCOLUMN colInfo = {0};
		//插入列
		colInfo.mask |= LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
		colInfo.cx = 390;
		colInfo.pszText = (LPTSTR)TEXT("音乐");
		colInfo.fmt = LVCFMT_RIGHT;
		ListView_InsertColumn(hListCtrl, 0, &colInfo);
		//向列表控件中插入默认F:\\音乐下的音乐路径
		// PTCHAR strFolder = TEXT("F:\\音乐");
		TCHAR strFolder[MAX_PATH];
    	GetPrivateProfileString(TEXT("url"),TEXT("musicUrl"),NULL,strFolder,MAX_PATH,TEXT("./a.ini"));
		EumeFile(strFolder);
		//设置按钮控件的字体
		// SendMessage(hButtonwnd1, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hButtonwnd3, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hButtonwnd4, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hButtonwnd5, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hButtonwnd6, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hListCtrl, WM_SETFONT, (WPARAM)hFont, NULL);
		SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
		//初始化音乐播放器
		BASS_Init(-1, 44100, BASS_DEVICE_CPSPEAKERS, hWnd, 0);
		h = hWnd;
	}
	break;
	case WM_NOTIFY:
	{
		NMHDR *pMhDr = (NMHDR *)lParam;
		if (pMhDr->idFrom == IDC_LIST_1) //列表控件ID
		{
			if (pMhDr->code == NM_CLICK) //列表别点击的消息
			{
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;

				p = lpnmitem->iItem;	   //获取列表被点击的行号
				Open(MuiscPathList.at(p)); //打开音乐路径的音乐文件
				Play();					   //播放音乐文件
				PTCHAR temp = new TCHAR[MAX_PATH];
				ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
				SetWindowText(hWnd, temp);
				delete[] temp;
			}
		}
	}
	break;
	case WM_COMMAND:
	{
		UINT nCtrlId = LOWORD(wParam); //控件ID
		UINT ncode = HIWORD(wParam);   //通知码BN_CLICKED
		if (ncode == BN_CLICKED)	   //按钮被点击的消息
		{
			if (nCtrlId == IDC_BUTTON_2)
			{
				TCHAR trip[MAX_PATH];
				GetWindowText(hButtonwnd3, trip, MAX_PATH);
				if (_tcscmp(trip, TEXT("暂停")) == 0)
				{
					Pause(); //暂停
				}
				else if (_tcscmp(trip, TEXT("继续")) == 0)
				{
					Play(); //播放
				}
			}
			else if (nCtrlId == IDC_BUTTON_3)
			{
				Stop();
			}
			else if (nCtrlId == IDC_BUTTON_4) //上一曲
			{
				p--;
				if (p == -1)
					p = ListView_GetItemCount(hListCtrl) - 1;
				Open(MuiscPathList.at(p)); //打开音乐
				Play();					   //播放音乐
				PTCHAR temp = new TCHAR[MAX_PATH];
				ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
				SetWindowText(hWnd, temp);
				delete[] temp;
			}
			else if (nCtrlId == IDC_BUTTON_5) //下一曲
			{
				p++;
				if (p == ListView_GetItemCount(hListCtrl)) //如果是最后一首,则下一曲是第一首
					p = 0;
				Open(MuiscPathList.at(p)); //打开音乐
				Play();					   //播放音乐
				PTCHAR temp = new TCHAR[MAX_PATH];
				ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
				SetWindowText(hWnd, temp);
				delete[] temp;
			}
		}
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_MEDIA_NEXT_TRACK:
		{

			p++;
			if (p == ListView_GetItemCount(hListCtrl)) //如果是最后一首,则下一曲是第一首
				p = 0;
			Open(MuiscPathList.at(p)); //打开音乐
			Play();					   //播放音乐
			PTCHAR temp = new TCHAR[MAX_PATH];
			ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
			SetWindowText(hWnd, temp);
			delete[] temp;
		}
		break;
		case VK_MEDIA_PREV_TRACK:
		{
			p--;
			if (p == -1)
				p = ListView_GetItemCount(hListCtrl) - 1;
			Open(MuiscPathList.at(p)); //打开音乐
			Play();					   //播放音乐
			PTCHAR temp = new TCHAR[MAX_PATH];
			ListView_GetItemText(hListCtrl, p, 0, temp, MAX_PATH);
			SetWindowText(hWnd, temp);
			delete[] temp;
		}
		break;
		case VK_MEDIA_PLAY_PAUSE:
		{
			TCHAR trip[MAX_PATH];
			GetWindowText(hButtonwnd3, trip, MAX_PATH);
			if (_tcscmp(trip, TEXT("暂停")) == 0)
			{
				Pause(); //暂停
			}
			else if (_tcscmp(trip, TEXT("继续")) == 0)
			{
				Play(); //播放
			}
		}
		break;
		case VK_MEDIA_STOP:
		{
			Stop();
		}
		break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		BASS_ChannelStop(hMusicStream);
		BASS_ChannelFree(hMusicStream);
		BASS_Stop();
		BASS_Free();
		MuiscPathList.clear();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
