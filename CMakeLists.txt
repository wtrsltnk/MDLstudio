cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(MDLstudio)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(MDLstudio WIN32)

target_sources(MDLstudio
    PUBLIC
        hlsdk/cmdlib.c
        hlsdk/cmdlib.h
        hlsdk/mathlib.c
        hlsdk/mathlib.h
        hlsdk/studio.h
        hlsdk/studio_event.h
        hlsdk/studio_export.cpp
        hlsdk/studio_import.cpp
        hlsdk/studio_render.cpp
        hlsdk/studio_render.h
        hlsdk/studio_utils.cpp
        resource/energy.ico
        resource/explorer.bmp
        resource/hand.cur
        resource/resource.h
        resource/resource.rc
        source/CBrowseDlg.cpp
        source/CBrowseDlg.h
        source/CCManager.cpp
        source/CCManager.h
        source/CInfoDlg.cpp
        source/CInfoDlg.h
        source/CMainFrame.cpp
        source/CMainFrame.h
        source/CMaterialDlg.cpp
        source/CMaterialDlg.h
        source/COpenGL.cpp
        source/COpenGL.h
        source/CSequenceDlg.cpp
        source/CSequenceDlg.h
        source/CWin.cpp
        source/CWin.h
        source/Settings.cpp
        source/Settings.h
        source/WinMain.cpp
        source/controls/CButton.cpp
        source/controls/CButton.h
        source/controls/CCommonDialogs.cpp
        source/controls/CCommonDialogs.h
        source/controls/CControl.cpp
        source/controls/CControl.h
        source/controls/CFileView.cpp
        source/controls/CFileView.h
        source/controls/CFolderView.cpp
        source/controls/CFolderView.h
        source/controls/CLabel.cpp
        source/controls/CLabel.h
        source/controls/CSlider.cpp
        source/controls/CSlider.h
        source/controls/CStatusBar.cpp
        source/controls/CStatusBar.h
        source/controls/CToolBar.cpp
        source/controls/CToolBar.h
        source/controls/CTreeView.cpp
        source/controls/CTreeView.h
        source/controls/GUI.H
        source/decompiler/hlsdk/activity.h
        source/glad.c
)

target_link_libraries(MDLstudio
    ${OPENGL_LIBRARIES}
    winmm
    comctl32
    )

target_compile_features(MDLstudio
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )

target_include_directories(MDLstudio
    PRIVATE source
)
