#pragma once
#include "settings/control/sink/SettingsControlSink.h"
#include <libwebsockets.h>
#include <cstdint>
#include <vector>
#include <mutex>

class WebSocketControlSink : public SettingsControlSink
{
public:
  WebSocketControlSink(int port = 8080);
  ~WebSocketControlSink() override;

  // Delete copy/move to prevent context invalidation
  WebSocketControlSink(const WebSocketControlSink&) = delete;
  WebSocketControlSink& operator=(const WebSocketControlSink&) = delete;

  ResultCode start();
  void stop();
  void service(int timeout_ms = 50);

  bool isRunning() const { return m_running; }

  // Override sink methods to broadcast to connected clients
  ResultCode applySettings(const RadioSettings& settings) override;
  // ResultCode applySettingUpdate(const SettingUpdate& update) override;

private:
  static int websocketCallback(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
  );

  void handleEstablished(struct lws *wsi);
  void handleClosed(struct lws *wsi);
  void handleWritable(struct lws *wsi);

  ResultCode broadcastMessage(const uint8_t* data, size_t len);

  int m_port;
  struct lws_context *m_context;
  bool m_running;

  std::vector<struct lws*> m_clients;
  std::mutex m_clientsMutex;

  // Pending broadcast data
  std::vector<uint8_t> m_broadcastBuffer;
  std::mutex m_broadcastMutex;

  static constexpr size_t MAX_MESSAGE_SIZE = 4096;
  uint8_t m_txBuffer[LWS_PRE + MAX_MESSAGE_SIZE];
};
