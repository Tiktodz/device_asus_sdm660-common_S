/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2019, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;
using std::string;

string heapstartsize, heapgrowthlimit, heapsize,
       heapminfree, heapmaxfree, heaptargetutilization;

void property_override(string prop, string value)
{
    auto pi = (prop_info*) __system_property_find(prop.c_str());

    if (pi != nullptr)
        __system_property_update(pi, value.c_str(), value.size());
    else
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
}

void check_device()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 5072ull * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        heapstartsize = "16m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.5";
        heapminfree = "8m";
        heapmaxfree = "32m";
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xxhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
     } else if (sys.totalram > 2048ull * 1024 * 1024) {
        heapstartsize = "8m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.70";
        heapminfree = "2m";
        heapmaxfree = "8m";
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }
}

void vendor_load_properties()
{
    check_device();

    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);

   // Safetynet Workaround
    property_override("ro.debuggable", "0");
    property_override("ro.build.type", "user");
    property_override("ro.build.tags", "release-keys");
    property_override("ro.build.selinux", "1");
    property_override("ro.boot.warranty_bit", "0");
    property_override("ro.warranty_bit", "0");
    property_override("ro.secure", "1");
    property_override("ro.boot.flash.locked", "1");
    property_override("ro.boot.verifiedbootstate", "green");
    property_override("ro.boot.selinux", "enforcing");
    property_override("ro.boot.veritymode", "enforcing");
    property_override("ro.boot.vbmeta.device_state", "locked");
    property_override("ro.build.description", "walleye-user 8.1.0 OPM1.171019.011 4448085 release-keys");
    property_override("ro.build.fingerprint", "google/walleye/walleye:8.1.0/OPM1.171019.011/4448085:user/release-keys");
    property_override("ro.system.build.fingerprint", "google/walleye/walleye:8.1.0/OPM1.171019.011/4448085:user/release-keys");

}
