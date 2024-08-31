#!/bin/bash
#thanks dank_meme01
export NDK=/opt/android-ndk
#export ANDROID_ABI=armeabi-v7a
#ABIS=("armeabi-v7a" "arm64-v8a")
#ABIS=("arm64-v8a")
ABIS=("armeabi-v7a")
export MINSDKVERSION=23
#export GEODE_SDK=/home/fire/.geodenightly # remove when geode 2.0.0 is released
export GEODE_BINDINGS_REPO_PATH=/home/fire/.geodebindings
export NO_IMGUI=1


mkdir -p build-android
for ABI in "${ABIS[@]}"; do
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-$MINSDKVERSION -DCMAKE_BUILD_TYPE=RelWithDebInfo -DGEODE_DEBUG=1 -DGEODE_DONT_INSTALL_MODS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DGEODE_DISABLE_FMT_CONSTEVAL=1 -G Ninja -B build-android/$ABI
#cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ANDROID_ABI -DANDROID_PLATFORM=android-$MINSDKVERSION -DCMAKE_BUILD_TYPE=Debug -DGEODE_DEBUG=1 -DGEODE_DONT_INSTALL_MODS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G Ninja -B build-android

cmake --build build-android/$ABI --config RelWithDebInfo
if [[ $ABI == "arm64-v8a" ]] ;then
    echo "pushing"
    adb push ./build-android/arm64-v8a/firee.prism.geode /sdcard/Android/media/com.geode.launcher/game/geode/mods
fi
done
