inherit externalsrc

EXTERNALSRC = "/home/einfochips/Project/yocto_Learning/linux-7.2.7"
EXTERNALSRC_BUILD = "${WORKDIR}/external-build"

S = "${EXTERNALSRC}"
B = "${EXTERNALSRC_BUILD}"

LINUX_VERSION = "7.2.7"
PV = "${LINUX_VERSION}"

KERNEL_LOCALVERSION = "-yocto-rpi5"

KBUILD_DEFCONFIG:raspberrypi5 = "defconfig"

KERNEL_DEVICETREE = "broadcom/bcm2712-rpi-5-b.dtb"
