//
//  kern_start.cpp
//  FeatureUnlock.kext
//
//  Copyright © 2020 osy86, DhinakG, 2021 Mykola Grymalyuk. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_user.hpp>

#define MODULE_SHORT "sidecar"

#pragma mark - Patches

// CoreBrightness.framework
// Lowers NightShift requirement: 10.13.1+
static const uint8_t kNightShiftOriginal[] = { 0x44, 0x61, 0x6E, 0x67, 0x75, 0x61, 0x67, 0x65, 0x20, 0x43, 0x68, 0x6F, 0x6F, 0x73, 0x65, 0x72, 0x2E, 0x61, 0x70, 0x70 };
static const uint8_t kNightShiftPatched[] = { 0x4C, 0x61, 0x6E, 0x67, 0x75, 0x61, 0x67, 0x65, 0x20, 0x43, 0x68, 0x6F, 0x6F, 0x73, 0x65, 0x72, 0x2E, 0x61, 0x70, 0x70 };

static mach_vm_address_t orig_cs_validate {};

#pragma mark - Kernel patching code

template <size_t findSize, size_t replaceSize>
static inline void searchAndPatch(const void *haystack, size_t haystackSize, const char *path, const uint8_t (&needle)[findSize], const uint8_t (&patch)[replaceSize]) {
   if (UNLIKELY(KernelPatcher::findAndReplace(const_cast<void *>(haystack), haystackSize, needle, findSize, patch, replaceSize))) {
	   SYSLOG(MODULE_SHORT, "found function to patch at %s!", path);
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	SYSLOG(MODULE_SHORT, "FOUND!!!!!\n\n\n");
	}
}

#pragma mark - Patched functions

// For Big Sur and newer
static void patched_cs_validate_page(vnode_t vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data, int *validated_p, int *tainted_p, int *nx_p) {
    char path[PATH_MAX];
    int pathlen = PATH_MAX;
    FunctionCast(patched_cs_validate_page, orig_cs_validate)(vp, pager, page_offset, data, validated_p, tainted_p, nx_p);
	if (vn_getpath(vp, path, &pathlen) == 0 && UserPatcher::matchSharedCachePath(path)) {
        searchAndPatch(data, PAGE_SIZE, path, kNightShiftOriginal, kNightShiftPatched);
    }
}

#pragma mark - Patches on start/stop

static void pluginStart() {
	DBGLOG(MODULE_SHORT, "start");
	lilu.onPatcherLoadForce([](void *user, KernelPatcher &patcher) {
		KernelPatcher::RouteRequest csRoute = KernelPatcher::RouteRequest("_cs_validate_page", patched_cs_validate_page, orig_cs_validate);
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
