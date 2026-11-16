SUMMARY = "Educational character driver"
DESCRIPTION = "Memory-backed miscellaneous character driver for Raspberry Pi 5 driver learning"
LICENSE = "GPL-2.0-only"

LIC_FILES_CHKSUM = "file://het_char.c;beginline=1;endline=1;md5=fcab174c20ea2e2bc0be64b493708266"

inherit module

SRC_URI = " \
    file://het_char.c \
    file://Makefile \
"

S = "${WORKDIR}"

# Keep this frequently rebuilt educational module as an independent,
# unstripped file. Avoid separate .debug and source-copy processing.
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

# Keep a stable package name for image installation. The module class creates
# the actual .ko package with the kernel release appended to its package name.
ALLOW_EMPTY:${PN} = "1"
RDEPENDS:${PN} += "kernel-module-het-char-${KERNEL_VERSION}"

COMPATIBLE_MACHINE = "(raspberrypi5|raspberrypi5-mainline)"
