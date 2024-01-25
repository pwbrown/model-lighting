#include <string>

namespace Enums {

/** Light Mode */
enum class LightMode {
  Off,
  Running,
  LowBeam,
  HighBeam,
};

/**
 * Convert light mode to string
 * @param mode Light mode
 */
std::string lightModeToStr(LightMode mode) {
  switch (mode) {
  case LightMode::Off:
    return "Off";
  case LightMode::Running:
    return "Running";
  case LightMode::LowBeam:
    return "LowBeam";
  case LightMode::HighBeam:
    return "HighBeam";
  }
}

/**
 * Convert string to light mode
 */
LightMode strToLightMode(std::string mode) {
  if (mode == "Running") {
    return LightMode::Running;
  } else if (mode == "LowBeam") {
    return LightMode::LowBeam;
  } else if (mode == "HighBeam") {
    return LightMode::HighBeam;
  } else {
    return LightMode::Off;
  }
}

} // namespace Enums