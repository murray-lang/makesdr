Yes — guaranteed by the standard, not just in practice.

[alg.transform] explicitly permits the output to alias an input: *"result may be equal to first1 or first2"* for the binary overload. So `multiplyByCoefficients(buf, buf)` is well-defined, and `Fir.h:57-63` needs no change to support it.

The reason it's safe is that each output element depends only on the inputs at the *same* index. Note that `transform` also does **not** guarantee in-order application of `binary_op` — which is fine here, but means only exact same-index aliasing is safe. A shifted overlap (`result = first1 + k`) is not.

Where this matters for the 32 KiB I flagged: the multiply was never the obstacle — the FFT was. Both your backends happen to tolerate `&input == &output`, for different reasons:

- **liquid** (`liquid/.../Fft.h:44-77`): plans are bound to `m_inputBuffer`/`m_outputBuffer` at construction, so `transform` copies in, executes on its own buffers, then memcpys out. `input` is never read after the copy.
- **STM32** (`stm32h745i/.../Fft.h:24-45`): deliberately copies input→output and runs `arm_cfft_f32` in place on `output`.

So this collapses to:

```cpp
void applyFftCoefficients(const ComplexSamplesFft& input, ComplexSamplesFft& output)
{
  m_fft.transform(input, output, FILTER_FFT_SIZE, true, false);
  multiplyByCoefficients(output, output);
  m_fft.transform(output, output, FILTER_FFT_SIZE, false, true);
}
```

**One real caveat before you do that.** The third line aliases `input` and `output`, and both backends take the `else` branch for the inverse transform:

```cpp
std::memcpy(m_inputBuffer.data(), input.data(), inputLength * sizeof(sdrcomplex));  // liquid:60
memcpy(output.data(), input.data(), inputLength * sizeof(sdrcomplex));             // stm32:31
```

The STM32 one becomes `memcpy(p, p, n)`. That's UB by the letter of the standard — `memcpy`'s parameters are `restrict`-qualified — and GCC/Clang do exploit that assumption. It'll almost certainly work today, but guard it rather than rely on it:

```cpp
if (output.data() != input.data()) {
  memcpy(output.data(), input.data(), inputLength * sizeof(sdrcomplex));
}
```

Also worth noting the liquid `Fft` already carries its own `m_inputBuffer` + `m_outputBuffer` (16 KiB each), and you have two `Fft` instances in that path — `Fir::m_fft` and `FirKernel::m_fft` — so that's 64 KiB of FFT scratch on top of the 32 KiB of stack locals. The kernel's FFT is only used at `configure()` time (`BandPassFirKernel.cpp:59`), so it's 32 KiB resident for something that runs on mode changes.

Two inconsistencies while you're in there: liquid's `normalise` loop runs to `FILTER_FFT_SIZE` (`Fft.h:71`) while STM32's runs to `inputLength` (`Fft.h:48`); and `applyFftCoefficients` resizes its locals to `input.size()` but calls `transform` with `FILTER_FFT_SIZE`, so a short input would have the FFT write past `size()` while `std::transform` only multiplies the first `size()` elements. Currently masked because `FilterStage::initialiseBuffers` assigns exactly `FILTER_FFT_SIZE` — but it's a silent trap if a partial block ever reaches it.