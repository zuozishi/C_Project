#pragma once
#include "DevicesModel.h"
// ���뾲̬��
#pragma comment(lib, "Comctl32.lib")
// �����Ӿ�Ч�� Copy from MSDN
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//UI��������
//��ʾ������
void ShowMainWindow(DeviceList *list);
//��ʾ����豸�Ի���
void ShowAddDeviceWindow(DeviceList *list, HWND *window);
//��ʾ�鿴�豸�Ի���
void ShowLookDeviceWindow(DeviceList *list,Device *dev, HWND *window);
//��ʾ�����豸�Ի���
void ShowFindDeviceWindow(DeviceList *list, HWND *window);
//��ʾ�豸�������öԻ���
void ShowSetTypeWindow(HWND *window);
//��ʾ���ڶԻ���
void ShowAboutWindow(HWND *window);
//���豸�������ݼ��ص�ListView�ؼ�(��ɸѡ)
//windows->���ھ��,listview->ListView�ؼ����,type->�豸����ɸѡ(����-1��Ч),sname->���豸����ɸѡ(����NULL��Ч)
void LoadListView(HWND *window, int listview, int type, char *sname);

//Windows������Ϣ�ص�����
BOOL CALLBACK MainWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AddDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LookDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);