mv bl61x_SDK/os/bsp/common bl61x_SDK/os/bsp/common_old
cp -r bl61x_SDK/AiPi_bsp/common  bl61x_SDK/os/bsp/common
cp -r bl61x_SDK/AiPi_bsp/dclock bl61x_SDK/os/components/graphics/lvgl/extra/widgets
rm bl61x_SDK/os/components/utils/CMakeLists.txt
cp bl61x_SDK/AiPi_bsp/CMakeLists.txt bl61x_SDK/os/components/utils/
mv bl61x_SDK/os/components/utils/log bl61x_SDK/os/components/utils/log_old
cp -r bl61x_SDK/AiPi_bsp/log bl61x_SDK/os/components/utils