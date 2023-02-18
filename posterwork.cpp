#include "posterwork.h"

PosterWork::PosterWork(QObject *parent)
	: QObject(parent)
{
    db_service.addDatabase(DB, "PosterManageWork_DB");
    db_service.addDatabase(DB_SECOND, "PosterManageWork_DB_SECOND");

    DB.setConnectOptions("MYSQL_OPT_RECONNECT=1");  //��ʱ����
    DB.open();
}

PosterWork::~PosterWork()
{
    DB.close();
}

void PosterWork::working()
{
    if (workType == -1)
        return;
    if (workType != 1)
    {
        tabModel->setTable("magic_contents");
        tabModel->setSort(tabModel->fieldIndex("c_id"), Qt::DescendingOrder);
        tabModel->setHeaderData(tabModel->fieldIndex("c_id"), Qt::Horizontal, "���");
        tabModel->setHeaderData(tabModel->fieldIndex("title"), Qt::Horizontal, "����");
        tabModel->setHeaderData(tabModel->fieldIndex("author_id"), Qt::Horizontal, "������");
        tabModel->setFilter("isHide = 0");  //����ʾδ����
        tabModel->select();
        
        emit contentsWorkFinished();
    }
    else
    {
        manageModel->setTable("magic_contents");
        manageModel->setSort(manageModel->fieldIndex("c_id"), Qt::DescendingOrder);
        manageModel->setEditStrategy(QSqlTableModel::OnRowChange);
        manageModel->setHeaderData(manageModel->fieldIndex("c_id"), Qt::Horizontal, "���");
        manageModel->setHeaderData(manageModel->fieldIndex("title"), Qt::Horizontal, "����");
        manageModel->select();

        emit contentsManageWorkFinished();
    }
}

QSqlDatabase PosterWork::getDB()
{
    return DB;
}

void PosterWork::submitAll()
{
    manageModel->submitAll();
}

void PosterWork::setModel(QSqlTableModel* model)
{
    tabModel = model;
}

void PosterWork::setManageModel(QSqlTableModel* model)
{
    manageModel = model;
}

void PosterWork::setWorkType(int type)
{
    workType = type;
}

void PosterWork::poster_statistics()
{
    DB_SECOND.open();
    QSqlQuery statistics(DB_SECOND);

    //ͳ�ƶ�̬������
    statistics.exec("SELECT * FROM magic_statistics WHERE date='" + QDateTime::currentDateTime().date().toString("yyyy-MM-dd") + "'");
    if (statistics.next())
        statistics.exec("UPDATE magic_statistics SET dynamics_cnt=dynamics_cnt+1 WHERE date='" + QDateTime::currentDateTime().date().toString("yyyy-MM-dd") + "'");
    else
        statistics.exec("INSERT INTO magic_statistics (date, dynamics_cnt) VALUES ('" + QDateTime::currentDateTime().date().toString("yyyy-MM-dd") + "', 1)");
    statistics.clear();
    DB_SECOND.close();
}

void PosterWork::setFilter(int type, const QString& filter)
{
    if (type == 0)
        tabModel->setFilter(filter);
    else
        manageModel->setFilter(filter);
}
