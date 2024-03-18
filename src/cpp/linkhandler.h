// En un nuevo archivo header, por ejemplo, linkhandler.h

#ifndef LINKHANDLER_H
#define LINKHANDLER_H

#include <QObject>
#include <QUrl>

class LinkHandler : public QObject
{
    Q_OBJECT
public:
    explicit LinkHandler(QObject *parent = nullptr);

signals:
    void openUrlInNewTab(const QUrl &url);
    void openUrlInNewWindow(const QUrl &url);

public slots:
    void handleUrl(const QUrl &url);
};

#endif // LINKHANDLER_H
