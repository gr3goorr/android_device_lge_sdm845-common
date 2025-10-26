/*
 * Copyright (C) 2019, 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService-lge_sdm845"

#include "TouchscreenGesture.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <cutils/properties.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {


const std::string kGesturePath = "/sys/devices/virtual/input/lge_touch/swipe_enable";


const std::map<int32_t, GestureInfo> kGestureInfoMap_judyln = {
    {0, {249, "Swipe Left"}},
    {1, {250, "Swipe Right"}},
    {2, {248, "Swipe Up"}},
    {3, {247, "Swipe Down"}},
};

const std::map<int32_t, GestureInfo> kGestureInfoMap_judypn = {
    {0, {248, "Swipe Up"}},
    {1, {247, "Swipe Down"}},
    {2, {249, "Swipe Left"}},
    {3, {250, "Swipe Right"}},
};

const std::map<int32_t, GestureInfo> kGestureInfoMap_judyp = {
    {0, {248, "Swipe Up"}},
    {1, {247, "Swipe Down"}},
    {2, {250, "Swipe Right"}},
    {3, {249, "Swipe Left"}},
};


TouchscreenGesture::TouchscreenGesture() {
    initializeGestureMap();
}


void TouchscreenGesture::initializeGestureMap() {
    char device[PROPERTY_VALUE_MAX];
    property_get("ro.boot.hardware", device, "UNKNOWN");

    LOG(INFO) << "Initializing gestures for device: " << device;

    if (!strcmp(device, "judyln")) {
        mGestureInfoMap = kGestureInfoMap_judyln;
    } else if (!strcmp(device, "judypn")) {
        mGestureInfoMap = kGestureInfoMap_judypn;
    } else if (!strcmp(device, "judyp")) {
        mGestureInfoMap = kGestureInfoMap_judyp;
    } else {
        LOG(WARNING) << "Unknown device hardware, using default (judyln) gesture map.";
        
        mGestureInfoMap = kGestureInfoMap_judyln; 
    }
}


ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    if (mGestureInfoMap.empty()) {
       LOG(WARNING) << "Gesture map is empty, no gestures supported for this device variant?";
    }

    
    for (const auto& entry : mGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    *_aidl_return = gestures;
    return ndk::ScopedAStatus::ok();
}


ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
   
    const auto entry = mGestureInfoMap.find(gesture.id);
    if (entry == mGestureInfoMap.end()) {
        LOG(ERROR) << "Unsupported gesture ID: " << gesture.id;
        
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::ofstream file(kGesturePath);
    if (!file) {
        LOG(ERROR) << "Failed to open gestures file: " << kGesturePath;
       
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }


    std::string output = std::to_string(gesture.id) + " " + std::to_string(enabled ? 1 : 0);

    LOG(INFO) << "Writing to " << kGesturePath << ": '" << output << "'";

    file << output << std::endl;
    if (!file) {
        LOG(ERROR) << "Failed to write gesture " << output << " to " << kGesturePath;

        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
