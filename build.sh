#export GEODE_SDK=/home/fire/.geodenightly # remove when geode 2.0.0 is released
export SPLAT_DIR=/home/fire/xwin/splat
export TOOLCHAIN_REPO=/home/fire/xwin/toolchain
export GEODE_BINDINGS_REPO_PATH=/home/fire/.geodebindings
#export GEODE_GD_VERSION=2.200
export LOCAL=1
export HOST_ARCH=x64
export MSVC_BASE=$SPLAT_DIR/crt
export WINSDK_BASE=$SPLAT_DIR/sdk
#export WINSDK_VER=10.0.22000
# change this to your llvm version!!!
#export LLVM_VER=15
#export LLVM_VER=17
#export CLANG_VER=$LLVM_VER

#export NO_IMGUI=1

# you can also use `-G Ninja` here
#-DCMAKE_BUILD_TYPE=Release -B build-linux \
#cmake \
#   -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
#   -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_REPO/clang-cl-msvc.cmake \
#   -DCMAKE_BUILD_TYPE=Release -B build-linux \
#   -DGEODE_DISABLE_FMT_CONSTEVAL=1

#cmake --build build-linux --config Release
cmake \
   -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
   -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_REPO/clang-cl-msvc.cmake \
   -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build-linux-deb \
   -DGEODE_DISABLE_FMT_CONSTEVAL=1 -G Ninja

cmake --build build-linux-deb --config RelWithDebInfo
# /SDrive/Users/Fire/Desktop/Restoration/ClarifyGDPS V.1.3/GDGeode-2.206
cp build-linux-deb/firee.prism.geode "/SDrive/Users/Fire/Desktop/Restoration/ClarifyGDPS V.1.3/GDGeode-2.206/geode/mods/firee.prism.geode"
