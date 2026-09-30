# Use the local upstream Linux source tree directly.
inherit externalsrc

# THISDIR points to:
# meta-het-anant/recipes-kernel/linux
#
# ../../../linux-7.2.7 resolves to:
# yocto_Learning/linux-7.2.7
EXTERNALSRC = "${THISDIR}/../../../linux-7.2.7"

# Keep generated kernel build files inside Yocto's work directory.
EXTERNALSRC_BUILD = "${WORKDIR}/external-build"

S = "${EXTERNALSRC}"
B = "${EXTERNALSRC_BUILD}"


# ---------------------------------------------------------------------------
# Kernel version
# ---------------------------------------------------------------------------

LINUX_VERSION = "7.2.7"
PV = "${LINUX_VERSION}"

KERNEL_LOCALVERSION = "-yocto-rpi5"


# ---------------------------------------------------------------------------
# Kernel configuration
# ---------------------------------------------------------------------------

# The upstream kernel.org source does not contain the Raspberry Pi downstream
# bcm2712_defconfig, so use the standard ARM64 defconfig.
KBUILD_DEFCONFIG:raspberrypi5 = "defconfig"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
 
SRC_URI:append = " \
file://rpi5-prune.cfg \
"
