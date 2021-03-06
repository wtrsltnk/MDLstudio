cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(MDLstudio)

set(CMAKE_CXX_STANDARD 17)

add_executable(MDLstudio WIN32)

target_sources(MDLstudio
    PUBLIC
        source/CBrowseDlg.cpp
        source/CCManager.cpp
        source/CInfoDlg.cpp
        source/CMainFrame.cpp
        source/CMaterialDlg.cpp
        source/COpenGL.cpp
        source/CSequenceDlg.cpp
        source/CWin.cpp
        source/Settings.cpp
        source/WinMain.cpp
        source/CBrowseDlg.h
        source/CCManager.h
        source/CInfoDlg.h
        source/CMainFrame.h
        source/CMaterialDlg.h
        source/COpenGL.h
        source/CSequenceDlg.h
        source/CWin.h
        resource/resource.h
        source/Settings.h
        resource/energy.ico
        resource/explorer.bmp
        resource/hand.cur
        resource/resource.rc
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
)
target_link_libraries(MDLstudio
    PUBLIC
        opengl32.lib
        glu32.lib
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        comdlg32.lib
        advapi32.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        odbc32.lib
        odbccp32.lib
        comctl32.lib
    PUBLIC
        opengl32.lib
        glu32.lib
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        comdlg32.lib
        advapi32.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        odbc32.lib
        odbccp32.lib
        comctl32.lib
)
target_compile_definitions(MDLstudio
    PUBLIC
        $<$<CONFIG:Release>:${_MBCS}>
    PUBLIC
        $<$<CONFIG:Debug>:${_MBCS}>
)

