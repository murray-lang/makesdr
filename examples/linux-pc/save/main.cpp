#include <ResultCode.h>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>

// #include "util/GenerateResolvedPathSourceFiles.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/core/ProtobufIo.h"

#include "../../data/exampleRadioSettings.h"


int main()
{
  // const char* homeDir = getpwuid(getuid())->pw_dir;
  const char* homeDir = getenv("HOME");

  uint8_t buffer[RadioSettings_RadioSettingsPb_size];
  size_t written = 0;
  ResultCode rc = ProtobufIo::writeProtobuf<RadioSettings_RadioSettingsPb>(
    buffer,
    RadioSettings_RadioSettingsPb_size,
    &written,
    &RadioSettings_RadioSettingsPb_msg,
    exampleRadioSettingsPb);
  std::string settingFilePath(homeDir);
  if (rc != ResultCode::OK) {
    std::cerr << "Failed to serialize radio settings " << (uint32_t)rc << std::endl;
    return 1;
  }
  settingFilePath += "/.config/nexusdr/nexusdr-settings.binpb";
  std::ofstream outFile(settingFilePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "Failed to open output file" << std::endl;
    return 1;
  }

  outFile.write(reinterpret_cast<const char*>(buffer), written);
  outFile.close();

  if (!outFile) {
    std::cerr << "Error writing to file" << std::endl;
    return 1;
  }

  std::cout << "Written " << written << " bytes to radio_settings.binpb" << std::endl;

  return 0;
}