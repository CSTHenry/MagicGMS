#include "friendinfowidget.h"

FriendInfoWidget::FriendInfoWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FriendInfoWidgetClass())
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

    clipboard = QApplication::clipboard();  //��ȡϵͳ���а�ָ��
    
    //���߳�
    thread = new QThread();
    infoWork = new baseInfoWork();
    infoWork->moveToThread(thread);
    thread->start();
    
    connect(this, &FriendInfoWidget::getCurrentMemInfo, infoWork, &baseInfoWork::loadBaseInfoWorking);
    connect(infoWork, &baseInfoWork::baseInfoFinished, this, &FriendInfoWidget::setInfoPage);
}

void FriendInfoWidget::setLoading()
{
    ui->btn_share->setEnabled(false);
    ui->uid->setText("������...");
    ui->name->setText("������...");
    ui->department->setText("������...");
    ui->group->setText("������...");
    ui->phone->setText("������...");
    ui->mail->setText("������...");
    ui->verifyType->setText("������...");
    ui->verifyInfo->setText("--");
    ui->avatar->setPixmap(QPixmap(":/images/color_icon/user.svg"));
    ui->verifyIcon->setPixmap(QPixmap(":/images/color_icon/color-delete.svg"));
}

void FriendInfoWidget::setInfoPage()
{
	ui->uid->setText(m_uid);
	ui->name->setText(infoWork->getName());
	ui->department->setText(infoWork->getDepartment());
	ui->group->setText(infoWork->getGroup());
	if(infoWork->getTel().isEmpty())
		ui->phone->setText("--");
    else
		ui->phone->setText(infoWork->getTel());
	if(infoWork->getMail().isEmpty())
		ui->mail->setText("--");
	else
		ui->mail->setText(infoWork->getMail());
	ui->verifyType->setText(infoWork->getVerifyType());
	ui->verifyInfo->setText(infoWork->getVerifyInfo());
    if(infoWork->getAvatar().isNull())
		ui->avatar->setPixmap(QPixmap(":/images/color_icon/user.svg"));
    else
		ui->avatar->setPixmap(infoWork->getAvatar());
    
    int verifyTag = infoWork->getVerifyTag();
    if (verifyTag == 1)
        ui->verifyIcon->setPixmap(QPixmap(":/images/color_icon/verify.svg"));
    else if (verifyTag == 2)
        ui->verifyIcon->setPixmap(QPixmap(":/images/color_icon/verify_2.svg"));

    if (verifyTag == -1)
    {
        ui->verifyIcon->setPixmap(QPixmap(":/images/color_icon/color-delete.svg"));
        ui->verifyType->setText("������֤");
        ui->verifyInfo->setText("--");
    }
    ui->btn_share->setEnabled(true);
}

void FriendInfoWidget::setUid(const QString& uid)
{
	m_uid = uid;
    infoWork->setUid(m_uid);
    setLoading();
    emit getCurrentMemInfo();
}

void FriendInfoWidget::on_btn_share_clicked()
{
	QString text = QString("WePlanet �û���Ƭ��\nUID��%1\n������%2\n���%3\n���ţ�%4\n\n��Դ��WePlanet �û�����").arg(m_uid, infoWork->getName(), infoWork->getGroup(), infoWork->getDepartment());
    clipboard->setText(text);
	QMessageBox::information(this, "WePlanet �û�����", "�ѽ��û����Ϸ��������а塣", QMessageBox::Ok);
}

QString FriendInfoWidget::getUid()
{
	return m_uid;
}


FriendInfoWidget::~FriendInfoWidget()
{
    //�ڴ˴��ȴ������߳�ֹͣ
    if (thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }
    delete infoWork;
    delete thread;
	delete ui;
}
