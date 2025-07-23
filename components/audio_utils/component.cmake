# components/audio_utils/component.cmake

idf_component_register(SRCS "audio_utils.c"
                        INCLUDE_DIRS "include"
                        REQUIRES "opus" "lame" "libsndfile" )  # 加入依赖
