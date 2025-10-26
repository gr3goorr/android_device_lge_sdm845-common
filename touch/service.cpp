/*
 * Copyright (C) 2019, 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.lge_sdm845"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "TouchscreenGesture.h"


using aidl::vendor::lineage::touch::TouchscreenGesture;


int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    LOG(INFO) << "Lineage Touch HAL service (AIDL) for lge_sdm845 is starting.";

    
    std::shared_ptr<TouchscreenGesture> tg = ndk::SharedRefBase::make<TouchscreenGesture>();
  
    const std::string tgInstance = std::string() + TouchscreenGesture::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(tg->asBinder().get(), tgInstance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to register TouchscreenGesture HAL instance";

    LOG(INFO) << "Lineage Touch HAL service (AIDL) for lge_sdm845 started successfully.";
    ABinderProcess_joinThreadPool();


    LOG(ERROR) << "Lineage Touch HAL service (AIDL) for lge_sdm845 somehow exited!";
    return EXIT_FAILURE;
}
