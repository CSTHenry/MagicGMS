﻿#pragma once
#pragma execution_character_set("utf-8")

#ifndef SQLWORK_H
#define SQLWORK_H

#include <QtSql>
#include <QThread>
#include <QSqlQuery>
#include "service.h"

class SqlWork : public QObject
{
    Q_OBJECT
private:
    int cnt = 1;
    QSqlDatabase DB;    //在主窗口中已弃用，仅用于登录窗口
    QSqlDatabase testDB;    //用于测试DB状态，防止query冲突
    QString dbName;
    service dbService;
    QSqlQuery *testDbConnection;
    bool run = true;
    bool status = false;
    bool isPaused = true;
    bool isStop = true; //停止线程
    bool dbStatus = false;  //心跳验证状态

public:
    explicit SqlWork(QString dbName);
    ~SqlWork();
    void working();
    void beginThread();
    void stopThread();
    void quit();    //用于结束working函数，结束后需重新emit触发working
    bool getisPaused();
    QSqlDatabase getDb();
    QSqlDatabase getTestDb();
    QString getDbName();

signals:
    void newStatus(bool status);
    void firstFinished();
};

#endif // SQLWORK_H
