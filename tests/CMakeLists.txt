# ==============================================================================
# Tests - Main Configuration
# ==============================================================================

function(add_tests_from_folder FOLDER LIBRARY PREFIX)
    file(GLOB test_files CONFIGURE_DEPENDS "${FOLDER}/test*.c*")

    file(GLOB helper CONFIGURE_DEPENDS "${FOLDER}/helper*.c*")

    foreach(test_file ${test_files})
        get_filename_component(test_exe ${test_file} NAME_WE)
        string(REGEX REPLACE "^test_" "" base_name ${test_exe})
        set(test_name "${PREFIX}_${base_name}")

        add_executable(${test_exe} ${test_file})

        target_sources(${test_exe}
            PUBLIC
                FILE_SET CXX_MODULES FILES
                ${helper}
        )

        target_link_libraries(${test_exe} PRIVATE ${LIBRARY})
        target_compile_definitions(${test_exe} PRIVATE
            $<$<CONFIG:Debug>:ENGINE_DEBUG>
        )
        add_test(NAME ${test_name} COMMAND ${test_exe})
    endforeach()
endfunction()

add_tests_from_folder("ecs" "FrozenStars" "ecs")

message(STATUS "Tests configured successfully")
