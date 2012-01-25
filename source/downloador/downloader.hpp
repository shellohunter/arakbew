

#include <QtGui>
#include <QtCore>
#include <QtNetwork>

class DownloadManager: public QObject
{
Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;

public:
    DownloadManager();
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
};



#if 0

Class Downloader : public PThread
{

public:
    int download(string & url, string & saveto, bool sync = false);

    void start();
    void run();
};


hash();

bool checkBuffer(string & url);

int Downloader::download(string & url, string & saveto, bool sync = false)
{
    if(checkBuffer(url))
    {
        cout<<"Already available in the buffer!"<<endl;
        return ;
    }
    else
    {
        start();
    }
}

void run()
{
    libcurl();
    xxxxxxxx
}
#endif
