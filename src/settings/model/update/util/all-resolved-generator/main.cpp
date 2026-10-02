#include <settings/model/update/generateResolvedPathSourceFiles.h>

// Include all tag lookup headers
#include <settings/model/update/SplitBandDualIqTagLookup.h>
#include <settings/model/update/BasicRxTagLookup.h>
#include <settings/model/update/BasicRxTxTagLookup.h>
#include <settings/model/update/BasicIqRxTagLookup.h>
#include <settings/model/update/BasicIqRxTxTagLookup.h>
#include <settings/model/update/DualIqRxTagLookup.h>
#include <settings/model/update/DualIqRxTxTagLookup.h>

#include <filesystem>
#include <iostream>
#include <string>

struct RadioConfig
{
  const char* name;
  const FieldEntry* fieldTable;
  const char* outputBaseName;
  const char* relativePath;
};

int main(int argc, char* argv[])
{
  // Configuration for all radio types
  RadioConfig configs[] = {
    {"Basic Rx", basic_rx_radio_fields, "BasicRxResolved", "radios/basic/rx"},
    {"Basic Rx/Tx", basic_rxtx_radio_fields, "BasicRxTxResolved", "radios/basic/rxtx"},
    {"Basic IQ Rx", basic_iq_rx_radio_fields, "BasicIqRxResolved", "radios/iq/basic/rx"},
    {"Basic IQ Rx/Tx", basic_iq_rxtx_radio_fields, "BasicIqRxTxResolved", "radios/iq/basic/rxtx"},
    {"Dual IQ Rx", dual_iq_rx_radio_fields, "DualIqRxResolved", "radios/iq/dual/rx"},
    {"Dual IQ Rx/Tx", dual_iq_rxtx_radio_fields, "DualIqRxTxResolved", "radios/iq/dual/rxtx"},
    {"Split-band Dual IQ", split_band_dual_iq_radio_fields, "SplitBandDualIqResolved", "radios/iq/dual/split" },
  };

  // Get the source root
  std::filesystem::path sourceRoot;

  if (argc > 1) {
    // Use provided source root path
    sourceRoot = argv[1];
  } else {
#ifdef SOURCE_PATH
    // Use the path compiled in from CMake
    sourceRoot = SOURCE_PATH;
#else
    std::cerr << "Error: No source path provided and SOURCE_PATH not defined.\n";
    std::cerr << "Usage: " << argv[0] << " <source-path>\n";
    std::cerr << "Example: " << argv[0] << " /path/to/src/settings/model/path\n";
    return 1;
#endif
  }

  std::cout << "Generating resolved path source files...\n";
  std::cout << "Source root: " << sourceRoot << "\n\n";

  for (const auto& config : configs) {
    std::cout << "Processing: " << config.name << "\n";

    // Build output directory path
    std::filesystem::path outputDir = sourceRoot / config.relativePath / "all-resolved";
    std::filesystem::path srcDir = outputDir / "src";
    std::filesystem::path incDir = outputDir / "include/settings/model/path";

    // Create directories if they don't exist
    std::filesystem::create_directories(srcDir);
    std::filesystem::create_directories(incDir);

    // Change to src directory for generation
    auto originalPath = std::filesystem::current_path();
    std::filesystem::current_path(srcDir);

    // Generate files
    generateResolvedPathSourceFiles(config.fieldTable, config.outputBaseName);

    // Move header to include directory
    std::string headerName = std::string(config.outputBaseName) + ".h";
    std::filesystem::path headerInSrc = srcDir / headerName;
    std::filesystem::path headerInInc = incDir / headerName;

    if (std::filesystem::exists(headerInSrc)) {
      std::filesystem::rename(headerInSrc, headerInInc);
    }

    // Restore original directory
    std::filesystem::current_path(originalPath);

    std::cout << "  Generated: " << outputDir << "\n\n";
  }

  std::cout << "All files generated successfully!\n";
  return 0;
}
