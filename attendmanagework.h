﻿#pragma once
#pragma execution_character_set("utf-8")

#ifndef ATTENDMANAGEWORK_H
#define ATTENDMANAGEWORK_H

#include <QObject>
#include "querymodel.h"
#include "service.h"

class AttendManageWork : public QObject
{
    Q_OBJECT
public:
    explicit AttendManageWork(QObject *parent = nullptr);
    ~AttendManageWork();
    bool isFirst = true;    //是否为首次加载model
    void working();
    void setCurAvatarUrl(const QString url);
    //void setUserModel(QSqlRelationalTableModel *relTableModel);
    //void setAttendModel(QSqlRelationalTableModel *relTableModel);
    void getComboxItems(QStringList& comboxItems_group, QStringList& comboxItems_department);  //已弃用
    void submitAll(int type);   //1补签 0退签
    void loadAvatar();
    void setCombox(QComboBox* group, QComboBox* department);
    void dataOperate(int type);
    void setFilter(int type, const QString& filter);
    bool isDisplay = false;
    QSqlDatabase getDB();
    QSqlRelationalTableModel* getUserModel();
    QSqlRelationalTableModel* getAttendModel();
private:
    service db_service;
    QComboBox *m_group, *m_department;
    QPixmap curPix;
    QString avatarUrl;
    QSqlDatabase DB, DB_SECOND;
    QStringList comboxItems_group, comboxItems_department;
    QSqlRelationalTableModel *userModel, *attendModel;
    QTimer* heartBeat;
    QQueue<QSqlRelationalTableModel*> userModelQueue, attendModelQueue;
    void getComboxItems();
signals:
    void attendManageWorkFinished();
    void submitAddFinished(bool);
    void submitDelFinished(bool);
    void avatarFinished(QPixmap curPix);
    void dataOperateFinished(bool);
};

#endif // ATTENDMANAGEWORK_H
