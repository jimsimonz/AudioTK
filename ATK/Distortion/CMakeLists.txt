
FILE(GLOB_RECURSE
  ATK_DISTORTION_SRC
  *.cpp
)

FILE(GLOB_RECURSE
  ATK_DISTORTION_HEADERS
  *.h*
)

SET(ATK_DISTORTION_DEFINITIONS)
SET(ATK_DISTORTION_INCLUDE)
SET(ATK_DISTORTION_LIBRARIES ATKCore)

SET(ATK_DISTORTION_NAME ATKDistortion)
SET(ATK_DISTORTION_FOLDER Distortion)

ATK_add_library(ATK_DISTORTION)
