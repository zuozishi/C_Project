#pragma once
#include "stdafx.h"
#include "windows.h"

//设备结构体
struct Device
{
	int id;//设备编号
	char name[50];//设备名称
	int type;//设备种类(标号,用于定位ComboBox控件)
	char type_name[50];//设备种类
	double price;//设备价格
	SYSTEMTIME addtime;//设备购买时间
	bool isdel;//是否报废
	SYSTEMTIME deltime;//报废时间
};

//链表操作回调
typedef void(CALLBACK* LISTOPT)();

//设备链表结构体
struct DeviceList {
	Device*  dev;//当前节点的设备
	DeviceList* node;//下一个节点
	LISTOPT listcallback;//回调函数,向主线程通知链表数据已改变
};

//从文件恢复链表
//list->链表,path->文件位置(默认为"devdata.dat")
void LoadFromFile(DeviceList *list,char *path = NULL);

//导出链表数据到文件
//list->链表,path->文件保存位置(默认为"devdata.dat")
void ExportToFile(DeviceList *list,char *path = NULL);

//导出链表数据到文件(CSV格式)
//list->链表,path->文件保存位置(默认为"devdata.csv")
void ExportToFileCSV(DeviceList *list, char *path = NULL);

//获取设备种类
void GetDeviceType(TCHAR* typelist[50]);
//添加设备种类
void AddDeviceType(TCHAR *type);
//删除设备种类
void DelDeviceType(TCHAR *type);
//删除设备种类
void DelDeviceType(int num);

//验证数据是否合法
bool DeviceVerify(Device *dev);
//获取第i个节点
DeviceList* List_Get(DeviceList *list,int i);
//获取链表中的节点数量
int List_Count(DeviceList* list);
//判断是否有相同ID的设备
bool List_CanAdd(DeviceList *list, Device dev);
//向链表里添加节点
void List_Add(DeviceList * list, Device dev);
//清空链表里的节点
void List_Clear(DeviceList* list);
//获取最后一个节点
DeviceList* List_End(DeviceList* list);
//删除指定的节点
void List_Remove(DeviceList* list, Device  dev);
//删除第i个节点
void List_RemoveAt(DeviceList* list,int i);