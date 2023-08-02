#!/bin/bash
set -e

TOP=$(cd $(dirname $0) && pwd)
DATE=$(date +%Y%m%d%H)
echo $DATE
mkdir -p tools/Hummingbird-M-Production-Tool/bin

DEMO_RUN=$(sed -n '/#define[[:space:]]USER_RUN_DEMO[[:space:]]/p' user/inc/user_config.h | awk '{split($0, a);print a[3]}')
DEMO_NAME=$(sed -n '/#define[[:space:]]USER_RUN_DEMO_SELECT/p' user/inc/user_config.h | awk '{split($0, a);print a[3]}')
echo $DEMO_RUN
if [ "${DEMO_RUN}"x == "1"x ]; then
    echo $DEMO_NAME
    if [ "${DEMO_NAME}"x == "USER_DEMO_HOUSEKEEPER"x ]; then
        sed -i "/USE_UNIONE_PROTOCOL/s/ 1 / 0 /g" user/inc/user_config.h
        sed -i "/USE_SAMPLE_PROTOCOL/s/ 1 / 0 /g" user/inc/user_config.h
        rm -rf tools/scripts/tones/*.mp3
        cp -raf user/src/examples/res_housekeeper/* tools/scripts/
        cp -f user/src/examples/inc_housekeeper/* include/
    elif [ "${DEMO_NAME}"x == "USER_DEMO_SMART_AC"x ]; then
        rm -rf tools/scripts/tones/*.mp3
        cp -raf user/src/examples/res_smart_ac/* tools/scripts/
        cp -f user/src/examples/inc_smart_ac/* include/
        if [ "${BUILD_LANG}"x == "en"x ]; then
            cp tools/scripts/asrfix_en.dat tools/scripts/asrfix.dat
        else
            cp tools/scripts/asrfix_cn.dat tools/scripts/asrfix.dat
        fi
    fi
fi

rm -rf output/*.bin
for i in release debug ; do
    if [ "release"x == "${i}"x ]; then
        cp tools/scripts/config_release.bin tools/scripts/config.bin
        cp tools/scripts/aik_release.json tools/scripts/aik.json
    else
        cp tools/scripts/config_debug.bin tools/scripts/config.bin
        cp tools/scripts/aik_debug.json tools/scripts/aik.json
    fi
    cd tools/scripts && python res_build_tool.py manual
    cd -
    if [ "update"X == "$1"X ]; then
        cd build && make clean OTA_UPDATE=TRUE && make OTA_UPDATE=TRUE
    else
        cd build && make clean && make
    fi
    cd -
    if [ "release"x == "${i}"x ]; then
        mv -f output/uni_app.bin output/uni_app_release.bin
        if [ "update"X == "$1"X ]; then
            dd if=output/uni_app_release.bin of=output/uni_app_release_update.bin bs=1k skip=64
        fi
    else
        mv -f output/uni_app.bin output/uni_app_debug.bin
        if [ "update"X == "$1"X ]; then
            dd if=output/uni_app_debug.bin of=output/uni_app_debug_update.bin bs=1k skip=64
        fi
    fi
done
if [ "update"X == "$1"X ]; then
    cd build && make clean OTA_UPDATE=TRUE
else
    cd build && make clean
fi
cd -
echo "====== build done ==> output/ ======"
