SUMMARY = "Het Raspberry Pi 5 minimal development image"
DESCRIPTION = "Minimal Raspberry Pi 5 image with SSH, package management, splash and development utilities"
LICENSE = "MIT"

require recipes-core/images/core-image-minimal.bb


# ---------------------------------------------------------------------------
# Image features
# ---------------------------------------------------------------------------

IMAGE_FEATURES += " \
    debug-tweaks \
    ssh-server-openssh \
    splash \
    package-management \
"


# ---------------------------------------------------------------------------
# Additional packages
# ---------------------------------------------------------------------------

IMAGE_INSTALL:append = " \
    bash \
    nano \
    vim \
    htop \
    procps \
    iproute2 \
    iputils \
    curl \
    wget \
    gcc \
    psplash-raspberrypi \
    kernel-base \
    hello-world \
"
