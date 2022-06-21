#include "pentagramm.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QtMath>
#include <QSound>

Pentagramm::Pentagramm(QWidget *parent)
    : QWidget{parent}
{
    setGeometry(0, 0, 400, 400);

    animation = new QPropertyAnimation(this, "", this);
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void Pentagramm::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor::fromRgb(0, 0, 0, 0)));
    painter.translate(200, 200);
    painter.rotate(rotation);
    painter.translate(-200, -200);

    drawPentagramm(painter);
}

int Pentagramm::getLineProgress() const
{
    return lineProgress;
}

void Pentagramm::setLineProgress(int newLineLength)
{
    if (lineProgress == newLineLength)
        return;
    lineProgress = newLineLength;
    update();
    emit lineProgressChanged();
}

int Pentagramm::getArcLength() const
{
    return arcLength;
}

void Pentagramm::setArcLength(int newArcLength)
{
    if (arcLength == newArcLength)
        return;
    arcLength = newArcLength;
    update();
    emit arcLengthChanged();
}

int Pentagramm::getRotation() const
{
    return rotation;
}

void Pentagramm::setRotation(int newRotation)
{
    if (rotation == newRotation)
        return;
    rotation = newRotation;
    update();
    emit rotationChanged();
}

void Pentagramm::drawPentagramm(QPainter &painter)
{
    painter.setPen(QPen(Qt::red, 10));
    painter.drawArc(5, 5, 390, 390, 0, arcLength);
    painter.translate(200, 200);

    if(lineProgress){
        for(int i = 0; i < 5; ++i){
            int x1 = 190 * qCos(qDegreesToRadians((double)(-90 + i * 72)));
            int y1 = 190 * qSin(qDegreesToRadians((double)(-90 + i * 72)));

            int x2 = 190 * qCos(qDegreesToRadians((double)(-90 + (i + 2) * 72))) - x1;
            int y2 = 190 * qSin(qDegreesToRadians((double)(-90 + (i + 2) * 72))) - y1;
            painter.drawLine(x1, y1, x1 + (x2 * lineProgress) / 128, y1 + (y2 * lineProgress) / 128);
        }
    }
    if(rotation){
        drawCentralLight(painter);
    }
}

void Pentagramm::drawCentralLight(QPainter &painter)
{


    painter.setPen(QPen(QBrush(), 0));
    QRadialGradient grad(0, 0, 600 * rotation / (60*360));
    grad.setColorAt(0, QColor::fromRgbF(1, 1, 1, 1));
    grad.setColorAt(1, QColor::fromRgbF(0, 0, 0, ((double)rotation) / (60*360)));
    painter.setBrush(QBrush(grad));
    painter.drawEllipse(-300, -300, 600, 600);
//    painter.drawEllipse(-200 * rotation / (60*360), -200 * rotation / (60*360), 400 * rotation / (60*360), 400 * rotation / (60*360));
//    painter.setPen()
}

void Pentagramm::init()
{
    if(state != State::Disabled) return;
    state = State::Initializing;

    animation->setPropertyName("arcLength");
    animation->setDuration(5000);
    animation->setStartValue(0);
    animation->setEndValue(360 * 16);

    animation->start();
}

void Pentagramm::start()
{
    animation->setPropertyName("rotation");
    animation->setDuration(25000);
    animation->setEasingCurve(QEasingCurve::InSine);
    animation->setStartValue(0);
    animation->setEndValue(60 * 360);

//    QSound::play("/home/gms/Kryack/Moon.wav");
    sound = new QSound("./Moon.wav");
    sound->play();
    animation->start();
}

void Pentagramm::animationFinished()
{
    switch (animation->propertyName().at(0)) {
        case 'a'/*arcLength*/:
            initLines();
        break;
        case 'l'/*line progress*/:
            state = State::Ready;
            emit ready();
        break;
        case 'r'/*rotation*/:
            state = State::Disabled;
            sound->stop();
            emit finished();
        break;
    default:
        Q_ASSERT(0);
    }
}

void Pentagramm::initLines()
{
    Q_ASSERT(state == State::Initializing);

    animation->setPropertyName("lineProgress");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(128);

    QSound::play("./Zombie_89.wav");
    animation->start();

}
