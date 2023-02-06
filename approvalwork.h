#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include "service.h"

class ApprovalWork  : public QObject
{
	Q_OBJECT

private:
	service db_service;
	QSqlDatabase DB;
	QList<QByteArray> applyItems;
	QHash<QString, QString> auditorName;
	QList<QString> auditorList;
	QString modifyItemID;
	
public:
	void getManagePageApplyItems(const QString& uid);
	void getManagePageAuditorList();	//��ȡ�������Ȩ�޵���Ա�б�
	void addOrModifyApplyItem(int type, QByteArray array);	//0������1�޸������� ByteArray����->ѡ��->������->����->isHide
	QList<QByteArray> getApplyItems();
	QList<QString> getAuditorList();
	QString getAuditorName(const QString& uid);
	void setModifyItemID(const QString& id);
	void deleteOrSwitchApplyItem(int type, const QString& id);	//0ɾ���� 1����/��ͣ ������
	
private slots:

signals:
	void getManagePageApplyItemsFinished();
	void addOrModifyApplyItemFinished(bool res);
	void deleteOrSwitchApplyItemFinished(bool res);

public:
	ApprovalWork(QObject *parent = nullptr);
	~ApprovalWork();
};
