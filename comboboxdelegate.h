﻿#pragma once
#pragma execution_character_set("utf-8")

#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

private:
    QStringList m_ItemList;//选择列表
    bool m_isEdit; //是否可编辑
    int type = 1;   //0编号映射，1文本映射

public:
    ComboBoxDelegate(QObject *parent = 0);

    void setItems(QStringList items, bool isEdit);//初始化设置列表内容，是否可编辑
    //自定义代理组件必须继承以下4个函数
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setType(int type);
};

#endif // QWCOMBOBOXDELEGATE_H
