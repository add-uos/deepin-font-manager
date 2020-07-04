#ifndef DFONTPREVIEWLISTDATATHREAD_H
#define DFONTPREVIEWLISTDATATHREAD_H

#include "dfmdbmanager.h"
#include "dfontinfomanager.h"

#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <QMetaType>

class QFileSystemWatcher;
class DFontPreviewListView;
class DFontPreviewListDataThread : public QObject
{
    Q_OBJECT
public:
    static DFontPreviewListDataThread *instance(DFontPreviewListView *view);
    static DFontPreviewListDataThread *instance();

    DFontPreviewListDataThread(DFontPreviewListView *view);
    virtual ~DFontPreviewListDataThread();

    //从fontconfig配置文件同步字体启用/禁用状态数据
    void syncFontEnableDisableStatusData(const QStringList &disableFontPathList);
    void refreshFontListData(bool isStartup = false, const QStringList &list = QStringList());
    void removeFontData(const DFontPreviewItemData &removeItemData);
    void checkStyleName(DFontInfo &f);

    bool checkChineseStyleName(const QString fontFamilyName);

    int insertFontItemData(const QString &filePath,
                           int index,
                           const QStringList &chineseFontPathList,
                           const QStringList &monoSpaceFontPathList,
                           bool isStartup = false, bool isEnabled = true);

    QList<DFontPreviewItemData> getFontModelList();
    QList<DFontPreviewItemData> getDiffFontModelList() const;
    void setMutex(QMutex *mutex);

    void updateItemStatus(int index, DFontPreviewItemData itemData);
    void updateFontId(const DFontPreviewItemData &itemData, int id);
signals:
    void resultReady();
    void requestForceDeleteFiles(const QStringList &files);
    void requestBatchReInstallContinue();
    void requestRemoveFileWatchers(const QStringList &files);
    void requestAutoDirWatchers();

protected slots:
    void doWork();
    void initFileSystemWatcher();

public slots:
    void updateChangedFile(const QString &path);
    void updateChangedDir(const QString &path);
    void addPathWatcher(const QString &path);
    void removePathWatcher(const QString &path);
    void onFileDeleted(const QStringList &files);
    void onFileAdded(const QStringList &files);
    void forceDeleteFiles(const QStringList &files);
    void onRemoveFileWatchers(const QStringList &files);
    void onAutoDirWatchers();
    void setCantDisabledMonoList();

protected:
    QThread mThread;

    DFMDBManager *m_dbManager {nullptr};
    QList<DFontPreviewItemData> m_fontModelList;
    QList<DFontPreviewItemData> m_diffFontModelList;
    DFontPreviewListView *m_view;
    QFileSystemWatcher *m_fsWatcher;
    QStringList cantDisabledMonoList;
    QMutex *m_mutex;
};

#endif // DFONTPREVIEWLISTDATATHREAD_H
