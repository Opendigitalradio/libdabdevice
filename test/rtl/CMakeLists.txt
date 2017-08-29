set(CUTE_GROUP "rtl")

cute_test(file
  LIBRARIES ${${PROJECT_NAME}_LOWER} ${${${PROJECT_NAME}_UPPER}_DEPS})
