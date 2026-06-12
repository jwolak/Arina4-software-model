# create_spdlog_symlinks.cmake
# This script ensures expected spdlog library symlinks exist in ThirdParty/logger/x64/Release

if(NOT UNIX)
    return()
endif()

set(SPDLOG_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/logger/x64/Release")

if(EXISTS "${SPDLOG_DIR}")
    # Absolute paths for safety
    set(SPDLOG_REAL_SO "${SPDLOG_DIR}/libspdlog.so.1.15")
    set(SPDLOG_REAL_SO_GENERIC "${SPDLOG_DIR}/libspdlog.so")
    set(SPDLOG_DEBUG_SO "${SPDLOG_DIR}/libspdlogd.so.1.15")
    set(SPDLOG_DEBUG_SO_GENERIC "${SPDLOG_DIR}/libspdlogd.so")

    if(NOT EXISTS "${SPDLOG_DEBUG_SO}" AND EXISTS "${SPDLOG_REAL_SO}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SPDLOG_REAL_SO}" "${SPDLOG_DEBUG_SO}"
            RESULT_VARIABLE _create_spdlogd_res
            OUTPUT_QUIET ERROR_QUIET
        )
        if(NOT _create_spdlogd_res EQUAL 0)
            message(WARNING "Failed to create symlink ${SPDLOG_DEBUG_SO} -> ${SPDLOG_REAL_SO}")
        else()
            message(STATUS "Created symlink: ${SPDLOG_DEBUG_SO} -> ${SPDLOG_REAL_SO}")
        endif()
    endif()

    if(NOT EXISTS "${SPDLOG_DEBUG_SO_GENERIC}" AND EXISTS "${SPDLOG_REAL_SO_GENERIC}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SPDLOG_REAL_SO_GENERIC}" "${SPDLOG_DEBUG_SO_GENERIC}"
            RESULT_VARIABLE _create_spdlogd_res2
            OUTPUT_QUIET ERROR_QUIET
        )
        if(NOT _create_spdlogd_res2 EQUAL 0)
            message(WARNING "Failed to create symlink ${SPDLOG_DEBUG_SO_GENERIC} -> ${SPDLOG_REAL_SO_GENERIC}")
        else()
            message(STATUS "Created symlink: ${SPDLOG_DEBUG_SO_GENERIC} -> ${SPDLOG_REAL_SO_GENERIC}")
        endif()
    endif()
endif()
