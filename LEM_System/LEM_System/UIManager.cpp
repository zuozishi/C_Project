#include "stdafx.h"
#include "windows.h"
#include "Commctrl.h"

#include "resource.h"
#include "UIManager.h"
#include "DevicesModel.h"
#include "Converter.h"

//临时静态变量
static Device *lookdev = NULL;
static Device *editdev = NULL;
static DeviceList *alldevlist = NULL;

void ShowMainWindow(DeviceList * list)
{
	alldevlist = list;
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainWindowProc, NULL);
}

void ShowAddDeviceWindow(DeviceList *list, HWND *window=NULL)
{
	alldevlist = list;
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_ADD),*window, AddDeviceProc, NULL);
}

void ShowLookDeviceWindow(DeviceList *list,Device * dev, HWND *window = NULL)
{
	alldevlist = list;
	lookdev = dev;
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_LOOK), *window, LookDeviceProc, NULL);
}

void ShowFindDeviceWindow(DeviceList * list, HWND *window = NULL)
{
	alldevlist = list;
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_FIND), *window, FindDeviceProc, NULL);
}

void ShowSetTypeWindow(HWND * window)
{
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_TYPE), *window, SetTypeProc, NULL);
}

void ShowAboutWindow(HWND *window = NULL)
{
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), *window, AboutWindowProc, NULL);
}

void LoadListView(HWND *window,int listview,int type,char *sname)
{
	if (window != NULL)
	{
		HWND mainlist = GetDlgItem(*window, listview);
		LVCOLUMN colums[7];
		LPWSTR titles[7] = { L"ID",L"名称",L"种类",L"价格",L"购入时间",L"是否报废",L"报废时间" };
		int sizes[7] = { 50,80,100,100,100,80,100 };
		ListView_DeleteAllItems(mainlist);
		SendMessage(*window, LVM_SETVIEW, LV_VIEW_LIST, NULL);
		DWORD dwExStyle =
			LVS_EX_FULLROWSELECT | //设置可选整行
			LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
			LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
			LVS_EDITLABELS;
		ListView_SetExtendedListViewStyle(mainlist, dwExStyle);
		int columlength=SendMessage(mainlist, LVM_GETCOLUMNWIDTH, 0, NULL);
		if (columlength == 0)
		{
			for (int i = 0; i < 7; i++)
			{
				colums[i].mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
				colums[i].pszText = titles[i];
				colums[i].cx = sizes[i];
				colums[i].iSubItem = i;
				ListView_InsertColumn(mainlist, i, &colums[i]);
			}
		}
		int devnum = List_Count(alldevlist);
		LVITEMW *item = new LVITEMW();
		item->mask = LVIF_TEXT;
		int size = 0;
		for (int i = 0; i < devnum; i++)
		{
			Device* dev = List_Get(alldevlist, i)->dev;
			if (type != -1&& type != dev->type)
			{
				continue;
			}
			if (sname != NULL && strcmp(sname,dev->name)!=0)
			{
				continue;
			}
			TCHAR id[50], name[50], type[50], price[50], addtime[50], *isdel, deltime[50];
			NumToTCHAR(dev->id, id);
			CharToTchar(dev->name, name);
			CharToTchar(dev->type_name, type);
			NumToTCHAR(dev->price, price);
			SystemTimeToTchar(&dev->addtime, addtime);
			SystemTimeToTchar(&dev->deltime, deltime);
			if (dev->isdel) {
				isdel = L"是";
			}
			else {
				isdel = L"否";
			}
			item->iItem = size;
			item->iSubItem = 0;
			item->pszText = (LPWSTR)id;
			ListView_InsertItem(mainlist, item);
			item->iSubItem = 1;
			item->pszText = (LPWSTR)name;
			ListView_SetItem(mainlist, item);
			item->iSubItem = 2;
			item->pszText = (LPWSTR)type;
			ListView_SetItem(mainlist, item);
			item->iSubItem = 3;
			item->pszText = (LPWSTR)price;
			ListView_SetItem(mainlist, item);
			item->iSubItem = 4;
			item->pszText = (LPWSTR)addtime;
			ListView_SetItem(mainlist, item);
			item->iSubItem = 5;
			item->pszText = (LPWSTR)isdel;
			ListView_SetItem(mainlist, item);
			if (dev->isdel) {
				item->iSubItem = 6;
				item->pszText = (LPWSTR)deltime;
				ListView_SetItem(mainlist, item);
			}
			size++;
		}
		if ((type != -1 || sname != NULL) && size == 0)
		{
			MessageBox(*window, L"没有搜索结果!", L"提示", MB_OK);
		}
	}
}

BOOL CALLBACK MainWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		HWND mainlist = GetDlgItem(hDlg, IDC_LIST_MAIN);
		LoadListView(&hDlg, IDC_LIST_MAIN,-1,NULL);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_MENU_EXIT)
		{
			exit(0);
		}
		else if (LOWORD(wParam) == ID_MENU_ABOUT)
		{
			ShowAboutWindow(&hDlg);
		}
		else if (LOWORD(wParam) == ID_MENU_ADD)
		{
			ShowAddDeviceWindow(alldevlist,&hDlg);
			HWND mainlist = GetDlgItem(hDlg, IDC_LIST_MAIN);
			LoadListView(&hDlg, IDC_LIST_MAIN, -1, NULL);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			exit(0);
		}
		else if (LOWORD(wParam) == ID_MENU_LOADFILE)
		{
			OPENFILENAME ofn = { 0 };
			TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
			ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
			ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
			ofn.lpstrFilter = TEXT("设备数据文件\0*.dat;所有文件\0*.*\0\0");//设置过滤
			ofn.nFilterIndex = 1;//过滤器索引
			ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
			ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
			ofn.lpstrInitialDir = NULL;//初始目录为默认
			ofn.lpstrTitle = TEXT("请选择设备数据文件(.dat)");//使用系统默认标题留空即可
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;//文件、目录必须存在
			if (GetOpenFileName(&ofn))
			{
				char path[500];
				TcharToChar(strFilename, path);
				LoadFromFile(alldevlist, path);
				HWND mainlist = GetDlgItem(hDlg, IDC_LIST_MAIN);
				LoadListView(&hDlg, IDC_LIST_MAIN, -1, NULL);
			}
		}
		else if (LOWORD(wParam) == ID_MENU_EXPORTFILE)
		{
			OPENFILENAME ofn = { 0 };
			TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
			ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
			ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
			ofn.lpstrFilter = TEXT("设备数据文件(.dat)\0*.dat\0CSV文件(.csv)\0*.csv\0\0");//设置过滤
			ofn.nFilterIndex = 1;//过滤器索引
			ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
			ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
			ofn.lpstrInitialDir = NULL;//初始目录为默认
			ofn.lpstrTitle = TEXT("另存为");//使用系统默认标题留空即可
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;//文件、目录必须存在
			if (GetSaveFileName(&ofn))
			{
				char path[500];
				TcharToChar(strFilename, path);
				if (ofn.nFilterIndex == 1) {
					strcat(path, ".dat");
					ExportToFile(alldevlist, path);
				}
				else {
					strcat(path, ".csv");
					ExportToFileCSV(alldevlist, path);
				}
			}
		}
		else if (LOWORD(wParam) == ID_MENU_LOOK)
		{
			HWND mainlist = GetDlgItem(hDlg, IDC_LIST_MAIN);
			int index = SendMessage(mainlist, LVM_GETSELECTIONMARK, 0, 0);
			if (index >= 0)
			{
				Device *dev = List_Get(alldevlist, index)->dev;
				ShowLookDeviceWindow(alldevlist, dev, &hDlg);
				LoadListView(&hDlg, IDC_LIST_MAIN, -1, NULL);
			}
			else {
				MessageBox(NULL, L"请选择需要查看的设备!", L"提示", MB_OK);
			}
		}
		else if (LOWORD(wParam) == ID_MENU_FIND)
		{
			ShowFindDeviceWindow(alldevlist, &hDlg);
		}
		else if (LOWORD(wParam) == ID_MENU_TYPE)
		{
			ShowSetTypeWindow(&hDlg);
		}
		else if (LOWORD(wParam) == ID_MEUN_EXCEL)
		{
			ExportToFileCSV(alldevlist, "temp.csv");
			system("temp.csv");
		}
		break;
	case 123:
		HWND mainlist = GetDlgItem(hDlg, IDC_LIST_MAIN);
		int index=SendMessage(mainlist, LVM_GETSELECTIONMARK, 0, 0);
		if (index >= 0)
		{
			Device *dev = List_Get(alldevlist,index)->dev;
			ShowLookDeviceWindow(alldevlist, dev, &hDlg);
			LoadListView(&hDlg, IDC_LIST_MAIN, -1, NULL);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK AddDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		HWND combo_type = GetDlgItem(hDlg, IDC_COMBO_ADD_CATE);
		TCHAR* typelist[50];
		for (int i = 0; i < 50; i++)
		{
			typelist[i] = NULL;
		}
		GetDeviceType(typelist);
		for (int i = 0; i <50; i++)
		{
			if (typelist[i] != NULL)
			{
				SendMessage(combo_type, CB_ADDSTRING, NULL, (LPARAM)typelist[i]);
			}
			else {
				break;
			}
		}
		if (editdev != NULL)
		{
			HWND text_id = GetDlgItem(hDlg, IDC_EDIT_ADD_ID);
			HWND text_name = GetDlgItem(hDlg, IDC_EDIT_ADD_NAME);
			HWND text_price = GetDlgItem(hDlg, IDC_EDIT_ADD_PRICE);
			HWND date_add = GetDlgItem(hDlg, IDC_DATETIMEPICKER_ADD_ADDDATE);
			HWND check_isdel = GetDlgItem(hDlg, IDC_CHECK_ADD_ISDEL);
			HWND date_del = GetDlgItem(hDlg, IDC_DATETIMEPICKER_ADD_DELDATE);
			TCHAR id[50], name[50], price[50];
			NumToTCHAR(editdev->id, id);
			CharToTchar(editdev->name, name);
			NumToTCHAR(editdev->price, price);
			SendMessage(text_id, WM_SETTEXT, NULL, (LPARAM)id);
			SendMessage(combo_type, CB_SETCURSEL, editdev->type, NULL);
			SendMessage(text_name, WM_SETTEXT, NULL, (LPARAM)name);
			SendMessage(text_price, WM_SETTEXT, NULL, (LPARAM)price);
			SendMessage(date_add, DTM_SETSYSTEMTIME, NULL, (LPARAM)&editdev->addtime);
			SendMessage(check_isdel, BM_SETCHECK, editdev->isdel, NULL);
			SendMessage(date_del, DTM_SETSYSTEMTIME, NULL, (LPARAM)&editdev->deltime);
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EDITBALLOONTIP tip;
			tip.pszText = L"提示";
			tip.ttiIcon = TTI_ERROR_LARGE;
			HWND text_id = GetDlgItem(hDlg, IDC_EDIT_ADD_ID);
			HWND combo_type = GetDlgItem(hDlg, IDC_COMBO_ADD_CATE);
			HWND text_name = GetDlgItem(hDlg, IDC_EDIT_ADD_NAME);
			HWND text_price = GetDlgItem(hDlg, IDC_EDIT_ADD_PRICE);
			HWND date_add = GetDlgItem(hDlg, IDC_DATETIMEPICKER_ADD_ADDDATE);
			HWND check_isdel = GetDlgItem(hDlg, IDC_CHECK_ADD_ISDEL);
			HWND date_del = GetDlgItem(hDlg, IDC_DATETIMEPICKER_ADD_DELDATE);
			Device dev;
			int textlenght = 0;
			TCHAR id[50], name[50], price[50],type[50];
			textlenght = SendMessage(text_id, EM_GETLINE, 0, (LPARAM)id);
			if (textlenght == 0)
			{
				tip.pszText = TEXT("请填写设备编号!");
				Edit_ShowBalloonTip(text_id, &tip);
				MessageBox(hDlg, TEXT("请填写设备ID!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			id[textlenght] = '\0';
			dev.type=SendMessage(combo_type, CB_GETCURSEL, NULL, NULL);
			if (dev.type < 0)
			{
				MessageBox(hDlg, TEXT("请选择设备种类!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			SendMessage(combo_type, CB_GETLBTEXT, dev.type, (LPARAM)type);
			TcharToChar(type, dev.type_name);
			textlenght = SendMessage(text_name, EM_GETLINE, 0, (LPARAM)name);
			if (textlenght == 0)
			{
				tip.pszText = TEXT("请填写设备名称!");
				Edit_ShowBalloonTip(text_name, &tip);
				MessageBox(hDlg, TEXT("请填写设备名称!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			name[textlenght] = '\0';
			textlenght = SendMessage(text_price, EM_GETLINE, 0, (LPARAM)price);
			if (textlenght == 0)
			{
				tip.pszText = TEXT("请填写设备金额!");
				Edit_ShowBalloonTip(text_price, &tip);
				MessageBox(hDlg, TEXT("请填写设备金额!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			price[textlenght] = '\0';
			SendMessage(date_add, DTM_GETSYSTEMTIME, NULL, (LPARAM)&dev.addtime);
			dev.isdel=SendMessage(check_isdel, BM_GETCHECK, NULL,NULL);
			SendMessage(date_del, DTM_GETSYSTEMTIME, NULL, (LPARAM)&dev.deltime);
			dev.id=TcharToInt(id);
			TcharToChar(name, dev.name);
			dev.price = TCHARToDouble(price);
			if (DeviceVerify(&dev)) {
				if (List_CanAdd(alldevlist, dev))
				{
					List_Add(alldevlist, dev);
				}
				else {
					int res = MessageBox(hDlg, TEXT("已有该设备,是否覆盖?"), TEXT("提示"), MB_OKCANCEL);
					if (res == 1)
					{
						List_Remove(alldevlist,dev);
						List_Add(alldevlist, dev);
					}
				}
				EndDialog(hDlg, LOWORD(wParam));
			}
			else {
				MessageBox(hDlg, TEXT("数据格式错误!"), TEXT("提示"), MB_OK);
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	case WM_NOTIFY:
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK LookDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		HWND text_id = GetDlgItem(hDlg, IDC_EDIT_LOOK_ID);
		HWND combo_type = GetDlgItem(hDlg, IDC_COMBO_LOOK_CATE);
		HWND text_name = GetDlgItem(hDlg, IDC_EDIT_LOOK_NAME);
		HWND text_price = GetDlgItem(hDlg, IDC_EDIT_LOOK_PRICE);
		HWND date_add = GetDlgItem(hDlg, IDC_DATETIMEPICKER_LOOK_ADDDATE);
		HWND check_isdel = GetDlgItem(hDlg, IDC_CHECK_LOOK_ISDEL);
		HWND date_del = GetDlgItem(hDlg, IDC_DATETIMEPICKER_LOOK_DELDATE);
		TCHAR* typelist[50];
		for (int i = 0; i < 50; i++)
		{
			typelist[i] = NULL;
		}
		GetDeviceType(typelist);
		for (int i = 0; i <50; i++)
		{
			if (typelist[i] != NULL)
			{
				SendMessage(combo_type, CB_ADDSTRING, NULL, (LPARAM)typelist[i]);
			}
			else {
				break;
			}
		}
		TCHAR id[50], name[50], price[50];
		NumToTCHAR(lookdev->id, id);
		CharToTchar(lookdev->name, name);
		NumToTCHAR(lookdev->price, price);
		SendMessage(text_id, WM_SETTEXT, NULL, (LPARAM)id);
		SendMessage(combo_type, CB_SETCURSEL, lookdev->type, NULL);
		SendMessage(text_name, WM_SETTEXT, NULL, (LPARAM)name);
		SendMessage(text_price, WM_SETTEXT, NULL, (LPARAM)price);
		SendMessage(date_add, DTM_SETSYSTEMTIME, NULL, (LPARAM)&lookdev->addtime);
		SendMessage(check_isdel, BM_SETCHECK, lookdev->isdel,NULL);
		SendMessage(date_del, DTM_SETSYSTEMTIME, NULL, (LPARAM)&lookdev->deltime);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		else if (LOWORD(wParam) == ID_BUTTON_EDIT)
		{
			editdev = lookdev;
			ShowAddDeviceWindow(alldevlist, &hDlg);
			EndDialog(hDlg, LOWORD(wParam));
		}
		else if (LOWORD(wParam) == ID_BUTTON_DEL)
		{
			int res=MessageBox(hDlg, TEXT("是否删除该设备?"), TEXT("提示"), MB_OKCANCEL);
			if (res == 1)
			{
				List_Remove(alldevlist, *lookdev);
				lookdev = NULL;
				EndDialog(hDlg, LOWORD(wParam));
			}
		}
		break;
	case WM_NOTIFY:
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK FindDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		HWND combo_type = GetDlgItem(hDlg, IDC_COMBO_FIND);
		TCHAR* typelist[50];
		for (int i = 0; i < 50; i++)
		{
			typelist[i] = NULL;
		}
		GetDeviceType(typelist);
		int typenum = 0;
		while (typelist[typenum] != NULL)
		{
			SendMessage(combo_type, CB_ADDSTRING, NULL, (LPARAM)*(typelist + typenum));
			typenum++;
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FIND_TYPE)
		{
			HWND combo_type = GetDlgItem(hDlg, IDC_COMBO_FIND);
			int type=SendMessage(combo_type, CB_GETCURSEL, NULL, NULL);
			if (type < 0)
			{
				MessageBox(hDlg, TEXT("请选择设备种类!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			else {
				HWND listview = GetDlgItem(hDlg, IDC_LIST_FIND);
				LoadListView(&hDlg, IDC_LIST_FIND, type,NULL);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FIND_NAME)
		{
			TCHAR name[50];
			HWND text_name = GetDlgItem(hDlg, IDC_EDIT_FIND);
			int textlenght = SendMessage(text_name, EM_GETLINE, NULL, (LPARAM)name);
			name[textlenght] = '\0';
			if (textlenght == 0)
			{
				MessageBox(hDlg, TEXT("请输入设备名称!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			else {
				char cname[50];
				TcharToChar(name, cname);
				LoadListView(&hDlg, IDC_LIST_FIND, -1, cname);
			}
		}
		break;
	case WM_NOTIFY:

		break;
	case 123:
		HWND mainlist = GetDlgItem(hDlg, IDC_LIST_FIND);
		int index = SendMessage(mainlist, LVM_GETSELECTIONMARK, 0, 0);
		if (index >= 0)
		{
			Device *dev = List_Get(alldevlist, index)->dev;
			ShowLookDeviceWindow(alldevlist, dev, &hDlg);
			LoadListView(&hDlg, IDC_LIST_MAIN, -1, NULL);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK SetTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		HWND mainlist = GetDlgItem(hDlg, IDC_LIST_TYPE);
		ListView_DeleteAllItems(mainlist);
		SendMessage(hDlg, LVM_SETVIEW, LV_VIEW_LIST, NULL);
		DWORD dwExStyle =
			LVS_EX_FULLROWSELECT | //设置可选整行
			LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
			LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
			LVS_EDITLABELS;
		ListView_SetExtendedListViewStyle(mainlist, dwExStyle);
		TCHAR* typelist[50];
		for (int i = 0; i < 50; i++)
		{
			typelist[i] = NULL;
		}
		GetDeviceType(typelist);
		int typenum = 0;
		while (typelist[typenum] != NULL)
		{
			LVITEMW *item = new LVITEMW();
			item->mask = LVIF_TEXT;
			item->iItem = typenum;
			item->pszText = typelist[typenum];
			item->iSubItem = 0;
			ListView_InsertItem(mainlist, item);
			typenum++;
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON_ADD_TYPE)
		{
			TCHAR type[50];
			HWND text_type = GetDlgItem(hDlg, IDC_EDIT_TYPE);
			int textlenght = SendMessage(text_type, EM_GETLINE, NULL, (LPARAM)type);
			type[textlenght] = '\0';
			if (textlenght == 0)
			{
				MessageBox(hDlg, TEXT("请输入设备种类!"), TEXT("提示"), MB_OK);
				return (INT_PTR)TRUE;
			}
			else {
				AddDeviceType(type);
				HWND mainlist = GetDlgItem(hDlg, IDC_LIST_TYPE);
				ListView_DeleteAllItems(mainlist);
				SendMessage(hDlg, LVM_SETVIEW, LV_VIEW_LIST, NULL);
				DWORD dwExStyle =
					LVS_EX_FULLROWSELECT | //设置可选整行
					LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
					LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
					LVS_EDITLABELS;
				ListView_SetExtendedListViewStyle(mainlist, dwExStyle);
				TCHAR* typelist[50];
				for (int i = 0; i < 50; i++)
				{
					typelist[i] = NULL;
				}
				GetDeviceType(typelist);
				int typenum = 0;
				while (typelist[typenum] != NULL)
				{
					LVITEMW *item = new LVITEMW();
					item->mask = LVIF_TEXT;
					item->iItem = typenum;
					item->pszText = typelist[typenum];
					item->iSubItem = 0;
					ListView_InsertItem(mainlist, item);
					typenum++;
				}
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON_DEL_TYPE)
		{
			HWND mainlist = GetDlgItem(hDlg, IDC_LIST_TYPE);
			int index = SendMessage(mainlist, LVM_GETSELECTIONMARK, 0, 0);
			if (index >= 0)
			{
				DelDeviceType(index);
				HWND mainlist = GetDlgItem(hDlg, IDC_LIST_TYPE);
				ListView_DeleteAllItems(mainlist);
				SendMessage(hDlg, LVM_SETVIEW, LV_VIEW_LIST, NULL);
				DWORD dwExStyle =
					LVS_EX_FULLROWSELECT | //设置可选整行
					LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
					LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
					LVS_EDITLABELS;
				ListView_SetExtendedListViewStyle(mainlist, dwExStyle);
				TCHAR* typelist[50];
				for (int i = 0; i < 50; i++)
				{
					typelist[i] = NULL;
				}
				GetDeviceType(typelist);
				int typenum = 0;
				while (typelist[typenum] != NULL)
				{
					LVITEMW *item = new LVITEMW();
					item->mask = LVIF_TEXT;
					item->iItem = typenum;
					item->pszText = typelist[typenum];
					item->iSubItem = 0;
					ListView_InsertItem(mainlist, item);
					typenum++;
				}
			}
			else {
				MessageBox(hDlg, TEXT("请选择要删除的设备种类!"), TEXT("提示"), MB_OK);
			}
		}
		break;
	case WM_NOTIFY:

		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK AboutWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		///窗口加载完成
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_NOTIFY:
		
		break;
	}
	return (INT_PTR)FALSE;
}