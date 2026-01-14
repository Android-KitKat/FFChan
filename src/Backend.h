#pragma once

#include <QObject>

namespace FFChan {
class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
public:
    explicit Backend(QObject *parent = nullptr);
    QString message() const;
    Q_INVOKABLE void setMessage(const QString &msg);

signals:
    void messageChanged();

private:
    QString m_message;
};

};
