set(DEPENDENT_MP_BIN2HEXFinal_Project_default_4LtqCcwi "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFFinal_Project_default_4LtqCcwi "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_Project/default.elf")
set(DEPENDENT_TARGET_DIRFinal_Project_default_4LtqCcwi "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Final_Project")
set(DEPENDENT_BYPRODUCTSFinal_Project_default_4LtqCcwi ${DEPENDENT_TARGET_DIRFinal_Project_default_4LtqCcwi}/${sourceFileNameFinal_Project_default_4LtqCcwi}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRFinal_Project_default_4LtqCcwi}/${sourceFileNameFinal_Project_default_4LtqCcwi}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXFinal_Project_default_4LtqCcwi} ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_Project_default_4LtqCcwi} --image ${sourceFileNameFinal_Project_default_4LtqCcwi} ${addressFinal_Project_default_4LtqCcwi} ${modeFinal_Project_default_4LtqCcwi} -mdfp=C:/Users/boli/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRFinal_Project_default_4LtqCcwi}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFFinal_Project_default_4LtqCcwi})
add_custom_target(
    dependent_produced_source_artifactFinal_Project_default_4LtqCcwi 
    DEPENDS ${DEPENDENT_TARGET_DIRFinal_Project_default_4LtqCcwi}/${sourceFileNameFinal_Project_default_4LtqCcwi}.s
    )
