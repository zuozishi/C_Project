#pragma once
#include "stdafx.h"
#include "windows.h"

//�豸�ṹ��
struct Device
{
	int id;//�豸���
	char name[50];//�豸����
	int type;//�豸����(���,���ڶ�λComboBox�ؼ�)
	char type_name[50];//�豸����
	double price;//�豸�۸�
	SYSTEMTIME addtime;//�豸����ʱ��
	bool isdel;//�Ƿ񱨷�
	SYSTEMTIME deltime;//����ʱ��
};

//��������ص�
typedef void(CALLBACK* LISTOPT)();

//�豸����ṹ��
struct DeviceList {
	Device*  dev;//��ǰ�ڵ���豸
	DeviceList* node;//��һ���ڵ�
	LISTOPT listcallback;//�ص�����,�����߳�֪ͨ���������Ѹı�
};

//���ļ��ָ�����
//list->����,path->�ļ�λ��(Ĭ��Ϊ"devdata.dat")
void LoadFromFile(DeviceList *list,char *path = NULL);

//�����������ݵ��ļ�
//list->����,path->�ļ�����λ��(Ĭ��Ϊ"devdata.dat")
void ExportToFile(DeviceList *list,char *path = NULL);

//�����������ݵ��ļ�(CSV��ʽ)
//list->����,path->�ļ�����λ��(Ĭ��Ϊ"devdata.csv")
void ExportToFileCSV(DeviceList *list, char *path = NULL);

//��ȡ�豸����
void GetDeviceType(TCHAR* typelist[50]);
//����豸����
void AddDeviceType(TCHAR *type);
//ɾ���豸����
void DelDeviceType(TCHAR *type);
//ɾ���豸����
void DelDeviceType(int num);

//��֤�����Ƿ�Ϸ�
bool DeviceVerify(Device *dev);
//��ȡ��i���ڵ�
DeviceList* List_Get(DeviceList *list,int i);
//��ȡ�����еĽڵ�����
int List_Count(DeviceList* list);
//�ж��Ƿ�����ͬID���豸
bool List_CanAdd(DeviceList *list, Device dev);
//����������ӽڵ�
void List_Add(DeviceList * list, Device dev);
//���������Ľڵ�
void List_Clear(DeviceList* list);
//��ȡ���һ���ڵ�
DeviceList* List_End(DeviceList* list);
//ɾ��ָ���Ľڵ�
void List_Remove(DeviceList* list, Device  dev);
//ɾ����i���ڵ�
void List_RemoveAt(DeviceList* list,int i);