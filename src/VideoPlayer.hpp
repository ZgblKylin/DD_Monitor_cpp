#pragma once
#ifndef DDMONITOR_VIDEOPLAYER_HPP
#define DDMONITOR_VIDEOPLAYER_HPP

#include <VLCQtWidgets/WidgetVideo.h>

class VideoPlayer : public VlcWidgetVideo {
  Q_OBJECT
 public:
  explicit VideoPlayer(QWidget* parent = nullptr);
  ~VideoPlayer() override;

  Q_PROPERTY(int roomId READ roomId WRITE SetRoomId NOTIFY RoomIdChanged)
  int roomId() const;
  void SetRoomId(int id);
  Q_SIGNAL void RoomIdChanged(int id);

 private:
  VideoPlayer(const VideoPlayer&) = delete;
  VideoPlayer& operator=(const VideoPlayer&) = delete;

  int roomId_ = 0;
};

#endif  // DDMONITOR_VIDEOPLAYER_HPP
