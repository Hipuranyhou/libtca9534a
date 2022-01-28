#!/bin/sh

# cleanup
./clean.sh

# create needed
touch AUTHORS ChangeLog NEWS
mkdir -p m4

# autotools
autoreconf --install
