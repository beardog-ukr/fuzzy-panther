cmake_minimum_required(VERSION 3.6)

set(APP_NAME SimpleGamesCollection)

project(${APP_NAME})

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

if(NOT DEFINED BUILD_ENGINE_DONE) # to test SimpleGamesCollection into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/asteroids/AsteroidNode.cpp
     Classes/asteroids/AsteroidsScene.cpp
     Classes/asteroids/GameManager.cpp
     Classes/asteroids/GameEndScene.cpp
     Classes/asteroids/LaserNode.cpp
     Classes/asteroids/SpaceshipNode.cpp
     Classes/bird/BirdMainScene.cpp
     Classes/bird/GameEndScene.cpp
     Classes/bird/MapSectionNode.cpp
     Classes/bird/WizardNode.cpp
     Classes/blackjack/GameTableNode.cpp
     Classes/blackjack/BlackboardNode.cpp
     Classes/blackjack/BlackjackMainScene.cpp
     Classes/blackjack/GameStateKeeper.cpp
     Classes/blocks/BlocksMainScene.cpp
     Classes/blocks/BlocksGameNode.cpp
     Classes/blocks/FigureInfo.cpp
     Classes/blocks/FigureFactory.cpp
     Classes/blocks/WallInfo.cpp
     Classes/eyes/EyesMainScene.cpp
     Classes/fifteen/BoardCombinationsFactory.cpp
     Classes/fifteen/FifteenGameNode.cpp
     Classes/fifteen/FifteenMainScene.cpp
     Classes/fifteen/TokenNode.cpp
     Classes/flowers/FlowersGameNode.cpp
     Classes/flowers/FlowersMainScene.cpp
     Classes/flowers/GameStateKeeper.cpp
     Classes/flowers/TileNode.cpp
     Classes/life/CellNode.cpp
     Classes/life/GameNode.cpp
     Classes/life/LifeMainScene.cpp
     Classes/main_menu/BackgroundNode.cpp
     Classes/main_menu/MainMenuScene.cpp
     Classes/repeat/DigitNode.cpp
     Classes/repeat/GameNode.cpp
     Classes/repeat/RepeatMainScene.cpp
     Classes/snake/BodyPartsFactory.cpp
     Classes/snake/GameNode.cpp
     Classes/snake/GameStateKeeper.cpp
     Classes/snake/SnakeMainScene.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/asteroids/AsteroidNode.h
     Classes/asteroids/AsteroidsScene.h
     Classes/asteroids/GameManager.h
     Classes/asteroids/GameEndScene.h
     Classes/asteroids/LaserNode.h
     Classes/asteroids/SpaceshipNode.h
     Classes/asteroids/ZOrderValues.h
     Classes/bird/BirdMainScene.h
     Classes/bird/GameEndScene.h
     Classes/bird/CommonUtils.h
     Classes/bird/MapSectionNode.h
     Classes/bird/WizardNode.h
     Classes/blackjack/GameTableNode.h
     Classes/blackjack/BlackboardNode.h
     Classes/blackjack/BlackjackCommonUtils.h
     Classes/blackjack/BlackjackMainScene.h
     Classes/blackjack/GameStateKeeper.h
     Classes/blocks/BlocksMainScene.h
     Classes/blocks/BlocksGameNode.h
     Classes/blocks/BrickInfo.h
     Classes/blocks/CommonUtils.h
     Classes/blocks/FigureInfo.h
     Classes/blocks/FigureFactory.h
     Classes/blocks/WallInfo.h
     Classes/eyes/EyesMainScene.h
     Classes/fifteen/BoardCombinationsFactory.h
     Classes/fifteen/FifteenGameNode.h
     Classes/fifteen/FifteenMainScene.h
     Classes/fifteen/TokenNode.h
     Classes/fifteen/ZOrderValues.h
     Classes/flowers/FlowersGameNode.h
     Classes/flowers/FlowersMainScene.h     
     Classes/flowers/GameStateKeeper.h
     Classes/flowers/TileNode.h
     Classes/flowers/TileType.h
     Classes/flowers/ZOrderValues.h
     Classes/life/CellNode.h
     Classes/life/GameNode.h
     Classes/life/LifeMainScene.h
     Classes/life/ZOrderValues.h
     Classes/main_menu/BackgroundNode.h
     Classes/main_menu/MainMenuScene.h
     Classes/repeat/DigitNode.h
     Classes/repeat/GameNode.h
     Classes/repeat/RepeatMainScene.h
     Classes/repeat/ZOrderValues.h
     Classes/snake/BodyPartsFactory.h
     Classes/snake/GameNode.h
     Classes/snake/GameStateKeeper.h
     Classes/snake/SnakeMainScene.h
     Classes/snake/ZOrderValues.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

add_subdirectory(c6_lib)
target_link_libraries(${APP_NAME} c6_lib)

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")

    if(MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
