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

  std::string settingFilePath(homeDir);
  settingFilePath += "/.config/nexusdr/nexusdr-settings.binpb";

  std::ifstream inFile(settingFilePath, std::ios::binary | std::ios::ate);
  if (!inFile) {
    std::cerr << "Failed to open input file" << std::endl;
    return 1;
  }

  std::streamsize fileSize = inFile.tellg();
  inFile.seekg(0, std::ios::beg);

  uint8_t buffer[RadioSettings_RadioSettingsPb_size];
  if (!inFile.read(reinterpret_cast<char*>(buffer), fileSize)) {
    std::cerr << "Failed to read file" << std::endl;
    return 1;
  }
  inFile.close();

  RadioSettings_RadioSettingsPb restoredRadioSettingsPb;

  ResultCode rc = ProtobufIo::readProtobuf<RadioSettings_RadioSettingsPb>(
    buffer,
    fileSize,
    &RadioSettings_RadioSettingsPb_msg,
    RadioSettings_RadioSettingsPb_init_zero,
    restoredRadioSettingsPb
    );

  if (rc != ResultCode::OK) {
    std::cerr << "Failed to restore radio settings " << (uint32_t)rc << std::endl;
    return 1;
  }


  return 0;
}