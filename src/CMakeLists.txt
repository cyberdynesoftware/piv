target_sources(piv PRIVATE main.cc 
    CoreLoop.cc
    Folder.cc
    Image.cc
    AnimatedGIF.cc
    WebpImage.cc
    GUI.cc
    TextWithBackground.cc
    ImageManager.cc
    SceneManager.cc
    ImageView.cc
    MultiImageView.cc
    SingleImageView.cc
    ViewScrollingManager.cc)
target_include_directories(piv PRIVATE ${CMAKE_CURRENT_LIST_DIR})
