# The following variables contains the files used by the different stages of the build process.
set(Final_Project_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${Final_Project_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Final_Project_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Final_Project_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${Final_Project_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Final_Project_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Final_Project_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libraries/adc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libraries/pwm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libraries/spi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libraries/timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libraries/uart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../project_main.c")
set_source_files_properties(${Final_Project_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Final_Project_default_default_XC16_FILE_TYPE_link)
set(Final_Project_default_default_XC16_FILE_TYPE_bin2hex)
set(Final_Project_default_image_name "default.elf")
set(Final_Project_default_image_base_name "default")

# The output directory of the final image.
set(Final_Project_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Final_Project")

# The full path to the final image.
set(Final_Project_default_full_path_to_image ${Final_Project_default_output_dir}/${Final_Project_default_image_name})
