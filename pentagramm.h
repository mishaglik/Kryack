#ifndef PENTAGRAMM_H
#define PENTAGRAMM_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QSound>

class Pentagramm : public QWidget
{
    Q_OBJECT
    enum class State{
        Disabled,
        Initializing,
        Ready,
        Working,
    };

public:
    explicit Pentagramm(QWidget *parent = nullptr);

    int getRotation() const;
    void setRotation(int newRotation);

    int getArcLength() const;
    void setArcLength(int newArcLength);

    int getLineProgress() const;
    void setLineProgress(int newLineLength);

protected:
    void paintEvent(QPaintEvent *event) override;
    int rotation = 0;

    int arcLength = 0;
    int lineProgress = 0;
    QPropertyAnimation *animation = nullptr;

private:
    State state  = State::Disabled;
    QSound *sound;

    void drawPentagramm(QPainter &painter);
    void drawCentralLight(QPainter &painter);

    Q_PROPERTY(int rotation     READ getRotation     WRITE setRotation     NOTIFY rotationChanged)
    Q_PROPERTY(int arcLength    READ getArcLength    WRITE setArcLength    NOTIFY arcLengthChanged)
    Q_PROPERTY(int lineProgress READ getLineProgress WRITE setLineProgress NOTIFY lineProgressChanged)

    void initLines();
public slots:
    void init();
    void start();
protected slots:
    void animationFinished();
private slots:
signals:
    void ready();
    void finished();
    void rotationChanged();
    void arcLengthChanged();
    void lineProgressChanged();
};

#endif // PENTAGRAMM_H
