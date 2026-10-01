# rpm-native must not detect and statically link the build host's libgomp.a.
# A static non-PIC libgomp cannot be linked into RPM shared libraries.
EXTRA_OECMAKE:append:class-native = " \
    -DENABLE_OPENMP=OFF \
    -DWITH_OPENMP=OFF \
"
