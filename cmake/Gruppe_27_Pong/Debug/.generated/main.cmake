include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Gruppe_27_Pong_Debug_library_list )

# Handle files with suffix s, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assemble)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_assemble OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assemble})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_assemble_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_assemble)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_assemble>")

endif()

# Handle files with suffix S, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assembleWithPreprocess)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_assembleWithPreprocess OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assembleWithPreprocess})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_assembleWithPreprocess_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_assembleWithPreprocess)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_compile OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_compile_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_compile)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_compile>")

endif()

# Handle files with suffix cpp, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile_cpp)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_compile_cpp OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile_cpp})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_compile_cpp_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_compile_cpp)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_compile_cpp>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_ihex)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_ihex OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_ihex})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_ihex_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_ihex)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_ihex>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_eep)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_eep OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_eep})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_eep_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_eep)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_eep>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_lss)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_lss OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_lss})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_lss_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_lss)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_lss>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_srec)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_srec OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_srec})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_srec_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_srec)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_srec>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_sig)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_sig OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_sig})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_sig_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_sig)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_sig>")

endif()

# Handle files with suffix elf, for group Debug-avr-gcc
if(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_memusage)
add_library(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_memusage OBJECT ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_memusage})
    Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_memusage_rule(Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_memusage)
    list(APPEND Gruppe_27_Pong_Debug_library_list "$<TARGET_OBJECTS:Gruppe_27_Pong_Debug_Debug_avr_gcc_objcopy_memusage>")

endif()


# Main target for this project
add_executable(Gruppe_27_Pong_Debug_image_MpATHMyV ${Gruppe_27_Pong_Debug_library_list})

set_target_properties(Gruppe_27_Pong_Debug_image_MpATHMyV PROPERTIES
    OUTPUT_NAME "Debug"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${Gruppe_27_Pong_Debug_output_dir}")
target_link_libraries(Gruppe_27_Pong_Debug_image_MpATHMyV PRIVATE ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_link})
# Add the link options from the rule file.
Gruppe_27_Pong_Debug_link_rule( Gruppe_27_Pong_Debug_image_MpATHMyV)


#Add objcopy steps
Gruppe_27_Pong_Debug_objcopy_ihex_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)
Gruppe_27_Pong_Debug_objcopy_eep_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)
Gruppe_27_Pong_Debug_objcopy_lss_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)
Gruppe_27_Pong_Debug_objcopy_srec_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)
Gruppe_27_Pong_Debug_objcopy_sig_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)
Gruppe_27_Pong_Debug_objcopy_memusage_rule(Gruppe_27_Pong_Debug_image_MpATHMyV)

