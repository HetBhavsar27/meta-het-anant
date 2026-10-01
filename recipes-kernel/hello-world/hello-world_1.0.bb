SUMMARY = "Hello World out-of-tree Linux kernel module"
DESCRIPTION = "Kernel module that prints a Hello World message during system startup"
HOMEPAGE = "https://github.com/HetBhavsar27"
LICENSE = "GPL-2.0-only"

LIC_FILES_CHKSUM = "file://hello_world.c;beginline=1;endline=1;md5=fcab174c20ea2e2bc0be64b493708266"

inherit module update-rc.d

SRC_URI = " \
    file://hello_world.c \
    file://Makefile \
    file://hello_world.init \
"

S = "${WORKDIR}"

# Keep the kernel module in the stable hello-world package.
KERNEL_SPLIT_MODULES = "0"

# This small development module does not need a separate debug copy.
# Disabling strip/debug splitting prevents hard-link and inode reuse around
# hello_world.ko during repeated kernel and image rebuilds.
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME:${PN} = "hello-world"
INITSCRIPT_PARAMS:${PN} = "start 99 2 3 4 5 . stop 10 0 1 6 ."

# Always begin installation with a completely empty destination directory.
do_install[cleandirs] = "${D}"

do_install() {
    install -d \
        ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/updates

    install -m 0644 \
        ${S}/hello_world.ko \
        ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/updates/hello_world.ko

    install -d ${D}${sysconfdir}/init.d

    install -m 0755 \
        ${WORKDIR}/hello_world.init \
        ${D}${sysconfdir}/init.d/hello-world
}

FILES:${PN} += " \
    ${nonarch_base_libdir}/modules/${KERNEL_VERSION}/updates/hello_world.ko \
    ${sysconfdir}/init.d/hello-world \
"

COMPATIBLE_MACHINE = "(raspberrypi5|raspberrypi5-mainline)"
