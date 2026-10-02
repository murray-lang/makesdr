#include "iq/filter/FilterStage.h"
// #include <QDebug>

FilterStage::FilterStage()
  : m_inputCentre(FFT_SIZE / 2)
  , m_inputCursor(0)
{
  initialiseBuffers();
}

void
FilterStage::configure(int32_t freqLoCut, int32_t freqHiCut, int32_t offset, uint32_t sampleRate)
{
  m_fir.configure(freqLoCut, freqHiCut, offset, sampleRate);
}

uint32_t
FilterStage::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  static_assert(PIPELINE_BUFFER_LENGTH >= FFT_SIZE/2);

  uint32_t outPos = 0;
  const ComplexSamplesBuffer& input = buffers.input();
  ComplexSamplesBuffer& output = buffers.output();
  // sdrreal sum = 0.0;
  for (uint32_t inputIndex = 0; inputIndex < inputLength; inputIndex++) {
    const sdrcomplex& nextInput = input[inputIndex];
    // sdrreal mag = std::abs(nextInput);
    // sum += mag;
    m_overlapBuffer.at(m_inputCursor) = nextInput;
    uint32_t fftInputIndex = m_inputCentre + m_inputCursor++;
    m_inputBuffer.at(fftInputIndex) = nextInput;
    if (m_inputCursor == FIR_SIZE - 1) {
      // for (int i = 0; i < FFT_SIZE; i++) {
      //   m_outputBuffer[i] = m_inputBuffer[i];
      // }
      m_fir.filter(m_inputBuffer, m_outputBuffer);
      // sdrreal sum = 0.0;
      for(uint32_t filteredIndex = m_inputCentre; filteredIndex < FFT_SIZE; filteredIndex++) {
        // sdrreal mag = std::abs(m_outputBuffer[filteredIndex]);
        // sum += mag;
        output[outPos++] = m_outputBuffer[filteredIndex];
      }
      // sdrreal avg = sum / static_cast<sdrreal>(inputLength);
      // qDebug() << "Filter out avg: " << avg;
      for (uint32_t overlapIndex = 0; overlapIndex < FIR_SIZE - 1; overlapIndex++) {
        m_inputBuffer.at(overlapIndex) = m_overlapBuffer.at(overlapIndex);
      }
      m_inputCursor = 0;
    }
  }
  // sdrreal avg = sum / static_cast<sdrreal>(inputLength);
  // qDebug() << "Filter in avg: " << avg;
  return outPos;
}

void
FilterStage::initialiseBuffers()
{
  m_overlapBuffer.assign(FIR_SIZE, sdrcomplex(0, 0));
  m_inputBuffer.assign(FFT_SIZE, sdrcomplex(0, 0));
  m_outputBuffer.assign(FFT_SIZE, sdrcomplex(0, 0));
}