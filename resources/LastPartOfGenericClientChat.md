Fair point: only the methods would be templated, so Q_OBJECT isn't a real obstacle. I'll carry on with views. First I'll re-read the files you've edited since my last pass, starting with the view and mirror headers.

The mirror is unchanged, and `IRadioSettingsView` now carries your `traits()`. Next, the current widget code that the band-level views have to serve.

`QtMiniVfoToolbar` keeps the band pointer (`m_bandSettings`) and uses it later from button handlers, so band views given to widgets must outlive the notification. I'll read those parts closely.

Now `IRadioSettingsView`, slimmed down. It keeps your `traits()` and `state()`, drops the `splitBands()` facet, and turns the focus queries into non-virtual helpers over `activeBands()`.

Only a comment line differs there (you removed the "Differences between flavours" line, and I've kept it out), so I'll write the new version over it.

Next, the client uses the mirror's persistent view as the state. The changes view stays a temporary.

Converting `QtFrequencyPanel`: it uses the view types throughout and guards `m_band2Readout`, which your traits change can now leave null.

Those two `.cpp` files use `RxPipelineSettings` members. They used to get its definition through the concrete band header, so they now need it included directly.

Now `StandardFace`. `hasFocusBand()`, `pipeline()` and `isMultiPipeline()` have moved off the radio view, and the panel needs its traits. Reading the current code first.

I'll make the frequency panel lazy: it's created from `traits()` on the first settings and initialised from the full state. `updatePanadapter` will take the focus band view.

Cleaning up the leftover nested braces from that rewrite.

MainWindow next: it moves to the renamed helpers (`getFocusBand`, `getFocusMode`, `getCentreFrequency`), which now match the concrete settings API.

Searching for leftover references to the old API, then compiling everything touched, including the three readout widgets.

All eight files compile, and `QtRadioClient.cpp` instantiates the adapters for all four settings types. Five Q_OBJECT headers changed, so I'll run moc on each and compile the output too.

The band-level views are in place, and no `IRadioSettingsView` header refers to a concrete settings type any more. The five UI widgets I converted are now written once for every radio type, with `traits()` deciding what to display. All eight touched files compile, and `QtRadioClient.cpp` builds the views for all four settings types. The moc output for the six changed Q_OBJECT headers also compiles. The full target still can't link because of the back-end errors.

**The view layers** (in `src/settings/model/radio/base/include/settings/model/radio/`)
- **`IRadioSettingsView`**: now just `traits()`, `state()`, `hasActiveBands()`/`activeBands()` and `hasPtt()`/`ptt()`. The focus helpers are non-virtual and named after the concrete methods: `getFocusBand()`, `getFocusMode()`, `getCentreFrequency()`, plus `focusBandSettings()`, `focusBandRf()` and `focusPipeline()`. `splitBands()` is gone.
- **`IActiveBandSettingsView`**: the split-shaped API. A non-split radio has only band One, which always has focus; its TX band is One if the radio transmits, otherwise None.
    - The `has*()` methods report what changed.
    - `focusBandId()`, `txBandId()`, `rxBandId()` and `isSplit()` always return the effective value, from the state when the delta doesn't carry it. So the panel no longer needs to fetch those from the state itself.
- **`IBandSettingsView`**: the band API the widgets use.
- **The three `…ViewT` adapters**: forward to your `*Defaults` API. `requires` covers the gaps: bands without TX, the split selector ids, and single-pipeline bands, which report pipeline A and focus id A.

**Lifetime**
- The state view comes from the mirror, so it and its band views last as long as the mirror. That matters because `QtMiniVfoToolbar` keeps its band pointer for its button handlers. It now points at the state's band rather than a delta, so it's current whenever a button is pressed.
- The changes view is still a temporary that only lasts for the signal emission.

**Widgets**
- **`QtFrequencyPanel`**: uses the views, keeps your traits-based layout, and guards every use of `m_band2Readout` where it can be null.
- **`QtBandReadout`, `QtVfoReadout`, `QtMiniVfoToolbar`**: now take `const IBandSettingsView*`. `getIsMuted` null-checks the pipeline, since band B doesn't exist on single-pipeline bands.
- **`StandardFace`**: creates the frequency panel on the first settings, because the face is initialised before the traits are known. It initialises the panel from the full state, then applies changes as usual. It no longer calls `hasFocusBand()`.

**Worth knowing**
- If capabilities arrive again, a new mirror replaces the old one. The toolbar's kept pointers would then dangle, and the panel isn't rebuilt for the new traits. Both need a "radio changed" reset if that can happen.
- `SingleIqBandSettings` has no `txPipelineId`, so a single-IQ RX/TX radio never shows the TX indicator on its VFO.
- Nothing in the back end changed. The `has_band_1` bug in `SplitBandDualIqActiveBandSettings::hasFocusBand()` is still there; the views just don't use it. The `*Defaults` additions I suggested earlier aren't needed now.