#include "components.h"

#include <QTimer>
#include <QGraphicsScene>

//Инициализация игрока
CCannon::CCannon(EColor eColor, QGraphicsItem *pParent) : QGraphicsPixmapItem(pParent)
{
    SetColor(eColor);
}

//Инициализация выстрела
void CCannon::Shoot()
{
    CBullet* pBullet = new CBullet(m_eColor);
    connect(pBullet, &CBullet::sigIncreaseScore, this, &CCannon::sigIncreaseScore);

    pBullet->setPos(x() + 27, y() - 10);
    scene()->addItem(pBullet);
}

//Функция возвращает цвет игрока
EColor CCannon::GetColor() const
{
    return m_eColor;
}

//Набор цветов игрока
void CCannon::SetColor(EColor eColor)
{
    m_eColor = eColor;

    switch (eColor)
    {
        case EColor::Red:
        {
            QPixmap oPixmap("Resources/RedCannon.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Pink:
        {
            QPixmap oPixmap("Resources/PinkCannon.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Blue:
        {
            QPixmap oPixmap("Resources/BlueCannon.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
        default:
        {
            QPixmap oPixmap("Resources/RedCannon.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            m_eColor = EColor::Red;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Инициализация врага
CAlien::CAlien(EColor eColor, QGraphicsItem *pParent) : QGraphicsPixmapItem(pParent)
{
    SetColor(eColor);

    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &CAlien::onMove);
    pTimer->start(gAlienSpeed);
}

//Функция возвращает цвет врага
EColor CAlien::GetColor() const
{
    return m_eColor;
}

//Набор цветов врага
void CAlien::SetColor(EColor eColor)
{
    m_eColor = eColor;

    switch (eColor)
    {
        case EColor::Red:
        {
            QPixmap oPixmap("Resources/RedAlien.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Pink:
        {
            QPixmap oPixmap("Resources/PinkAlien.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Blue:
        {
            QPixmap oPixmap("Resources/BlueAlien.png");
            setPixmap(oPixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
            break;
        }
    }
}

//Движение врага
void CAlien::onMove()
{
    setPos(x(), y() + 5);

    if (pos().y() >= (scene()->height() - gCannonSize.height()))
    {
        scene()->removeItem(this);
        emit sigDecreaseHealth();
        delete this;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Инициализация пули
CBullet::CBullet(EColor eColor, QGraphicsItem *pParent) : QGraphicsPixmapItem(pParent)
{
    SetColor(eColor);

    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &CBullet::onMove);
    pTimer->start(gBulletSpeed);
}

//Функция возвращает цвет пули
EColor CBullet::GetColor() const
{
    return m_eColor;
}

//Набор цветов пули
void CBullet::SetColor(EColor eColor)
{
    m_eColor = eColor;

    switch (eColor)
    {
        case EColor::Red:
        {
            QPixmap oPixmap("Resources/RedBullet.png");
            setPixmap(oPixmap.scaled(QSize(35, 35), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Pink:
        {
            QPixmap oPixmap("Resources/PinkBullet.png");
            setPixmap(oPixmap.scaled(QSize(35, 35), Qt::KeepAspectRatio));
            break;
        }
        case EColor::Blue:
        {
            QPixmap oPixmap("Resources/BlueBullet.png");
            setPixmap(oPixmap.scaled(QSize(35, 35), Qt::KeepAspectRatio));
            break;
        }
    }
}

//Движение пули
void CBullet::onMove()
{
    QList<QGraphicsItem*> lstCollidingItem = collidingItems();

    for (auto const pItem : lstCollidingItem)
    {
        CAlien* pAlien = dynamic_cast<CAlien*>(pItem);
        if (pAlien != nullptr)
        {
            if (pAlien->GetColor() == GetColor()) //Если пуля попадает по врагу с тем же цветом, удаляем без присвоения очков.
            {
                scene()->removeItem(pAlien);
                scene()->removeItem(this);


                delete pAlien;
                delete this;
            }
            else //иначе присваиваем очки.
            {
                emit sigIncreaseScore();
                scene()->removeItem(this);
                delete pAlien;
                delete this;
            }
            return;
        }
    }

    setPos(x(), y() - 10);

    if (pos().y() < 0)//Удаление пули
    {
        scene()->removeItem(this);
        delete this;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Инициализация жизни и баллов
CPoints::CPoints(QGraphicsItem *pParent) : QGraphicsTextItem(pParent)
{
    setPlainText(QString("Health : ") + QString::number(m_nHealth) + "\n"
                 + QString("Score : ") + QString::number(m_nScore));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times", 24));
}

//Функция прибавляет баллы
void CPoints::IncreaseScore()
{
    m_nScore += 50;
    setPlainText(QString("Health : ") + QString::number(m_nHealth) + "\n"
                 + QString("Score : ") + QString::number(m_nScore));
}

//возможность обменять 1000 баллов на жизнь
void CPoints::DecreaseScore()
{
    if (m_nScore >= 1000)
    {
        m_nScore -= 1000;
        m_nHealth ++;
        setPlainText(QString("Health : ") + QString::number(m_nHealth) + "\n"
                 + QString("Score : ") + QString::number(m_nScore));
    }
}

//Функция убавляет очки жизни
void CPoints::DecreaseHealth()
{
    m_nHealth --;
    setPlainText(QString("Health : ") + QString::number(m_nHealth) + "\n"
                 + QString("Score : ") + QString::number(m_nScore));
}

//Функция возвращает очки жизни
int CPoints::GetHealth() const
{
    return m_nHealth;
}

//Функция возвращает количество баллов
int CPoints::GetScore() const
{
    return m_nScore;
}
