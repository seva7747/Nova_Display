cmake_minimum_required(VERSION 3.16)
# Reuses ../components/nova_display - the GC9A01/LVGL panel driver and
# per-state screens. This project only adds main.c, which picks one state
# to show.
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../components")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(nova_display_states)
