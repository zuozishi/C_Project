#include "stdafx.h"
#include "windows.h"

#include "DevicesModel.h"
#include "Converter.h"

void LoadFromFile(DeviceList *list,char * path)
{
	FILE *fp;
	if (path == NULL)
	{
		fp = fopen("devdata.dat", "r");
	}
	else {
		fp = fopen(path, "r");
	}
	if (fp == NULL)return;
	List_Clear(list);
	while (feof(fp) == 0)
	{
		Device dev;
		fread(&dev, sizeof(Device), 1, fp);
		if(DeviceVerify(&dev))List_Add(list,dev);
	}
	fclose(fp);
}

void ExportToFile(DeviceList *list,char * path)
{
	FILE *fp;
	if(path == NULL)
	{
		fp = fopen("devdata.dat", "w");
	}
	else {
		fp = fopen(path, "w");
	}
	int num = List_Count(list);
	for (int i = 0; i < num; i++)
	{
		DeviceList *dev = List_Get(list, i);
		fwrite(dev->dev, sizeof(Device), 1, fp);
	}
	fclose(fp);
}

void ExportToFileCSV(DeviceList * list, char * path)
{
	FILE *fp;
	if (path == NULL)
	{
		fp = fopen("devdata.csv", "w");
	}
	else {
		fp = fopen(path, "w");
	}
	fprintf(fp, "设备ID,名称,种类,价格,购入时间,是否报废,报废时间\n");
	int num = List_Count(list);
	for (int i = 0; i < num; i++)
	{
		char isdel[50],addtime[50],deltime[50];
		Device *dev = List_Get(list, i)->dev;
		SystemTimeToChar(&dev->addtime, addtime);
		SystemTimeToChar(&dev->deltime, deltime);
		if (dev->isdel)
		{
			strcpy(isdel, "是");
		}
		else {
			strcpy(isdel, "否");
			strcpy(deltime, "");
		}
		fprintf(fp, "%d,%s,%s,%f,%s,%s,%s,\n", dev->id, dev->name,dev->type_name, dev->price, addtime, isdel, deltime);
	}
	fclose(fp);
}

void GetDeviceType(TCHAR* typelist[50])
{
	int i = 0;
	FILE* fp = fopen("devicetype.dat", "r");
	if (fp != NULL)
	{
		while (feof(fp)==0)
		{
			char str[50];
			fscanf(fp, "%s\n",&str[0]);
			typelist[i] = new TCHAR[50];
			CharToTchar(str, typelist[i]);
			i++;
		}
		fclose(fp);
	}
	else {
		fp = fopen("devicetype.dat", "w");
		fprintf(fp, "微机\n打印机\n显示器\n");
		fclose(fp);
		GetDeviceType(typelist);
	}
}

void AddDeviceType(TCHAR *type)
{
	TCHAR* typelist[50];
	for (int i = 0; i < 50; i++)
	{
		typelist[i] = NULL;
	}
	GetDeviceType(typelist);
	bool canadd = true;
	int i = 0;
	while (typelist[i] != NULL)
	{
		char str1[50], str2[50];
		TcharToChar(typelist[i], str1);
		TcharToChar(type, str2);
		if (strcmp(str1, str2) == 0)canadd = false;
		i++;
	}
	if (canadd) {
		typelist[i] = new TCHAR[50]; 
		char str[50];
		TcharToChar(type, str);
		CharToTchar(str, typelist[i]);
		FILE* fp = fopen("devicetype.dat", "w");
		i = 0;
		while (typelist[i] != NULL)
		{
			char str[50];
			TcharToChar(typelist[i], str);
			fprintf(fp, "%s\n", str);
			i++;
		}
		fclose(fp);
	}
}

void DelDeviceType(TCHAR *type)
{
	char ctype[50],temptype[50];
	TcharToChar(type, ctype);
	TCHAR* typelist[50];
	for (int i = 0; i < 50; i++)
	{
		typelist[i] = NULL;
	}
	GetDeviceType(typelist);
	FILE *fp = fopen("devicetype.dat", "w");
	int typenum = 0;
	while (typelist[typenum] != NULL)
	{
		TcharToChar(typelist[typenum], temptype);
		if (strcmp(ctype, temptype) != 0)
		{
			fprintf(fp, "%s\n", temptype);
		}
		typenum++;
	}
	fclose(fp);
}

void DelDeviceType(int num)
{
	char temptype[50];
	TCHAR* typelist[50];
	for (int i = 0; i < 50; i++)
	{
		typelist[i] = NULL;
	}
	GetDeviceType(typelist);
	FILE *fp = fopen("devicetype.dat", "w");
	int typenum = 0;
	while (typelist[typenum] != NULL)
	{
		TcharToChar(typelist[typenum], temptype);
		if (typenum != num)
		{
			fprintf(fp, "%s\n", temptype);
		}
		typenum++;
	}
	fclose(fp);
}

bool DeviceVerify(Device * dev)
{
	bool res = true;
	if (strlen(dev->name) == 0)res = false;
	if (dev->id<0)res = false;
	if (dev->type<0)res = false;
	return res;
}

DeviceList* List_Get(DeviceList *list,int i)
{
	if (i == 0) {
		return list;
	}
	else {
		DeviceList* obj = list;
		while (i>0)
		{
			obj = obj->node;
			i--;
		}
		return obj;
	}
}

int List_Count(DeviceList *list)
{
	if (list->dev != NULL) {
		int num = 1;
		DeviceList *node = list;
		while (node->node != NULL)
		{
			num++;
			node = node->node;
		}
		return num++;
	}
	else {
		return 0;
	}
}

bool List_CanAdd(DeviceList *list, Device dev)
{
	bool res = true;
	int num = List_Count(list);
	for (int i = 0; i < num; i++)
	{
		if (dev.id == List_Get(list, i)->dev->id)
		{
			res = false;
			break;
		}
	}
	return res;
}

void List_Add(DeviceList * list, Device dev)
{
	if (List_CanAdd(list, dev))
	{
		if (list->dev == NULL) {
			list->dev = new Device();
			*list->dev = dev;
		}
		else {
			DeviceList* node = List_End(list);
			node->node = new DeviceList();
			node->node->dev = new Device();
			*node->node->dev = dev;
		}
		if (list->listcallback != NULL)list->listcallback();
	}
}

void List_Clear(DeviceList * list)
{
	list->dev = NULL;
	list->node = NULL;
}

DeviceList * List_End(DeviceList * list)
{
	if (list->node == NULL)
	{
		return list;
	}
	else {
		DeviceList *node = list->node;
		while (node->node != NULL)
		{
			node = node->node;
		}
		return node;
	}
}

void List_Remove(DeviceList * list, Device dev)
{
	int count = List_Count(list);
	for (int i = 0; i < count; i++)
	{
		if (dev.id == List_Get(list, i)->dev->id)
		{
			List_RemoveAt(list,i);
			break;
		}
	}
}

void List_RemoveAt(DeviceList * list, int i)
{
	int count = List_Count(list);
	if (i >= count)return;
	if (i == 0)
	{
		if (list->node == NULL)
		{
			list->dev = NULL;
		}
		else {
			list->dev = list->node->dev;
			list->node = list->node->node;
		}
		if (list->listcallback != NULL)list->listcallback();
		return;
	}
	if (i == count - 1 && i > 1)
	{
		List_Get(list,i-1)->node = NULL;
	}
	else {
		i--;
		DeviceList *node = NULL;
		while (i!=0)
		{
			if (node == NULL)
			{
				node = list->node;
			}
			else {
				node = node->node;
			}
			i--;
		}
		if (node == NULL)
		{
			list->node = list->node->node;
		}
		else {
			node->node = node->node->node;
		}
	}
	if (list->listcallback != NULL)list->listcallback();
}