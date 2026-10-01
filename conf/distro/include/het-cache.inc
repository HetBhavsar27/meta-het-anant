# Shared Yocto download and sstate caches.
#
# These directories are outside individual build directories so compatible
# builds can reuse downloads and completed task artifacts.

YOCTO_CACHE_DIR ?= "/home/einfochips/Project/yocto_Learning/yocto-cache"

DL_DIR ?= "${YOCTO_CACHE_DIR}/downloads"
SSTATE_DIR ?= "${YOCTO_CACHE_DIR}/sstate-cache"
