// LEM_System.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "DevicesModel.h"//设备及其链表库
#include "Converter.h"//数据转换库
#include "UIManager.h"//UI处理库

DeviceList devlist;
void WINAPI listcallback();//链表操作回调函数

int main()
{
	system("color fc");
	devlist.dev = NULL;
	devlist.node = NULL;
	LoadFromFile(&devlist);
	devlist.listcallback = listcallback;
	printf("**********************************\n");
	printf("*请输入1进入管理系统,按其它键退出*\n");
	printf("**********************************\n");
	char a = getchar();
	if (a == '1')
	{
		printf("正在运行中,请勿关闭该窗口...\n");
		ShowMainWindow(&devlist);
	}
	else {
		exit(0);
	}
	return 0;
}

void WINAPI listcallback()
{
	ExportToFile(&devlist);
}