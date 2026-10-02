#pragma once

#include "AfSettings.h"

class ReceiverSettings
{
public:
	using Proto = makesdr_ReceiverSettingsPb;

	ReceiverSettings(Proto& rawSettings)
		: m_rawSettings(rawSettings)
		,  m_afSettings(rawSettings.volume)
	{}

	[[nodiscard]] bool hasVolume() const { return m_rawSettings.has_volume; }
	[[nodiscard]] AfSettings& volume() { return m_afSettings; }
	[[nodiscard]] const AfSettings& volume() const { return m_afSettings; }

protected:
	Proto& m_rawSettings;
	AfSettings m_afSettings;
};