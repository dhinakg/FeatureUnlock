//
//  kern_start.cpp
//  SidecarFixup.kext
//
//  Copyright © 2020 osy86, DhinakG, 2021 Mykola Grymalyuk. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_user.hpp>

#define MODULE_SHORT "sidecar"

#pragma mark - Patches

// Replaces Mac with Nac in SidecarCore
static const uint8_t kMacModelOriginal[] = { 0x69, 0x4D, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x31, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x32, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x33, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x31, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x32, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x33, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x34, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x35, 0x2C, 0x31, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x36, 0x2C, 0x31, 0x00, 0x69, 0x4D, 0x61, 0x63, 0x31, 0x36, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x38, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x35, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x35, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x36, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x36, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x37, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x37, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x39, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x39, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x30, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x30, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x33, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x34, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x35, 0x00, 0x4D, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x32, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x36, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x36, 0x2C, 0x32, 0x00, 0x4D, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x37, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x50, 0x72, 0x6F, 0x35, 0x2C, 0x31, 0x00, 0x4D, 0x61, 0x63, 0x50, 0x72, 0x6F, 0x36, 0x2C, 0x31
};

static const uint8_t kMacModelPatched[] = { 0x69, 0x4E, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x31, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x32, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x33, 0x2C, 0x33, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x31, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x32, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x33, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x34, 0x2C, 0x34, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x35, 0x2C, 0x31, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x36, 0x2C, 0x31, 0x00, 0x69, 0x4E, 0x61, 0x63, 0x31, 0x36, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x38, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x35, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x35, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x36, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x36, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x37, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x41, 0x69, 0x72, 0x37, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x39, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x39, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x30, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x30, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x33, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x34, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x31, 0x2C, 0x35, 0x00, 0x4E, 0x61, 0x63, 0x42, 0x6F, 0x6F, 0x6B, 0x50, 0x72, 0x6F, 0x31, 0x32, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x36, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x36, 0x2C, 0x32, 0x00, 0x4E, 0x61, 0x63, 0x6D, 0x69, 0x6E, 0x69, 0x37, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x50, 0x72, 0x6F, 0x35, 0x2C, 0x31, 0x00, 0x4E, 0x61, 0x63, 0x50, 0x72, 0x6F, 0x36, 0x2C, 0x31
};

// Replaces iPad with hPad
static const uint8_t kIPadModelOriginal[] = { 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x31, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x32, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x33, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x34, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x35, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x36, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x37, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x38, 0x00, 0x69, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x39, 0x00, 0x69, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x31, 0x00, 0x69, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x32, 0x00, 0x69, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x33, 0x00, 0x69, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x34, 0x00, 0x69, 0x50, 0x61, 0x64, 0x36, 0x2C, 0x31, 0x31, 0x00, 0x69, 0x50, 0x61, 0x64, 0x36, 0x2C, 0x31, 0x32
};

static const uint8_t kIPadModelPatched[] = { 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x31, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x32, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x33, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x34, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x35, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x36, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x37, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x38, 0x00, 0x68, 0x50, 0x61, 0x64, 0x34, 0x2C, 0x39, 0x00, 0x68, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x31, 0x00, 0x68, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x32, 0x00, 0x68, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x33, 0x00, 0x68, 0x50, 0x61, 0x64, 0x35, 0x2C, 0x34, 0x00, 0x68, 0x50, 0x61, 0x64, 0x36, 0x2C, 0x31, 0x31, 0x00, 0x68, 0x50, 0x61, 0x64, 0x36, 0x2C, 0x31, 0x32
};

static_assert(sizeof(kMacModelOriginal) == sizeof(kMacModelPatched), "patch size invalid");
static_assert(sizeof(kIPadModelOriginal) == sizeof(kIPadModelPatched), "patch size invalid");

static mach_vm_address_t orig_cs_validate {};

#pragma mark - Kernel patching code

template <size_t findSize, size_t replaceSize>
static inline void searchAndPatch(const void *haystack, size_t haystackSize, const char *path, const uint8_t (&needle)[findSize], const uint8_t (&patch)[replaceSize]) {
   if (UNLIKELY(KernelPatcher::findAndReplace(const_cast<void *>(haystack), haystackSize, needle, findSize, patch, replaceSize)))
	   DBGLOG(MODULE_SHORT, "found function to patch at %s!", path);
}

#pragma mark - Patched functions

// pre Big Sur
static boolean_t patched_cs_validate_range(vnode_t vp, memory_object_t pager, memory_object_offset_t offset, const void *data, vm_size_t size, unsigned *result) {
    char path[PATH_MAX];
    int pathlen = PATH_MAX;
    boolean_t res = FunctionCast(patched_cs_validate_range, orig_cs_validate)(vp, pager, offset, data, size, result);
    if (res && vn_getpath(vp, path, &pathlen) == 0 && UserPatcher::matchSharedCachePath(path)) {
        searchAndPatch(data, size, path, kMacModelOriginal, kMacModelPatched);
        searchAndPatch(data, size, path, kIPadModelOriginal, kIPadModelPatched);
    }
    return res;
}

// For Big Sur
static void patched_cs_validate_page(vnode_t vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data, int *validated_p, int *tainted_p, int *nx_p) {
    char path[PATH_MAX];
    int pathlen = PATH_MAX;
    FunctionCast(patched_cs_validate_page, orig_cs_validate)(vp, pager, page_offset, data, validated_p, tainted_p, nx_p);
	if (vn_getpath(vp, path, &pathlen) == 0 && UserPatcher::matchSharedCachePath(path)) {
        searchAndPatch(data, PAGE_SIZE, path, kMacModelOriginal, kMacModelPatched);
        searchAndPatch(data, PAGE_SIZE, path, kIPadModelOriginal, kIPadModelPatched);
    }
}

#pragma mark - Patches on start/stop

static void pluginStart() {
	DBGLOG(MODULE_SHORT, "start");
	lilu.onPatcherLoadForce([](void *user, KernelPatcher &patcher) {
		KernelPatcher::RouteRequest csRoute =
			getKernelVersion() >= KernelVersion::BigSur ?
			KernelPatcher::RouteRequest("_cs_validate_page", patched_cs_validate_page, orig_cs_validate) :
			KernelPatcher::RouteRequest("_cs_validate_range", patched_cs_validate_range, orig_cs_validate);
		if (!patcher.routeMultipleLong(KernelPatcher::KernelID, &csRoute, 1))
			SYSLOG(MODULE_SHORT, "failed to route cs validation pages");
	});
}

// Boot args.
static const char *bootargOff[] {
    "-caroff"
};
static const char *bootargDebug[] {
    "-cardbg"
};
static const char *bootargBeta[] {
    "-carbeta"
};

// Plugin configuration.
PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Catalina,
    KernelVersion::Monterey,
    pluginStart
};
