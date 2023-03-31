#pragma once
#pragma execution_character_set("utf-8")

#ifndef POSTERWORK_H
#define POSTERWORK_H

#include <QObject>
#include "service.h"

class PosterWork  : public QObject
{
	Q_OBJECT

public:
	PosterWork(QObject* parent = nullptr);
	~PosterWork();
	void working();

	QSqlDatabase getDB();
	void submitAll();
	void setModel(QSqlTableModel* model);
	void setManageModel(QSqlTableModel* model);
	void setWorkType(int type);
	void poster_statistics();
	void setFilter(int type, const QString& filter);

	bool cache = false;		//�����ݻ���
	int cacheRow = -1;		//����row
	bool isDisplay = false;

private:
	int workType = -1;	//1����ҳ�棻0�û�ҳ��
	service db_service;
	QSqlDatabase DB, DB_SECOND;
	QSqlTableModel* tabModel, *manageModel;
	QTimer* heartBeat;
signals:
	void contentsManageWorkFinished();
	void contentsWorkFinished();
	void submitAllFinished(bool);
	void operateFinished(QString error);
};

#endif //POSTERWORK_H