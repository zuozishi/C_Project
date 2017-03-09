#pragma once
#include "DevicesModel.h"
// 导入静态库
#pragma comment(lib, "Comctl32.lib")
// 开启视觉效果 Copy from MSDN
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//UI操作方法
//显示主窗口
void ShowMainWindow(DeviceList *list);
//显示添加设备对话框
void ShowAddDeviceWindow(DeviceList *list, HWND *window);
//显示查看设备对话框
void ShowLookDeviceWindow(DeviceList *list,Device *dev, HWND *window);
//显示查找设备对话框
void ShowFindDeviceWindow(DeviceList *list, HWND *window);
//显示设备种类设置对话框
void ShowSetTypeWindow(HWND *window);
//显示关于对话框
void ShowAboutWindow(HWND *window);
//将设备链表数据加载到ListView控件(可筛选)
//windows->窗口句柄,listview->ListView控件句柄,type->设备种类筛选(置于-1无效),sname->按设备名称筛选(置于NULL无效)
void LoadListView(HWND *window, int listview, int type, char *sname);

//Windows窗体消息回调函数
BOOL CALLBACK MainWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AddDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LookDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);