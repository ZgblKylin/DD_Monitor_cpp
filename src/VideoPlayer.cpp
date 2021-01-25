#include "VideoPlayer.hpp"
#include "VideoCache.hpp"

VideoPlayer::VideoPlayer(QWidget* parent) : VlcWidgetVideo(nullptr, parent) {
  auto cache = new VideoCache(115, QDir("cache"), VideoCache::Original);
  connect(cache, &VideoCache::FilePathChanged, this,
          [this](const QString& filePath) {
            auto instance= new VlcInstance(VlcCommon::args(), this);
            auto player = new VlcMediaPlayer(instance);
            player->setVideoWidget(this);
            setMediaPlayer(player);
            player->open(new VlcMedia(filePath, true, instance));
          });
}

VideoPlayer::~VideoPlayer() {}

int VideoPlayer::roomId() const { return roomId_; }

void VideoPlayer::SetRoomId(int id) {
  if (roomId_ == id) return;

  roomId_ = id;
  // TODO
  emit RoomIdChanged(roomId_);
}
