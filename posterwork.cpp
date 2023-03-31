#include "posterwork.h"

PosterWork::PosterWork(QObject *parent)
	: QObject(parent)
{
    db_service.addDatabase(DB, "PosterManageWork_DB");
    db_service.addDatabase(DB_SECOND, "PosterManageWork_DB_SECOND");

    //DB.setConnectOptions("MYSQL_OPT_RECONNECT=1");  //��ʱ����
    heartBeat = new QTimer(this);
    connect(heartBeat, &QTimer::timeout, this, [=]() {
        if (isDisplay)
        {
            if(workType == 1)
                manageModel->select();
			else if(workType == 2)
				tabModel->select();
        }
        else
            if (DB.isOpen())
                DB.close();
        });
    heartBeat->start(MYSQL_TIME_OUT);
}

PosterWork::~PosterWork()
{
    heartBeat->stop();
    if (DB.isOpen())
        DB.close();
}

void PosterWork::working()
{
    if (workType == -1)
        return;
    if (workType != 1)
    {
        if (!DB.isOpen())
            DB.open();
        isDisplay = true;
        tabModel->setTable("magic_contents");
        tabModel->setSort(tabModel->fieldIndex("c_id"), Qt::DescendingOrder);
        tabModel->setHeaderData(tabModel->fieldIndex("c_id"), Qt::Horizontal, "���");
        tabModel->setHeaderData(tabModel->fieldIndex("title"), Qt::Horizontal, "����");
        tabModel->setHeaderData(tabModel->fieldIndex("author_id"), Qt::Horizontal, "������");
        tabModel->setFilter("isHide = 0");  //����ʾδ����
        tabModel->select();
        while (tabModel->canFetchMore())
            tabModel->fetchMore();  //���س���256����������
        emit contentsWorkFinished();
    }
    else
    {
        if (!DB.isOpen())
            DB.open();
        isDisplay = true;
        manageModel->setTable("magic_contents");
        manageModel->setSort(manageModel->fieldIndex("c_id"), Qt::DescendingOrder);
        manageModel->setEditStrategy(QSqlTableModel::OnRowChange);
        manageModel->setHeaderData(manageModel->fieldIndex("c_id"), Qt::Horizontal, "���");
        manageModel->setHeaderData(manageModel->fieldIndex("title"), Qt::Horizontal, "����");
        manageModel->select();
        while (manageModel->canFetchMore())
            manageModel->fetchMore();  //���س���256����������
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
