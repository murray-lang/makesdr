#include "settings/control/websocket/WebSocketControlSource.h"
#include "settings/model/proto/ProtobufIo.h"
#include "settings/model/proto/RadioSettings.pb.h"
#include <cstring>

WebSocketControlSource::WebSocketControlSource(int port)
  : SettingsControlSource()
  , m_port(port)
  , m_context(nullptr)
  , m_running(false)
{
}

WebSocketControlSource::~WebSocketControlSource()
{
  stop();
}

ResultCode WebSocketControlSource::start()
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

void WebSocketControlSource::stop()
{
  if (!m_running) {
    return;
  }

  m_running = false;
  m_clients.clear();

  if (m_context) {
    lws_context_destroy(m_context);
    m_context = nullptr;
  }
}

void WebSocketControlSource::service(int timeout_ms)
{
  if (m_running && m_context) {
    lws_service(m_context, timeout_ms);
  }
}

int WebSocketControlSource::websocketCallback(
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

  auto* self = static_cast<WebSocketControlSource*>(protocols->user);

  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
      self->handleEstablished(wsi);
      break;

    case LWS_CALLBACK_RECEIVE:
      self->handleReceive(wsi, in, len);
      break;

    case LWS_CALLBACK_CLOSED:
      self->handleClosed(wsi);
      break;

    default:
      break;
  }

  return 0;
}

void WebSocketControlSource::handleEstablished(struct lws *wsi)
{
  m_clients.push_back(wsi);
}

void WebSocketControlSource::handleClosed(struct lws *wsi)
{
  auto it = std::find(m_clients.begin(), m_clients.end(), wsi);
  if (it != m_clients.end()) {
    m_clients.erase(it);
  }
}

void WebSocketControlSource::handleReceive(struct lws *wsi, void *data, size_t len)
{
  if (len > MAX_MESSAGE_SIZE) {
    return;
  }

  // Copy received data
  memcpy(m_rxBuffer, data, len);
  processMessage(m_rxBuffer, len);
}

ResultCode WebSocketControlSource::processMessage(const uint8_t* data, size_t len)
{
  /* TODO: Get protobuf exchange scheme sorted out
  // Try to decode as RadioSettings first
  RadioSettings settings = RadioSettings_RadioSettingsPb_init_zero;
  ResultCode result = ProtobufIo::readProtobuf(
    data,
    len,
    RadioSettings_fields,
    RadioSettings_init_zero,
    settings
  );

  if (result == ResultCode::OK) {
    return notifySettings(settings);
  }

  // Try to decode as SettingUpdate
  SettingUpdate update = SettingUpdate_init_zero;
  result = ProtobufIo::readProtobuf(
    data,
    len,
    SettingUpdate_fields,
    SettingUpdate_init_zero,
    update
  );

  if (result == ResultCode::OK) {
    return notifySettingUpdate(update);
  }
  */
  return ResultCode::ERR_SETTING_READING_PROTOBUF;
}
