#include <iostream>
#include <sstream>

#ifdef PROTO_GOOGLE
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/field_mask_util.h>
#include <google/protobuf/descriptor.h>
#include "proto/RadioSettings.pb.h"
#endif

#ifdef PROTO_NANOPB
#include "RadioSettings.pb.h"
#endif

#ifdef PROTO_GOOGLE
using google::protobuf::FieldMask;
using google::protobuf::util::FieldMaskUtil;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;

using nexusdr::RadioSettings::RadioSettingsPb;

#endif

#ifdef PROTO_NANOPB
using RadioSettingsPb = RadioSettings_RadioSettingsPb;
#endif

#include "util/PbUtils.h"
#include "util/MessageTagLookup.h"

std::string json_data = R"(
  {
  "receiver": {
    "iq_corrections": {
      "amplitude": {
        "value": 0.0,
        "coarse_delta": 0.01,
        "fine_delta": 0.001
      },
      "phase": {
        "value": 0.0,
        "coarse_delta": 0.01,
        "fine_delta": 0.001
      }
    }
  },
  "transmitter": {
    "mic": {
      "gain": {
        "value": 1.0,
        "coarse_delta": 0.01,
        "fine_delta": 0.001
      }
    },
    "iq_corrections": {
      "amplitude": {
        "value": 0.0,
        "coarse_delta": 0.01,
        "fine_delta": 0.001
      },
      "phase": {
        "value": 0.0,
        "coarse_delta": 0.01,
        "fine_delta": 0.001
      }
    }
  },
  "active_bands": {
    "focus_band_id": "SPLIT_BAND_ONE",
    "tx_band_id": "SPLIT_BAND_ONE",
    "rx_band_id": "SPLIT_BAND_ONE",

    "band_1":  {
      "band": {
        "name": "20m",
        "label": "20m",
        "lowest_frequency": 14000000,
        "highest_frequency": 14350000,
        "landing_frequency": 14200000,
        "default_fine_step": 100,
        "default_coarse_step": 1000,
        "default_mode": "MODE_USB"
      },
      "pipeline_a": {
        "rf": {
          "centre_frequency": {
            "value": 14200000,
            "coarse_delta": 1000,
            "fine_delta": 100
          },
          "vfo": {
            "value": 0,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "pipeline_b": {
        "rf": {
          "centre_frequency": {
            "value": 14200000,
            "coarse_delta": 1000,
            "fine_delta": 100
          },
          "vfo": {
            "value": 0,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "tx_pipeline": {
        "rf": {
          "centre_frequency": {
            "value": 14200000,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "is_multi_pipeline": true,
      "focus_pipeline_id": "PIPELINE_A",
      "tx_pipeline_id": "PIPELINE_A"
    },
    "band_2": {
      "band": {
        "name": "40m",
        "label": "40m",
        "lowest_frequency": 7000000,
        "highest_frequency": 7200000,
        "landing_frequency": 7100000,
        "default_fine_step": 100,
        "default_coarse_step": 1000,
        "default_mode": "MODE_LSB"
      },
      "pipeline_a": {
        "rf": {
          "centre_frequency": {
            "value": 7100000,
            "coarse_delta": 1000,
            "fine_delta": 100
          },
          "vfo": {
            "value": 7130000,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "pipeline_b": {
        "rf": {
          "centre_frequency": {
            "value": 7100000,
            "coarse_delta": 1000,
            "fine_delta": 100
          },
          "vfo": {
            "value": 7140000,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "tx_pipeline": {
        "rf": {
          "centre_frequency": {
            "value": 7100000,
            "coarse_delta": 1000,
            "fine_delta": 100
          },
          "vfo": {
            "value": 7130000,
            "coarse_delta": 1000,
            "fine_delta": 100
          }
        }
      },
      "is_multi_pipeline": true,
      "focus_pipeline_id": "PIPELINE_A",
      "tx_pipeline_id": "PIPELINE_A"
    }
  },
  "ptt": false
}
)";
/*
std::vector<int> PathToFieldNumbers(const Descriptor* descriptor, const std::string& path) {
  std::vector<const FieldDescriptor*> field_descriptors;
  std::vector<int> field_numbers;

  // GetFieldDescriptors validates the path and finds all descriptors along it
  if (FieldMaskUtil::GetFieldDescriptors(descriptor, path, &field_descriptors)) {
    for (const auto* field : field_descriptors) {
      field_numbers.push_back(field->number());
    }
  }
  return field_numbers;
}

std::string PathToNumberDotString(const Descriptor* descriptor, const std::string& path) {
  std::vector<const FieldDescriptor*> field_descriptors;
  std::string result;

  // Retrieve the descriptors for each segment of the path
  if (FieldMaskUtil::GetFieldDescriptors(descriptor, path, &field_descriptors)) {
    for (size_t i = 0; i < field_descriptors.size(); ++i) {
      // Append the field number
      result += std::to_string(field_descriptors[i]->number());

      // Add a dot if it's not the last element
      if (i < field_descriptors.size() - 1) {
        result += ".";
      }
    }
  } else {
    return "INVALID_PATH";
  }

  return result;
}

std::string PathToNumbers(const Descriptor* desc, const std::string& path) {
  if (!desc || path.empty()) return "";

  // Split the path into "current segment" and "remaining segments"
  size_t dot_pos = path.find('.');
  std::string segment = path.substr(0, dot_pos);
  std::string remaining = (dot_pos == std::string::npos) ? "" : path.substr(dot_pos + 1);

  // 1. Find the field in the current descriptor
  const FieldDescriptor* field = desc->FindFieldByName(segment);
  if (!field) return "NOT_FOUND:" + segment;

  std::string current_num = std::to_string(field->number());

  // 2. Base case: no more segments to process
  if (remaining.empty()) {
    return current_num;
  }

  // 3. Recursive step: move to the nested message descriptor
  if (field->type() == FieldDescriptor::TYPE_MESSAGE) {
    std::string next_part = PathToNumbers(field->message_type(), remaining);
    return current_num + "." + next_part;
  }

  // Error: we have more path segments but reached a primitive field
  return current_num + ".ERR_LEAF_REACHED_" + remaining;
}

void DumpPaths(
  const google::protobuf::Descriptor* rootDescriptor,
  const google::protobuf::Descriptor* nextDescriptor,
  std::string prefix = ""
) {
  for (int i = 0; i < nextDescriptor->field_count(); ++i) {
    const google::protobuf::FieldDescriptor* field = nextDescriptor->field(i);
    std::string current_path = prefix + field->name();

    std::string dottedFieldNumbers = PathToNumbers(rootDescriptor, current_path);

    // Print the current path
    std::cout << current_path << " [" << dottedFieldNumbers << "]" <<std::endl;

    // If the field is a nested message, recurse into it
    if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
      DumpPaths(rootDescriptor, field->message_type(), current_path + ".");
    }
  }
}
*/

RadioSettingsPb radioSettings = RadioSettings_RadioSettingsPb_init_zero;

uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
int64_t newFrequency = 7000000;

int main()
{

  // radioSettings.active_bands.band_1.band.lowest_frequency = 14000000;
  // std::cout << "lowest frequency set to: " << radioSettings.active_bands.band_1.band.lowest_frequency << std::endl;
  RadioSettings_SteppableInt64SettingPb* centreFrequency = &radioSettings.active_bands.band_1.pipeline_a.rf.centre_frequency;
  radioSettings.has_active_bands = true;
  radioSettings.active_bands.has_band_1 = true;
  radioSettings.active_bands.band_1.has_pipeline_a = true;
  radioSettings.active_bands.band_1.pipeline_a.has_rf = true;
  radioSettings.active_bands.band_1.pipeline_a.rf.has_centre_frequency = true;
  centreFrequency->value = 14200000;
  centreFrequency->coarse_delta = 1000;
  centreFrequency->fine_delta = 100;
  centreFrequency->use_fine = false;

  std::cout << "Centre frequency set to: " << radioSettings.active_bands.band_1.pipeline_a.rf.centre_frequency.value << std::endl;

  bool tagsFound = lookup_field_path("active_bands.band_1.pipeline_a.rf.centre_frequency", fieldIds, 5);

  // bool tagsFound = lookup_field_path("active_bands.band_1.band.lowest_frequency", fieldIds, 4);
  if (tagsFound) {
    int64_t frequencyChange = -1;
    bool updated = pb_traverse_and_update(&RadioSettings_RadioSettingsPb_msg,
                                   &radioSettings,
                                   fieldIds,
                                   5,
                                   &frequencyChange,
                                   sizeof(frequencyChange));

    if (updated) {
       std::cout << "Centre frequency updated to: " << radioSettings.active_bands.band_1.pipeline_a.rf.centre_frequency.value << std::endl;
      // std::cout << "lowest frequency updated to: " << radioSettings.active_bands.band_1.band.lowest_frequency << std::endl;
      // bool hasActiveBands = radioSettings.has_active_bands;
      // bool hasBand1 = radioSettings.active_bands.has_band_1;
      // bool hasBand2 = radioSettings.active_bands.has_band_2;
      // bool hasBand = radioSettings.active_bands.band_1.has_band;
      // std::cout << "hasActiveBands: " << hasActiveBands << std::endl;
      // std::cout << "hasBand1: " << hasBand1 << std::endl;
      // std::cout << "hasBand2: " << hasBand2 << std::endl;
      // std::cout << "hasBand: " << hasBand << std::endl;

    } else {
      std::cout << "Failed to update radio settings" << std::endl;
    }

  } else {
    std::cout << "Failed to find field ids" << std::endl;
  }


  /*
  google::protobuf::util::Status rc = google::protobuf::util::JsonStringToMessage(json_data, &radioSettings);
  if (rc.ok()) {
    // FieldMask mask = FieldMaskUtil::GetFieldMaskForAllFields<RadioSettings>();
    // mask.PrintDebugString();

    // const google::protobuf::RepeatedPtrField<std::string>& paths = mask.paths();
    // for ( const auto& path : paths ) {
    //   std::cout << path << std::endl;
    // }
    DumpPaths(RadioSettingsPb::descriptor(),RadioSettingsPb::descriptor());
    // Paths are separated by commas
    // FieldMaskUtil::FromString("user.display_name,photo", &mask);

    const auto& band_1 = radioSettings.active_bands().band_1();
    // google::protobuf::RepeatedPtrField<nexusdr::RadioSettings::BandSettings> _bands;
    // FieldMaskUtil::GetFieldValue(radioSettings, "active_bands.bands", &_bands);
    std::string serialised;
    rc = google::protobuf::util::MessageToJsonString(radioSettings, &serialised);
    if (rc.ok()) {
      // std::cout << serialised << std::endl;
    } else {
      std::cout << "Failed to serialise radio settings" << std::endl;
    }
  } else {
    std::cout << "Failed to parse json: " << rc.message() << std::endl;
  }
  */

  auto lang = "C++";
  std::cout << "Hello and welcome to " << lang << "!\n";

  // for (int i = 1; i <= 5; i++) {
  //   std::cout << "i = " << i << std::endl;
  // }

  return 0;
}