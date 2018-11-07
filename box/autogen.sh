#!/bin/sh

aclocal || exit 1
glibtoolize --force || exit 1
aclocal || exit 1

# Patch aclocal.m4 to get rid of the !!LAME!! sys_lib_search_path_spec.
# Without this patch, libtool picks up native libraries instead of letting
# the compiler pull them in from compiler specific directories. The result
# is that cross libraries are not used when native versions exist on the
# system.

if grep "^sys_lib_search_path_spec=.*/" aclocal.m4 >/dev/null ; then
    sed -e "s/^sys_lib_search_path_spec=.*\/.*/sys_lib_search_path_spec=\"\"/" aclocal.m4 >aclocal.m4.new
    mv aclocal.m4 aclocal.m4.old
    mv aclocal.m4.new aclocal.m4
    echo "Fixed bad sys_lib_search_path_spec"
fi

autoheader || exit 1

automake --add-missing || exit 1
automake --add-missing || exit 1
automake || exit 1

autoconf || exit 1

