
#include <format>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../radio/RadioSettings.h"
#include "MessageTagLookup.h"

struct ResolvedPath {
  std::string dottedPath;
  std::vector<uint32_t> tags;
};

static void generatePaths(
    const FieldEntry* table,
    const std::string& prefix,
    const std::vector<uint32_t>& currentTags,
    std::vector<ResolvedPath>& paths)
{
  if (table == nullptr) return;

  const FieldEntry* entry = table;
  while (entry->name != nullptr) {
    std::string currentPath = prefix.empty()
        ? std::string(entry->name)
        : prefix + "." + entry->name;

    // Add current tag to path
    std::vector<uint32_t> tags = currentTags;
    tags.push_back(entry->tag);

    // Store this resolved path
    paths.push_back({currentPath, tags});

    // Recurse into submessages
    if (entry->submsg != nullptr) {
      generatePaths(entry->submsg, currentPath, tags, paths);
    }

    entry++;
  }
}

void generateResolvedPathSourceFiles(const FieldEntry* fieldEntries, const char* baseName)
{
  std::vector<ResolvedPath> allPaths;

    // Generate all dotted paths with their resolved tags
    std::vector<uint32_t> emptyTags;
    generatePaths(fieldEntries, "", emptyTags, allPaths);

    // Output files
    std::string headerFile = std::format("{}.h", baseName);
    std::string sourceFile = std::format("{}.cpp", baseName);


    // Generate header file
    std::ofstream hFile(headerFile);
    hFile << "#pragma once\n";
    hFile << "#include \"../base/SettingFieldPath.h\"\n\n";
    hFile << "// Auto-generated pre-resolved field paths\n";
    hFile << "// Uncomment the ones you need, likewise in " << sourceFile << "\n";
    // hFile << "namespace FieldPaths {\n\n";

    for (const auto& path : allPaths) {
        // Convert dotted path to valid C++ identifier
        std::string identifier = path.dottedPath;
        for (char& c : identifier) {
            if (c == '.') c = '_';
        }

        hFile << "extern const SettingFieldPath " << identifier << ";\n";
    }

    // hFile << "\n} // namespace FieldPaths\n";
    hFile.close();

    // Generate source file
    std::ofstream cppFile(sourceFile);
    cppFile << "#include \"" << headerFile << "\"\n\n";
    cppFile << "// Pre-resolved field paths (no string processing at runtime)\n";
    cppFile << "// Uncomment the ones you need, likewise in " << headerFile << "\n";
    // cppFile << "namespace FieldPaths {\n\n";

    for (const auto& path : allPaths) {
        // Convert dotted path to valid C++ identifier
        std::string identifier = path.dottedPath;
        for (char& c : identifier) {
            if (c == '.') c = '_';
        }

        // Generate the tag array directly
        cppFile << "const SettingFieldPath " << identifier << " = {";
        for (size_t i = 0; i < path.tags.size(); ++i) {
            if (i > 0) cppFile << ", ";
            cppFile << path.tags[i];
        }
        cppFile << "};\n";
    }

    // cppFile << "\n} // namespace FieldPaths\n";
    cppFile.close();

    std::cout << "Generated " << allPaths.size() << " pre-resolved field paths:\n";
    std::cout << "  " << headerFile << "\n";
    std::cout << "  " << sourceFile << "\n";
}