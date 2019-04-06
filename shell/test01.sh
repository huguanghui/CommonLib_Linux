#!/usr/bin/env bash

set -x

BRANCH=${1:-master}
GVM_DEST=${2:-$HOME}
GVM_NAME="gvm"
SRC_REPO=${SRC_REPO:-https://github.com/moovweb/gvm.git}

[ "$GVM_DEST" = "$HOME" ] && GVM_NAME=".gvm"

GIT_ROOT=$(git rev-parse --show-toplevel 2>/dev/null)
