#pragma once
#ifndef DDMONITOR_VIDEOCACHE_HPP
#define DDMONITOR_VIDEOCACHE_HPP

class VideoCache : public QObject {
  Q_OBJECT
 public:
  enum Quality { Original = 10000, BlueRay = 400, HD = 250, Normal = 80 };
  enum Mode { Cache, Record };
  VideoCache(int roomId, const QDir& dir, Quality quality = BlueRay,
             Mode mode = Cache);
  ~VideoCache() override;

  Q_PROPERTY(QString filePath READ filePath NOTIFY FilePathChanged)
  QString filePath() const;
  Q_SIGNAL void FilePathChanged(const QString& filePath);

 private:
  VideoCache(const VideoCache&) = delete;
  VideoCache& operator=(const VideoCache&) = delete;

  int roomId_;
  QDir dir_;
  Quality quality_;
  Mode mode_;

  QThread thread_{};
  mutable QRecursiveMutex mutex_{};

  QNetworkAccessManager* manager_;

  Q_SLOT void OnMetaReadyRead();
  Q_SLOT void OnMetaFinished();
  Q_SLOT void OnMetaError(QNetworkReply::NetworkError error);
  Q_SLOT void OnMetaSslErrors(const QList<QSslError>& errors);
  QUrl metaUrl_{};
  QNetworkReply* metaReply_ = nullptr;
  QByteArray metaData_{};

  void ReserveFile();
  Q_SLOT void OnVideoReadyRead();
  Q_SLOT void OnVideoFinished();
  Q_SLOT void OnVideoError(QNetworkReply::NetworkError error);
  Q_SLOT void OnVideoSslErrors(const QList<QSslError>& errors);
  qint64 maxCacheSize_ = 500 * 1024 * 1024;
  QUrl videoUrl_{};
  QNetworkReply* videoReply_ = nullptr;
  QFile* oldFile_ = nullptr;
  QFile* file_ = nullptr;
  QString filePath_;
};

#endif  // DDMONITOR_VIDEOCACHE_HPP
