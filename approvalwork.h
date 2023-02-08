#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include "service.h"

class ApprovalWork  : public QObject
{
	Q_OBJECT

private:
	service db_service;
	QSqlDatabase DB, DB_SECOND;
	QList<QByteArray> applyItems, applyForms;
	QHash<QString, QString> auditorName, applyItemTitle;
	QList<QString> auditorList;
	QString modifyItemID;
	QList<QByteArray> currentProcess;	//��ǰ�������̽��ȣ��Ѱ�����˳�����򣩣�����result->result_text->operate_time
	QHash<QString, QList<QByteArray>> applyFormsProcess;	//������ǰ�û�������������������
	QHash<QString, QByteArray> simpleApplyItems; //simpleApplyItems ��������magic_applyItems�е�title��options
	QHash<QString, QList<QString>> applyAuditorList;
	QHash<QString, QString> currentFormOptionsText;	//��ǰ����� �û���д���ı�
	QList<QByteArray> applyFormList, applyFormListDone;	//�ȴ���˵��б��Ѿ���˵��б�apply_id, uid, item_id, options, operate_time��
	QList<QString> authApplyTokenResultList;	//���������id�������ˣ�������Ŀ�������״̬������ʱ��
	
public:
	void getManagePageApplyItems(const QString& uid);
	void getUserPageApplyItems(const QString& uid);
	void getManagePageAuditorList();	//��ȡ�������Ȩ�޵���Ա�б�
	void getAllApplyFormList(const QString& uid);	//��ȡ�������й�����Ա���
	void getApplyProcess(const QString& apply_id, const QString& item_id);
	void addOrModifyApplyItem(int type, QByteArray array);	//0������1�޸������� ByteArray ����->ѡ��->������->����->isHide
	QByteArray getSimpleApplyItems(const QString& item_id);
	QList<QByteArray> getApplyFormList();
	QList<QByteArray> getApplyFormListDone();
	QList<QByteArray> getApplyItems();
	QList<QByteArray> getApplyForms();
	QList<QString> getAuditorList();
	QString getAuditorName(const QString& uid);
	QString getApplyItemTitle(const QString& id);
	void getApplyToken(const QString& id);
	void setModifyItemID(const QString& id);
	void deleteOrSwitchApplyItem(int type, const QString& id);	//0ɾ���� 1����/��ͣ ������
	void agreeOrRejectApply(const QString& apply_id, const QString& auditor, const QString& result, const QString& text);
	void submitOrCancelApply(int type, const QString& apply_id, QByteArray array = QByteArray());	//1�ύ��0��������
	void authApplyToken(const QString& token);
	QList<QByteArray> getCurrentApplyProcess(const QString& id);
	QString getCurrentFormOptionsText(const QString& id);	//��ǰ���������д����ı�
	QList<QString> getAuthApplyTokenResultList();
	
private slots:

signals:
	void getManagePageApplyItemsFinished();
	void getUserPageApplyItemsFinished();
	void getApplyFormListFinished();
	void addOrModifyApplyItemFinished(bool res);
	void deleteOrSwitchApplyItemFinished(bool res);
	void submitOrCancelApplyFinished(bool res);
	void getApplyTokenFinished(QString token);
	void agreeOrRejectApplyFinished(bool res);
	void authApplyTokenFinished(bool res);

public:
	ApprovalWork(QObject *parent = nullptr);
	~ApprovalWork();
};
