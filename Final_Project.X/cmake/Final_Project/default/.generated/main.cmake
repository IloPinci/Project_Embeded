include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Final_Project_default_library_list )

# Handle files with suffix s, for group default-XC16
if(Final_Project_default_default_XC16_FILE_TYPE_assemble)
add_library(Final_Project_default_default_XC16_assemble OBJECT ${Final_Project_default_default_XC16_FILE_TYPE_assemble})
    Final_Project_default_default_XC16_assemble_rule(Final_Project_default_default_XC16_assemble)
    list(APPEND Final_Project_default_library_list "$<TARGET_OBJECTS:Final_Project_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(Final_Project_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(Final_Project_default_default_XC16_assemblePreproc OBJECT ${Final_Project_default_default_XC16_FILE_TYPE_assemblePreproc})
    Final_Project_default_default_XC16_assemblePreproc_rule(Final_Project_default_default_XC16_assemblePreproc)
    list(APPEND Final_Project_default_library_list "$<TARGET_OBJECTS:Final_Project_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(Final_Project_default_default_XC16_FILE_TYPE_compile)
add_library(Final_Project_default_default_XC16_compile OBJECT ${Final_Project_default_default_XC16_FILE_TYPE_compile})
    Final_Project_default_default_XC16_compile_rule(Final_Project_default_default_XC16_compile)
    list(APPEND Final_Project_default_library_list "$<TARGET_OBJECTS:Final_Project_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(Final_Project_default_default_XC16_FILE_TYPE_dependentObject)
add_library(Final_Project_default_default_XC16_dependentObject OBJECT ${Final_Project_default_default_XC16_FILE_TYPE_dependentObject})
    Final_Project_default_default_XC16_dependentObject_rule(Final_Project_default_default_XC16_dependentObject)
    list(APPEND Final_Project_default_library_list "$<TARGET_OBJECTS:Final_Project_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Final_Project_default_default_XC16_FILE_TYPE_bin2hex)
add_library(Final_Project_default_default_XC16_bin2hex OBJECT ${Final_Project_default_default_XC16_FILE_TYPE_bin2hex})
    Final_Project_default_default_XC16_bin2hex_rule(Final_Project_default_default_XC16_bin2hex)
    list(APPEND Final_Project_default_library_list "$<TARGET_OBJECTS:Final_Project_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(Final_Project_default_image_4LtqCcwi ${Final_Project_default_library_list})

set_target_properties(Final_Project_default_image_4LtqCcwi PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${Final_Project_default_output_dir}")
target_link_libraries(Final_Project_default_image_4LtqCcwi PRIVATE ${Final_Project_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
Final_Project_default_link_rule( Final_Project_default_image_4LtqCcwi)

# Call bin2hex function from the rule file
Final_Project_default_bin2hex_rule(Final_Project_default_image_4LtqCcwi)

