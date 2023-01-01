#include "friendswidget.h"

FriendsWidget::FriendsWidget(QWidget *parent)
	: QWidget(parent)
    , ui(new Ui::FriendsWidgetClass)
{
	ui->setupUi(this);

    //����HarmonyOS����
    QFont font;
    int font_Id = QFontDatabase::addApplicationFont(":/src/font/HarmonyOS_Sans_SC_Regular.ttf");
    QStringList fontName = QFontDatabase::applicationFontFamilies(font_Id);
    font.setFamily(fontName.at(0));
    auto listWidget = findChildren<QWidget*>();
    for (auto& widget : listWidget) //�����������
    {
        font.setWeight(widget->font().weight());
        font.setPointSize(widget->font().pointSize());
        widget->setFont(font);
    }

    //���߳�
    thread = new QThread();
    msgService = new MsgService(nullptr, 2);
    msgService->moveToThread(thread);
    thread->start();

    connect(this, &FriendsWidget::searchMember, msgService, &MsgService::searchMember);
    connect(msgService, &MsgService::searchMemRes, this, &FriendsWidget::setMemberInfo);
    connect(this, &FriendsWidget::sendApply, msgService, &MsgService::sendApply);
    connect(this, &FriendsWidget::loadApplyInfo, msgService, &MsgService::loadApplyInfo);
    connect(this, &FriendsWidget::operateApply, msgService, &MsgService::operateApply);
    connect(msgService, &MsgService::sendApplyFinished, this, [=](QString res) {
        ui->btn_sendApply->setText("������֤��Ϣ");
        ui->btn_sendApply->setEnabled(true);
        if(res == "Exist")
            QMessageBox::warning(this, "����", "�Ѵ��ڷ��͵���֤��Ϣ��ǰ�ѻ�Ϊ���ѡ�", QMessageBox::Ok);
        else if (res == "OK")
        {
            ui->lineEdit_applyInfo->clear();
            QMessageBox::information(this, "��ʾ", "���ͺ�����֤�ɹ����ȴ��Է���֤��", QMessageBox::Ok);
        }
        else
            QMessageBox::warning(this, "����", res, QMessageBox::Ok);
        });
    connect(msgService, &MsgService::loadApplyInfoFinished, this, [=](QString res) {
        ui->textBrowser_applyInfo->setText(res);
        if (!this->isVisible())
        {
            this->showMinimized();
            this->showNormal();
        }
        emit loadApplyInfoFinished();
        });
    connect(msgService, &MsgService::operateApplyFinished, this, [=](QString res) {
        ui->label_status->setText("�������");
        ui->btn_agree->setEnabled(true);
        ui->btn_reject->setEnabled(true);
        if (res.indexOf("������Ϣ") != -1)
        {
            QMessageBox::warning(this, "����", res, QMessageBox::Ok);
            return;
        }
        if (res == "OK_1")
            QMessageBox::information(this, "��ʾ", QString("�ѳɹ���� [%1] Ϊ���ѣ���ˢ�º����б�").arg(applyUid), QMessageBox::Ok);
        else
            QMessageBox::information(this, "��ʾ", QString("�Ѿܾ���� [%1] Ϊ���ѡ�").arg(applyUid), QMessageBox::Ok);
        applyUid = -1;

        initInfo();
        this->close();
        });
}

FriendsWidget::~FriendsWidget()
{
    //�ڴ˴��ȴ������߳�ֹͣ
    if (thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }

    delete msgService;
    delete thread;
}

void FriendsWidget::loading()
{
    QString str = "������...";
    ui->uid->setText(str);
    ui->name->setText(str);
    ui->group->setText(str);
    ui->department->setText(str);
    ui->avatar->setPixmap(QPixmap(":/images/color_icon/user.svg"));
}

void FriendsWidget::initInfo()
{
    QString str = "--";
    ui->uid->setText(str);
    ui->name->setText(str);
    ui->group->setText(str);
    ui->department->setText(str);
    ui->avatar->setPixmap(QPixmap(":/images/color_icon/user.svg"));
    ui->textBrowser_applyInfo->clear();
    ui->lineEdit_searchUid->clear();
    ui->lineEdit_applyInfo->clear();
    ui->label_status->setText("");
}

void FriendsWidget::loadApply(const QString& uid)
{
    applyUid = uid;
    initInfo();
    loading();
    emit searchMember(applyUid);
    emit loadApplyInfo(this->uid, applyUid);
    ui->group_addFriends->setEnabled(false);
    ui->group_applyInfo->setEnabled(true);
}

void FriendsWidget::setMemberInfo(QByteArray array)
{
    ui->btn_search->setEnabled(true);
    QDataStream stream(&array, QIODevice::ReadOnly);
    QString name, group, department;
    QPixmap avatar;
    bool res;
    stream >> name >> group >> department >> avatar >> res;

    ui->group->setText(group);
    ui->department->setText(department);
    ui->avatar->setPixmap(avatar);
    if (res)
    {
        ui->uid->setText(searchUid);
        ui->name->setText(name);
    }
    else
    {
        searchUid = "-1";
        ui->uid->setText("�û�������");
        ui->name->setText("--");
    }
    if(applyUid != "-1")
        ui->uid->setText(applyUid);
}

void FriendsWidget::on_btn_sendApply_clicked()
{
    if (ui->lineEdit_applyInfo->text().isEmpty() || searchUid == "-1")
    {
        QMessageBox::warning(this, "����", "����д��ȷ����Ϣ�����ԡ�", QMessageBox::Ok);
        return;
    }
    ui->btn_sendApply->setText("��������...");
    ui->btn_sendApply->setEnabled(false);
    emit sendApply(uid, searchUid, ui->lineEdit_applyInfo->text());
}

void FriendsWidget::on_btn_agree_clicked()
{
    ui->label_status->setText("������...");
    ui->btn_agree->setEnabled(false);
    ui->btn_reject->setEnabled(false);
    emit operateApply(uid, applyUid, 1);
}

void FriendsWidget::on_btn_reject_clicked()
{
    ui->label_status->setText("������...");
    ui->btn_agree->setEnabled(false);
    ui->btn_reject->setEnabled(false);
    emit operateApply(uid, applyUid, 2);
}

void FriendsWidget::on_btn_search_clicked()
{
    if (ui->lineEdit_searchUid->text().isEmpty())
        return;
    loading();
    applyUid = "-1";
    searchUid = ui->lineEdit_searchUid->text();
    ui->btn_search->setEnabled(false);
    emit searchMember(ui->lineEdit_searchUid->text());
}

void FriendsWidget::setUid(const QString& uid)
{
    this->uid = uid;
}

void FriendsWidget::set_group_applyInfo_enabled(bool flag)
{
    ui->group_applyInfo->setEnabled(flag);
}

void FriendsWidget::set_group_addFriends_enabled(bool flag)
{
    ui->group_addFriends->setEnabled(flag);
}
