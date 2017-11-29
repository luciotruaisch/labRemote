#ifndef REALCHIPTABLE_H
#define REALCHIPTABLE_H

#include <QObject>

class RealChipTable : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    explicit RealChipTable(QObject *parent = nullptr);

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(const QString& data);

    QString source() {return m_source;}

signals:
    void sourceChanged(const QString& source);
    void error(const QString& msg);

public slots:
    void setSource(const QString& source) {m_source = source;}

private:
    QString m_source;
};

#endif // REALCHIPTABLE_H
