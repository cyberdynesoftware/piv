target_sources(piv PRIVATE main.cc CoreLoop.cc InputEvent.cc MultiImageView.cc Folder.cc Image.cc AnimatedGIF.cc WebpImage.cc GUI.cc TextWithBackground.cc SceneManager.cc ImageManager.cc SingleImageView.cc)
target_include_directories(piv PRIVATE ${CMAKE_CURRENT_LIST_DIR})
