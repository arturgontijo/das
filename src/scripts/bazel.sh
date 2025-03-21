#!/bin/bash

set -eou pipefail

ARCH=$(uname -m)

IMAGE_NAME="das-builder"
CONTAINER_NAME=${IMAGE_NAME}-container
CONTAINER_CACHE_DIR=$(if [ "$ARCH" = "arm64" ]; then echo "/root/.cache"; else echo "/home/$USER/.cache"; fi)
BAZEL_CMD="/opt/bazel/bazelisk"

# local paths
LOCAL_WORKDIR=$(pwd)
LOCAL_BIN_DIR=$LOCAL_WORKDIR/src/bin
LOCAL_ASPECT_CACHE="$HOME/.cache/das/aspect"
LOCAL_BAZEL_CACHE="$HOME/.cache/das/bazel"
LOCAL_BAZELISK_CACHE="$HOME/.cache/das/bazelisk"
LOCAL_PIPTOOLS_CACHE="$HOME/.cache/das/pip-tools"
LOCAL_PIP_CACHE="$HOME/.cache/das/pip"

mkdir -p \
  "$LOCAL_ASPECT_CACHE" \
  "$LOCAL_BIN_DIR" \
  "$LOCAL_BAZEL_CACHE" \
  "$LOCAL_BAZELISK_CACHE" \
  "$LOCAL_PIPTOOLS_CACHE" \
  "$LOCAL_PIP_CACHE"

# container paths
CONTAINER_WORKDIR=/opt/das
CONTAINER_WORKSPACE_DIR=/opt/das/src
CONTAINER_BIN_DIR=$CONTAINER_WORKSPACE_DIR/bin
CONTAINER_ASPECT_CACHE=${CONTAINER_CACHE_DIR}/aspect
CONTAINER_BAZEL_CACHE=${CONTAINER_CACHE_DIR}/bazel
CONTAINER_PIP_CACHE=${CONTAINER_CACHE_DIR}/pip
CONTAINER_PIPTOOLS_CACHE=${CONTAINER_CACHE_DIR}/pip-tools
CONTAINER_BAZELISK_CACHE=${CONTAINER_CACHE_DIR}/bazelisk

if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
  echo "Removing existing container: ${CONTAINER_NAME}"
  docker rm -f "${CONTAINER_NAME}"
fi

docker run --rm \
  --name=$CONTAINER_NAME \
  -e BIN_DIR=$CONTAINER_BIN_DIR \
  $(if [ "$ARCH" = "arm64" ]; then echo "--user=root"; else echo "--user=$(id -u):$(id -g) --volume /etc/passwd:/etc/passwd:ro"; fi) \
  --network=host \
  --volume "$LOCAL_PIP_CACHE":"$CONTAINER_PIP_CACHE" \
  --volume "$LOCAL_PIPTOOLS_CACHE":"$CONTAINER_PIPTOOLS_CACHE" \
  --volume "$LOCAL_ASPECT_CACHE":"$CONTAINER_ASPECT_CACHE" \
  --volume "$LOCAL_BAZEL_CACHE":"$CONTAINER_BAZEL_CACHE" \
  --volume "$LOCAL_BAZELISK_CACHE":"$CONTAINER_BAZELISK_CACHE" \
  --volume "$LOCAL_WORKDIR":"$CONTAINER_WORKDIR" \
  --workdir "$CONTAINER_WORKSPACE_DIR" \
  --entrypoint "$BAZEL_CMD" \
  "${IMAGE_NAME}" \
  "$@"
