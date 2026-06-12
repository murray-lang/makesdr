#pragma once
#include "settings/control/source/SettingsControlSource.h"
#include <libwebsockets.h>
#include <cstdint>
#include <vector>

class WebSocketControlSource : public SettingsControlSource
{
public:
  WebSocketControlSource(int port = 8080);
  ~WebSocketControlSource() override;

  // Delete copy/move to prevent context invalidation
  WebSocketControlSource(const WebSocketControlSource&) = delete;
  WebSocketControlSource& operator=(const WebSocketControlSource&) = delete;

  ResultCode start();
  void stop();
  void service(int timeout_ms = 50);

  bool isRunning() const { return m_running; }

private:
  static int websocketCallback(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
  );

  void handleReceive(struct lws *wsi, void *data, size_t len);
  void handleEstablished(struct lws *wsi);
  void handleClosed(struct lws *wsi);

  ResultCode processMessage(const uint8_t* data, size_t len);

  int m_port;
  struct lws_context *m_context;
  bool m_running;

  std::vector<struct lws*> m_clients;

  static constexpr size_t MAX_MESSAGE_SIZE = 4096;
  uint8_t m_rxBuffer[MAX_MESSAGE_SIZE];
};
