Let me look at the current state of the client and mirror before answering.

The mirror uses `ListenerT*` and the client passes `this`, so those match. Two quick checks on names the code relies on:

Yes, and it goes further than I suggested. Once the face's handler is a template, it can pass `S`, or the nested types, straight down into the widgets. So I'd **drop my earlier advice to flatten things to leaf types**. `QtFrequencyPanel` can keep walking `ActiveBandSettings` exactly as it does now; it just walks it as a template parameter.

```cpp
// FaceBase (non-template QWidget): one no-op virtual slot per known type
public slots:
  virtual void handleSettings(const SplitBandDualIqRxTxSettings*) {}
  // virtual void handleSettings(const DualIqRxSettings*) {}

// StandardFace
public slots:
  void handleSettings(const SplitBandDualIqRxTxSettings* s) override { applySettings(s); }  // S deduced
private:
  template<typename S> void applySettings(const S* s);

// StandardFace::applySettings<S>
if (s->hasActiveBands()) {
  const typename S::ActiveBandSettings* active = s->activeBands();
  ...
  m_pFrequencyPanel->applyRadioSettings(s);   // widget method is a template too
}
```

Template argument deduction from the pointer means the slots don't even need an explicit `<S>`. Writing it out explicitly is harmless if you prefer it for readability.

**Where the template definitions live.** This is the one practical thing to decide.
- **Face templates:** define them in the face's `.cpp`. They're only instantiated by that face's own slots, so the header stays clean.
- **Widget templates** (e.g. `QtFrequencyPanel::applyRadioSettings<S>`): these are called from *another* translation unit (the face). Either define them in the header, or keep them in the widget's `.cpp` and add explicit instantiations there for the types the widget supports:
  ```cpp
  template void QtFrequencyPanel::applyRadioSettings(const SplitBandDualIqRxTxSettings*, bool);
  ```
  Explicit instantiation keeps the Qt headers light. It also doubles as a declaration of which radio types the widget supports: a missing instantiation shows up as a link error.

**Differences between types inside the templates.** The `ActiveBandSettingsDefaultsT` fallbacks cover most of the split vs non-split differences. Where a band type lacks a member, use `if constexpr (S::Traits::isDualIq)` or a `requires` check, or add the `BandSettingsDefaultsT` fallbacks I mentioned.

**What's left in `QtRadioClientT.h`, as it stands now:**
- **`applySettings<S>`** is still a stub. It needs the wait-loop release and `emitRadioSettingsReceived(&settings)`.
- **`emitRadioSettingsReceived(const RadioSettingsT*)`** (line 340) refers to a type that no longer exists. Replace it with the per-type overloads, `const SplitBandDualIqRxTxSettings*` for now.
- **`applyMessage(CapabilitiesMessage*)`** has no `return` after the `switch`. It also still needs to:
    - set `m_pUpdater`;
    - `start()` the mirror;
    - call `emitCapabilitiesReceived`;
    - release `m_waitingForCapabilities`.

  Otherwise `requestAll()` will always time out.

Should I fill those three in and update `QtRadioClient.h` to match, or would you rather do it while the code is still moving?