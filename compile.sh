#!/usr/bin/env bash
#
# Copyright (C) 2022 Ing <https://github.com/wjz304>
#
# This is free software, licensed under the MIT License.
# See /LICENSE for more information.
#

# set -x

# apt install -y curl build-essential bison flex dwarves ruby libncurses-dev libssl-dev libelf-dev

WORK_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

function makeEnvDeploy() {
  ROOT_PATH=$(realpath "${1:-${WORK_PATH}}")
  VERSION=${2:-"7.2"}
  PLATFORM=${3:-"epyc7002"}

  mkdir -p "${ROOT_PATH}"
  if [ ! -d "${ROOT_PATH}/pkgscripts-ng" ]; then
    git clone https://github.com/SynologyOpenSource/pkgscripts-ng.git ${ROOT_PATH}/pkgscripts-ng
  fi
  pushd "${ROOT_PATH}/pkgscripts-ng"
  git reset --hard
  git pull
  # if VERSION == 6.2, checkout 6.2.4
  git checkout DSM${VERSION}$([ "${VERSION}" = "6.2" ] && echo ".4")
  sudo ./EnvDeploy -v ${VERSION}$([ "${VERSION}" = "6.2" ] && echo ".4") -l # Get Available PLATFORMs
  sudo ./EnvDeploy -q -v ${VERSION} -p ${PLATFORM}
  RET=$?
  popd
  [ ${RET} -ne 0 ] && echo "EnvDeploy failed." && return 1

  ENV_PATH="${ROOT_PATH}/build_env/ds.${PLATFORM}-${VERSION}"
  sudo cp -al "${ROOT_PATH}/pkgscripts-ng" "${ENV_PATH}/"

  # Fault tolerance of pkgscripts-ng
  if [ "${PLATFORM}" == "broadwellntbap" -a "${VERSION}" == "7.1" ]; then
    sudo sed -i '/		broadwellnk	BROADWELLNK/a\		broadwellntbap  BROADWELLNTBAP                  linux-4.4.x             Intel Broadwell with ntb kernel config in AP mode' ${ENV_PATH}/pkgscripts-ng/include/platforms
  fi

  sudo rm -f script.sh
  cat >script.sh <<"EOF"
#!/bin/env bash
source ~/.bashrc
mkdir -p /source
cd pkgscripts
[ ${1:0:1} -gt 6 ] && sed -i 's/print(" ".join(kernels))/pass #&/' ProjectDepends.py
sed -i '/PLATFORM_FAMILY/a\\techo "PRODUCT=$PRODUCT" >> $file\n\techo "KSRC=$KERNEL_SEARCH_PATH" >> $file\n\techo "LINUX_SRC=$KERNEL_SEARCH_PATH" >> $file' include/build
./SynoBuild -c -p ${2}
while read line; do if [ ${line:0:1} != "#" ]; then export ${line%%=*}="${line#*=}"; fi; done </env${BUILD_ARCH}.mak
if [ -f "${KSRC}/Makefile" ]; then
  # gcc issue "unrecognized command-line option '--param=allow-store-data-races=0'".
  [ "${1}" == "7.2" ] && sed -i 's/--param=allow-store-data-races=0/--allow-store-data-races/g' ${KSRC}/Makefile
  KVERSION=$(cat ${KSRC}/Makefile | grep ^VERSION | awk -F' ' '{print $3}')
  PATCHLEVEL=$(cat ${KSRC}/Makefile | grep ^PATCHLEVEL | awk -F' ' '{print $3}')
  SUBLEVEL=$(cat ${KSRC}/Makefile | grep ^SUBLEVEL | awk -F' ' '{print $3}')
  [ -f "/env32.mak" ] && echo "KVER=${KVERSION}.${PATCHLEVEL}.${SUBLEVEL}" >>/env32.mak
  [ -f "/env64.mak" ] && echo "KVER=${KVERSION}.${PATCHLEVEL}.${SUBLEVEL}" >>/env64.mak
  CCVER=$($CC --version | head -n 1 | awk -F' ' '{print $3}')
  [ -f "/env32.mak" ] && echo "CCVER=${CCVER}" >>/env32.mak
  [ -f "/env64.mak" ] && echo "CCVER=${CCVER}" >>/env64.mak
fi
EOF
  sudo mv -f script.sh "${ENV_PATH}/script.sh"
  sudo chmod +x "${ENV_PATH}/script.sh"
  sudo chroot "${ENV_PATH}" "/script.sh" "${VERSION}" "${PLATFORM}"
  RET=$?
  [ ${RET} -ne 0 ] && echo "Chroot build failed." && return 1
  return 0
}

function getKver() {
  ROOT_PATH=$(realpath "${1:-${WORK_PATH}}")
  VERSION=${2:-"7.2"}
  PLATFORM=${3:-"epyc7002"}

  ENV_PATH="${ROOT_PATH}/build_env/ds.${PLATFORM}-${VERSION}"
  [ ! -d "${ENV_PATH}" ] && echo "ds.${PLATFORM}-${VERSION} not exist." && return 1

  BUILD_ARCH=$(cat ${ENV_PATH}/root/.bashrc 2>/dev/null | grep BUILD_ARCH | awk -F'=' '{print $2}' | sed "s/'//g")
  KVER=$(cat ${ENV_PATH}/env${BUILD_ARCH}.mak 2>/dev/null | grep KVER | awk -F'=' '{print $2}')
  echo "${KVER}"
  return 0
}

function makelkms() {
  ROOT_PATH=$(realpath "${1:-${WORK_PATH}}")
  VERSION=${2:-"7.2"}
  PLATFORM=${3:-"epyc7002"}
  SOURCE=$(realpath "${4}")
  OUTPUT=$(realpath "${5}")

  ENV_PATH="${ROOT_PATH}/build_env/ds.${PLATFORM}-${VERSION}"
  [ ! -d "${ENV_PATH}" ] && echo "ds.${PLATFORM}-${VERSION} not exist." && return 1

  sudo rm -rf "${ENV_PATH}/source" "${ENV_PATH}/output" 2>/dev/null
  sudo mkdir -p "${ENV_PATH}/source"
  sudo mkdir -p "${ENV_PATH}/output"
  sudo cp -al "${SOURCE}/"* "${ENV_PATH}/source/"

  sudo rm -f script.sh
  cat >script.sh <<"EOF"
#!/bin/env bash
source ~/.bashrc
sed -i 's/^CFLAGS=/#CFLAGS=/g; s/^CXXFLAGS=/#CXXFLAGS=/g' /env${BUILD_ARCH}.mak
while read line; do if [ ${line:0:1} != "#" ]; then export ${line%%=*}="${line#*=}"; fi; done </env${BUILD_ARCH}.mak
# [ "${2}" = "epyc7002" ] && sed -i 's#\$(Q)\$(MAKE) -f \$(srctree)/scripts/Makefile.modpost#\$(Q)\$(MAKE) -i -f \$(srctree)/scripts/Makefile.modpost#g' ${KSRC}/Makefile
cd /source/
[ -z "$(grep 'env.mak' Makefile)" ] && sed -i '1 i include /env.mak' Makefile
VN="v${1:0:1}"
array=(dev-${VN} prod-${VN} test-${VN})
for a in ${array[@]}; do
  PLATFORM="${2}" make -j$(nproc) CC="${CC}" "${a}"
  if [ -f redpill.ko ]; then
    ${STRIP_ORI} -g redpill.ko # Discard symbols from object files.
    RPKOVER=$(modinfo --field=vermagic redpill.ko | awk '{print $1}')
    gzip redpill.ko
    if [ "${2}" = "epyc7002" ]; then
      mv -f ./redpill.ko.gz /output/rp-${2}-${1}-${RPKOVER/+/}-$(echo ${a} | awk -F'-' '{print $1}').ko.gz
    else
      mv -f ./redpill.ko.gz /output/rp-${2}-${RPKOVER/+/}-$(echo ${a} | awk -F'-' '{print $1}').ko.gz
    fi
  else
    echo "error"
  fi
  make clean
done
# ls -al /output
EOF
  sudo mv -f script.sh "${ENV_PATH}/script.sh"
  sudo chmod +x "${ENV_PATH}/script.sh"
  sudo chroot "${ENV_PATH}" "/script.sh" "${VERSION}" "${PLATFORM}"
  [ $? -ne 0 ] && echo "Chroot build failed." && return 1

  mkdir -p "${OUTPUT}"
  sudo cp -a "${ENV_PATH}/output/"* "${OUTPUT}/"
  return 0
}
