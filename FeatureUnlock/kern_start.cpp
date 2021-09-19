//
//  kern_start.cpp
//  FeatureUnlock.kext
//
//  Copyright © 2020 osy86, DhinakG, 2021 Mykola Grymalyuk. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_user.hpp>

#define MODULE_SHORT "dcslv"

#pragma mark - Patches

static mach_vm_address_t orig_cs_require_lv {};

#pragma mark - Patched functions

static boolean_t patched_cs_require_lv(void *proc) {
    return 0;
}

#pragma mark - Patches on start/stop

static void pluginStart() {
	DBGLOG(MODULE_SHORT, "start");
	lilu.onPatcherLoadForce([](void *user, KernelPatcher &patcher) {
		KernelPatcher::RouteRequest csRoute = KernelPatcher::RouteRequest("_cs_require_lv", patched_cs_require_lv, orig_cs_require_lv);
		if (!patcher.routeMultipleLong(KernelPatcher::KernelID, &csRoute, 1))
			SYSLOG(MODULE_SHORT, "failed to route cs require library validation function");
	});
}

// Boot args.
static const char *bootargOff[] {
    "-cson"
};
static const char *bootargDebug[] {
    "-csdbg"
};
static const char *bootargBeta[] {
    "-csbeta"
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
