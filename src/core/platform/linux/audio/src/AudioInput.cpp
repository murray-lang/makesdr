#include "audio/AudioInput.h"


AudioInput::AudioInput()
  : m_thread(*this)
  , m_running(false)
  , m_pSink(nullptr)
  , m_maxPacketFrames(0)
  , m_numCurrentFrames(0)
{
}

AudioInput::AudioInput(AudioSink* pSink)
  : m_thread(*this)
  , m_running(false)
  , m_pSink(pSink)
  , m_maxPacketFrames(0)
  , m_numCurrentFrames(0)
{

}

AudioInput::AudioInput(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format, AudioSink* pSink)
  : AudioInputBase(format, pSink)
  , RtAudioDriver(api, deviceInfo)
  , m_thread(*this)
  , m_running(false)
  , m_pSink(pSink)
  , m_maxPacketFrames(0)
  , m_numCurrentFrames(0)
{
  // deviceId is left for start() to fill in via resolveDeviceId().
  m_params.nChannels = format.channelCount;
  m_params.firstChannel = 0;
}

// AudioInput::AudioInput(AudioInput&& other) noexcept
//   : AudioInputBase(std::move(other))
//   , RtAudioDriver(std::move(other))
//   , m_thread(*this)
//   , m_running(false)
//   , m_params(other.m_params)
//   , m_pSink(other.m_pSink)
//   , m_maxPacketFrames(other.m_maxPacketFrames)
//   , m_numCurrentFrames(0)
// {
// }

AudioInput::~AudioInput()
{
  AudioInput::stop();
  // m_thread.join();
}

// AudioInput& AudioInput::operator=(AudioInput&& other) noexcept
// {
//   AudioInputBase::operator=(std::move(other));
//   RtAudioDriver::operator=(std::move(other));
//   m_running = false;
//   m_params = other.m_params;
//   m_pSink = other.m_pSink;
//   m_maxPacketFrames = other.m_maxPacketFrames;
//   m_numCurrentFrames = 0;
//   return *this;
// }

void AudioInput::configure(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format, AudioSink* pSink)
{
  stop();
  bindApi(api, deviceInfo);
  m_format             = format;
  m_pSink              = pSink;
  // deviceId is left for start() to fill in via resolveDeviceId().
  m_params.nChannels   = format.channelCount;
  m_params.firstChannel = 0;
}

ResultCode
AudioInput::start(uint32_t maxPacketFrames)
{
  m_maxPacketFrames = maxPacketFrames;
  if (!m_running) {
    // unsigned int bufferFrames = DEFAULT_BUFFER_SIZE;

    if (!isApiBound()) {
      return ResultCode::ERR_AUDIO_DRIVER_NOT_CONFIGURED;
    }

    // Close any existing stream before opening a new one
    if (m_rtAudio->isStreamOpen()) {
      m_rtAudio->closeStream();
    }

    if (!resolveDeviceId(m_params.deviceId)) {
      return ResultCode::ERR_AUDIO_NO_MATCHING_INPUT_DEVICE;
    }

    RtAudioErrorType rc = m_rtAudio->openStream(
      nullptr, // no output
      &m_params, // input params
      static_cast<RtAudioFormat>(m_format.sampleFormat), // sample format
      m_format.sampleRate,
      &m_maxPacketFrames,
      &rtCallback,
      this
    );
    if (rc != RTAUDIO_NO_ERROR) {
      return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
    }
    // openStream takes maxPacketFrames as in/out and may grant a larger buffer
    // than requested. m_outputBuffer holds channelCount interleaved floats per
    // frame, so a granted packet has to fit within its capacity.
    if (m_maxPacketFrames * m_format.channelCount > m_outputBuffer.max_size()) {
      m_rtAudio->closeStream();
      return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
    }
    m_outputBuffer.resize(m_maxPacketFrames * m_format.channelCount);
    m_maxPacketFrames = std::min(m_maxPacketFrames, static_cast<uint32_t>(PIPELINE_BUFFER_LENGTH/2));

    rc = m_rtAudio->startStream();
    if (rc != RTAUDIO_NO_ERROR) {
      m_rtAudio->closeStream();
      return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
    }
    m_running = true;
    m_thread.start();
    return ResultCode::OK;
  }
  return ResultCode::ERR_AUDIO_INPUT_DRIVER_ALREADY_STARTED;
}

void
AudioInput::stop()
{
  if (m_running) {
    m_running = false;
    if (m_rtAudio->isStreamOpen()) {
      m_rtAudio->stopStream();
      m_rtAudio->closeStream();
    }
    m_dataAvailable.wakeOne();
    m_thread.join();
  }
}

int
AudioInput::rtCallback(void*, void* inputBuffer, unsigned int nframes, double,
                             RtAudioStreamStatus, void* userData)
{
  // qDebug() << "rtCallback(): " << nframes << " frames.";
  return static_cast<AudioInput*>(userData)->handleCallback(inputBuffer, nframes);
}


int
AudioInput::handleCallback(void* inputBuffer, unsigned int nframes)
{
  if (inputBuffer) {
    auto* in = static_cast<float*>(inputBuffer);
    // lock_guard<mutex> lock(m_mutex);
    LockGuard locker(m_mutex);
    size_t queueSize = m_queue.size();
    if (queueSize / m_format.channelCount + nframes > PIPELINE_BUFFER_LENGTH) {
      // Option 1: Drop new data (simplest)
      // qDebug() << "RtAudioInputDriver: Queue overflow, dropping samples";
      return 0;

      /*
      // Option 2: Clear old data to make room (better for maintaining "fresh" audio)
      size_t framesToRemove = (m_queue.size() / m_format.channelCount + nframes) - MAX_QUEUE_FRAMES;
      m_queue.erase(m_queue.begin(), m_queue.begin() + (framesToRemove * m_format.channelCount));
      */
    }
    // Append nframes * m_channels samples
    m_queue.insert(m_queue.end(), in, in + nframes * m_format.channelCount);
    queueSize = m_queue.size();
    m_dataAvailable.wakeOne();
  }
  return 0;
}

void
AudioInput::run()
{
  while (m_running) {
    {
      LockGuard locker(m_mutex);
      if (m_queue.empty()) {
        m_dataAvailable.wait(&m_mutex);
      }
      if (!m_running) {
        break;
      }
      if (!m_queue.empty()) {
        uint32_t requiredFrames = m_maxPacketFrames - m_numCurrentFrames;
        uint32_t numIncomingFrames = m_queue.size() / m_format.channelCount;
        uint32_t framesToRead = std::min(requiredFrames, numIncomingFrames);
        getSamplesFromBuffer(framesToRead, m_format.channelCount, m_outputBuffer);
        m_queue.erase(m_queue.begin(), m_queue.begin() + framesToRead * m_format.channelCount);
        m_numCurrentFrames += framesToRead;
      }
    }
    if (m_numCurrentFrames == m_maxPacketFrames) {
      // qDebug() << "AudioInputDevice::run(): " << m_numCurrentFrames << " frames.";
      m_pSink->sinkAudio(
        m_outputBuffer,
        static_cast<uint32_t>(m_numCurrentFrames) * m_format.channelCount,
        m_format.channelCount
      );
      m_numCurrentFrames = 0;
    }
  }
}

void AudioInput::getSamplesFromBuffer(size_t numFrames, uint32_t channelCount, RealSamplesBuffer& input)
{
  // A callback can deliver fewer than m_maxPacketFrames, so a packet is filled
  // over several passes. Append at the frames already held, or each pass
  // overwrites the previous one from index 0.
  for (size_t i = 0; i < numFrames; i++) {
    for (size_t j = 0; j < channelCount; j++) {
      input.at((m_numCurrentFrames + i) * channelCount + j) = m_queue.at(i * channelCount + j);
    }
  }
}
