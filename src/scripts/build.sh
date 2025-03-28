#!/bin/bash

set -eoux pipefail

IMAGE_NAME="das-builder"
CONTAINER_NAME=${IMAGE_NAME}-container

ENV_VARS=$(printenv | awk -F= '{print "--env "$1}')

# local paths
LOCAL_WORKDIR=$(pwd)
LOCAL_BIN_DIR=$LOCAL_WORKDIR/src/bin
LOCAL_ASPECT_CACHE="$HOME/.cache/das/aspect"
LOCAL_BAZEL_CACHE="$HOME/.cache/das/bazel"
LOCAL_BAZELISK_CACHE="$HOME/.cache/das/bazelisk"
LOCAL_PIPTOOLS_CACHE="$HOME/.cache/das/pip-tools"
LOCAL_PIP_CACHE="$HOME/.cache/das/pip"
mkdir -p \
  $LOCAL_ASPECT_CACHE \
  $LOCAL_BAZEL_CACHE \
  $LOCAL_BAZELISK_CACHE \
  $LOCAL_BIN_DIR \
  $LOCAL_PIPTOOLS_CACHE \
  $LOCAL_PIP_CACHE

# container paths
CONTAINER_WORKDIR=/opt/das
CONTAINER_WORKSPACE_DIR=/opt/das/src
CONTAINER_BIN_DIR=$CONTAINER_WORKSPACE_DIR/bin
CONTAINER_ASPECT_CACHE=/home/${USER}/.cache/aspect
CONTAINER_BAZEL_CACHE=/home/${USER}/.cache/bazel
CONTAINER_PIP_CACHE=/home/${USER}/.cache/pip
CONTAINER_PIPTOOLS_CACHE=/home/${USER}/.cache/pip-tools
CONTAINER_BAZELISK_CACHE=/home/${USER}/.cache/bazelisk

if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
  echo "Removing existing container: ${CONTAINER_NAME}"
  docker rm -f "${CONTAINER_NAME}"
fi

docker run --rm \
  --user=$(id -u):$(id -g) \
  --name=$CONTAINER_NAME \
  -e BIN_DIR=$CONTAINER_BIN_DIR \
  $ENV_VARS \
  --network host \
  --volume /etc/passwd:/etc/passwd:ro \
  --volume $LOCAL_PIP_CACHE:$CONTAINER_PIP_CACHE \
  --volume $LOCAL_PIPTOOLS_CACHE:$CONTAINER_PIPTOOLS_CACHE \
  --volume $LOCAL_ASPECT_CACHE:$CONTAINER_ASPECT_CACHE \
  --volume $LOCAL_BAZEL_CACHE:$CONTAINER_BAZEL_CACHE \
  --volume $LOCAL_BAZELISK_CACHE:$CONTAINER_BAZELISK_CACHE \
  --volume $LOCAL_WORKDIR:$CONTAINER_WORKDIR \
  --workdir $CONTAINER_WORKSPACE_DIR \
  ${IMAGE_NAME} \
  ./scripts/bazel_build.sh
