#!/bin/bash
if [ ! -d "vcpkg" ]; then
  git clone https://github.com/microsoft/vcpkg.git
  ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi
