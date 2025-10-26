/*
 * Copyright (C) 2019, 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnTouchscreenGesture.h>
#include <android/binder_status.h>
#include <vector>
#include <map>
#include <string>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

using ::aidl::vendor::lineage::touch::Gesture;

struct GestureInfo {
    int32_t keycode;
    const char* name;
};

class TouchscreenGesture : public BnTouchscreenGesture {
  public:
    TouchscreenGesture();

    
    ndk::ScopedAStatus getSupportedGestures(std::vector<Gesture>* _aidl_return) override;
    ndk::ScopedAStatus setGestureEnabled(const Gesture& gesture, bool enabled) override;

  private:
    std::map<int32_t, GestureInfo> mGestureInfoMap;
    void initializeGestureMap(); 
};

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
