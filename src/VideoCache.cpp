#include "VideoCache.hpp"

Q_GLOBAL_STATIC_WITH_ARGS(
    QByteArray, kUserAgent,
    ("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 "
     "(KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"))

VideoCache::VideoCache(int roomId, const QDir& dir, Quality quality, Mode mode)
    : roomId_(roomId),
      dir_(dir),
      quality_(quality),
      mode_(mode),
      manager_(new QNetworkAccessManager) {
  manager_->moveToThread(&thread_);
  connect(&thread_, &QThread::finished, manager_, &QObject::deleteLater);

  connect(&thread_, &QThread::started, manager_, [this] {
    QNetworkRequest request;
    metaUrl_ = QUrl(QStringLiteral("https://api.live.bilibili.com/room/v1/Room/"
                                   "playUrl?cid=%1&platform=web&qn=%2")
                        .arg(roomId_)
                        .arg(quality_));
    qDebug() << metaUrl_.isValid() << metaUrl_;
    request.setUrl(metaUrl_);
    // request.setRawHeader("User-Agent", *kUserAgent);

    metaReply_ = manager_->get(request);
    connect(metaReply_, &QIODevice::readyRead, metaReply_,
            [this] { OnMetaReadyRead(); });
    connect(metaReply_, &QNetworkReply::finished, metaReply_,
            [this] { OnMetaFinished(); });
    connect(metaReply_, &QNetworkReply::errorOccurred, metaReply_,
            [this](auto error) { OnMetaError(error); });
    connect(metaReply_, &QNetworkReply::sslErrors, metaReply_,
            [this](const auto& errors) { OnMetaSslErrors(errors); });
  });

  thread_.start();
}

VideoCache::~VideoCache() {
  thread_.quit();
  thread_.wait();
}

QString VideoCache::filePath() const {
  QMutexLocker locker(&mutex_);
  return filePath_;
}

void VideoCache::OnMetaReadyRead() { metaData_ += metaReply_->readAll(); }

void VideoCache::OnMetaFinished() {
  OnMetaReadyRead();

  QJsonParseError error;
  auto doc = QJsonDocument::fromJson(metaData_, &error);
  if (error.error != QJsonParseError::NoError) {
    // TODO
    return;
  }
  ReserveFile();

  videoUrl_ = QUrl(doc.object()
                       .value("data")
                       .toObject()
                       .value("durl")
                       .toArray()
                       .at(0)
                       .toObject()
                       .value("url")
                       .toString());
  QNetworkRequest request;
  request.setUrl(videoUrl_);
  request.setRawHeader("User-Agent", *kUserAgent);

  videoReply_ = manager_->get(request);
  videoReply_->setReadBufferSize(
      std::max(videoReply_->readBufferSize(), qint64(1024 * 1024)));
  connect(videoReply_, &QIODevice::readyRead, videoReply_,
          [this] { OnVideoReadyRead(); });
  connect(videoReply_, &QNetworkReply::finished, videoReply_,
          [this] { OnVideoFinished(); });
  connect(videoReply_, &QNetworkReply::errorOccurred, videoReply_,
          [this](auto error) { OnVideoError(error); });
  connect(videoReply_, &QNetworkReply::sslErrors, videoReply_,
          [this](const auto& errors) { OnVideoSslErrors(errors); });
}

void VideoCache::OnMetaError(QNetworkReply::NetworkError error) {
  qWarning() << error;
  // TODO
}

void VideoCache::OnMetaSslErrors(const QList<QSslError>& errors) {
  for (auto&& error : errors) {
    qWarning().noquote() << error.errorString();
  }
  // TODO
}

void VideoCache::ReserveFile() {
  if (file_) {
    if (file_->size() <= maxCacheSize_) return;
    if (oldFile_) std::exchange(oldFile_, nullptr)->deleteLater();
    std::swap(file_, oldFile_);
  }

  dir_.mkpath(dir_.absolutePath());
  QString fileName = dir_.absoluteFilePath(
      QStringLiteral("%1_%2.flv")
          .arg(roomId_)
          .arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
  if (mode_ == Cache) {
    file_ = new QTemporaryFile(fileName);
  } else {
    file_ = new QFile(fileName);
  }
  if (!file_->open(QFile::WriteOnly | QFile::Truncate)) {
    qWarning();  // TODO
    return;
  }

  QMutexLocker locker(&mutex_);
  filePath_ = file_->fileName();
  emit FilePathChanged(filePath_);
}

void VideoCache::OnVideoReadyRead() {
  ReserveFile();
  if (file_) file_->write(videoReply_->readAll());
}

void VideoCache::OnVideoFinished() { OnVideoReadyRead(); }

void VideoCache::OnVideoError(QNetworkReply::NetworkError error) {
  qWarning() << error;
  // TODO
}

void VideoCache::OnVideoSslErrors(const QList<QSslError>& errors) {
  for (auto&& error : errors) {
    qWarning().noquote() << error.errorString();
  }
  // TODO
}
