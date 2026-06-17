#include "settings/control/websocket/WebSocketControlSink.h"
#include "settings/model/proto/ProtobufIo.h"
#include "settings/model/proto/RadioSettings.pb.h"
#include <cstring>
#include <algorithm>

WebSocketControlSink::WebSocketControlSink(int port)
  : SettingsControlSink()
  , m_port(port)
  , m_context(nullptr)
  , m_running(false)
{
}

WebSocketControlSink::~WebSocketControlSink()
{
  stop();
}

ResultCode WebSocketControlSink::start()
{
  if (m_running) {
    return ResultCode::OK;
  }

  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));

  static struct lws_protocols protocols[] = {
    {
      "settings-protocol",
      websocketCallback,
      0,
      MAX_MESSAGE_SIZE,
      0,
      this,
      0
    },
    { nullptr, nullptr, 0, 0, 0, nullptr, 0 }
  };

  info.port = m_port;
  info.protocols = protocols;
  info.gid = -1;
  info.uid = -1;
  info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

  m_context = lws_create_context(&info);
  if (!m_context) {
    return ResultCode::ERR_SETTING_WEBSOCKET_INIT_FAILED;
  }

  m_running = true;
  return ResultCode::OK;
}

void WebSocketControlSink::stop()
{
  if (!m_running) {
    return;
  }

  m_running = false;

  {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    m_clients.clear();
  }

  if (m_context) {
    lws_context_destroy(m_context);
    m_context = nullptr;
  }
}

void WebSocketControlSink::service(int timeout_ms)
{
  if (m_running && m_context) {
    lws_service(m_context, timeout_ms);
  }
}

ResultCode WebSocketControlSink::applySettings(const RadioSettings& settings)
{
  // Serialize the settings to protobuf
  // size_t bytes_written = 0;
  // ResultCode result = ProtobufIo::writeProtobuf(
  //   m_txBuffer + LWS_PRE,
  //   MAX_MESSAGE_SIZE,
  //   &bytes_written,
  //   makesdr_fields,
  //   settings
  // );
  return ResultCode::ERR_SETTING_WEBSOCKET_INIT_FAILED; // For now
  // if (result != ResultCode::OK) {
  //   return result;
  // }
  //
  // return broadcastMessage(m_txBuffer + LWS_PRE, bytes_written);
}

// ResultCode WebSocketControlSink::applySettingUpdate(const SettingUpdate& update)
// {
//   // Serialize the field update to protobuf
//   size_t bytes_written = 0;
//   ResultCode result = ProtobufIo::writeProtobuf(
//     m_txBuffer + LWS_PRE,
//     MAX_MESSAGE_SIZE,
//     &bytes_written,
//     SettingUpdate_fields,
//     update
//   );
//
//   if (result != ResultCode::OK) {
//     return result;
//   }
//
//   return broadcastMessage(m_txBuffer + LWS_PRE, bytes_written);
// }

ResultCode WebSocketControlSink::broadcastMessage(const uint8_t* data, size_t len)
{
  if (len > MAX_MESSAGE_SIZE) {
    return ResultCode::ERR_SETTING_WEBSOCKET_MESSAGE_TOO_LARGE;
  }

  std::lock_guard<std::mutex> lock(m_clientsMutex);

  if (m_clients.empty()) {
    return ResultCode::OK;
  }

  // Store broadcast data
  {
    std::lock_guard<std::mutex> bLock(m_broadcastMutex);
    m_broadcastBuffer.assign(data, data + len);
  }

  // Request writable callback for all clients
  for (auto* client : m_clients) {
    lws_callback_on_writable(client);
  }

  return ResultCode::OK;
}

int WebSocketControlSink::websocketCallback(
  struct lws *wsi,
  enum lws_callback_reasons reason,
  void *user,
  void *in,
  size_t len
)
{
  auto* protocols = lws_get_protocol(wsi);
  if (!protocols || !protocols->user) {
    return 0;
  }

  auto* self = static_cast<WebSocketControlSink*>(protocols->user);

  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
      self->handleEstablished(wsi);
      break;

    case LWS_CALLBACK_CLOSED:
      self->handleClosed(wsi);
      break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
      self->handleWritable(wsi);
      break;

    default:
      break;
  }

  return 0;
}

void WebSocketControlSink::handleEstablished(struct lws *wsi)
{
  std::lock_guard<std::mutex> lock(m_clientsMutex);
  m_clients.push_back(wsi);
}

void WebSocketControlSink::handleClosed(struct lws *wsi)
{
  std::lock_guard<std::mutex> lock(m_clientsMutex);
  auto it = std::find(m_clients.begin(), m_clients.end(), wsi);
  if (it != m_clients.end()) {
    m_clients.erase(it);
  }
}

void WebSocketControlSink::handleWritable(struct lws *wsi)
{
  std::lock_guard<std::mutex> lock(m_broadcastMutex);

  if (m_broadcastBuffer.empty()) {
    return;
  }

  // Copy broadcast data to tx buffer with LWS_PRE padding
  memcpy(m_txBuffer + LWS_PRE, m_broadcastBuffer.data(), m_broadcastBuffer.size());

  lws_write(
    wsi,
    m_txBuffer + LWS_PRE,
    m_broadcastBuffer.size(),
    LWS_WRITE_BINARY
  );
}
