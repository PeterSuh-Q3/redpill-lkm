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
    #git clone https://${TOKEN:+${TOKEN}@}github.com/RROrg/pkgscripts-ng.git ${ROOT_PATH}/pkgscripts-ng
    #if echo "geminilakenk r1000nk v1000nk" | grep -wq "${PLATFORM}"; then
    #  sudo "${ROOT_PATH}/pkgscripts-ng/maketoolkit.sh" "${PLATFORM}" "${VERSION}" "${ROOT_PATH}/toolkit_tarballs"
    #fi
  fi
  pushd "${ROOT_PATH}/pkgscripts-ng" || exit 1
  git reset --hard
  git pull
  # if VERSION == 6.2, checkout 6.2.4
  git checkout "DSM${VERSION}$([ "${VERSION}" = "6.2" ] && echo ".4")"
  sudo ./EnvDeploy -v "${VERSION}$([ "${VERSION}" = "6.2" ] && echo ".4")" -l # Get Available PLATFORMs
  sudo ./EnvDeploy -q -v "${VERSION}" -p "${PLATFORM}"
  RET=$?
  popd || exit 1
  [ ${RET} -ne 0 ] && echo "EnvDeploy failed." && return 1

  ENV_PATH="${ROOT_PATH}/build_env/ds.${PLATFORM}-${VERSION}"
  sudo cp -al "${ROOT_PATH}/pkgscripts-ng" "${ENV_PATH}/"

  # # Fault tolerance of pkgscripts-ng
  # if [ "${PLATFORM}" == "broadwellntbap" ] && [ "${VERSION}" == "7.1" ]; then
  #   sudo sed -i '/		broadwellnk	BROADWELLNK/a\		broadwellntbap  BROADWELLNTBAP                  linux-4.4.x             Intel Broadwell with ntb kernel config in AP mode' ${ENV_PATH}/pkgscripts-ng/include/platforms
  # fi

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

