# The following variables contains the files used by the different stages of the build process.
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assemble)
set_source_files_properties(${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assembleWithPreprocess)
set_source_files_properties(${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assembleWithPreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_assembleWithPreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ExternalMemory/SRAM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Protokoller/UART_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c")
set_source_files_properties(${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile_cpp)
set_source_files_properties(${Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_compile_cpp} PROPERTIES LANGUAGE CXX)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_link)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_ihex)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_eep)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_lss)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_srec)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_sig)
set(Gruppe_27_Pong_Debug_Debug_avr_gcc_FILE_TYPE_objcopy_memusage)
set(Gruppe_27_Pong_Debug_image_name "Debug.elf")
set(Gruppe_27_Pong_Debug_image_base_name "Debug")

# The output directory of the final image.
set(Gruppe_27_Pong_Debug_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Gruppe_27_Pong")

# The full path to the final image.
set(Gruppe_27_Pong_Debug_full_path_to_image ${Gruppe_27_Pong_Debug_output_dir}/${Gruppe_27_Pong_Debug_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .lss
    .eep
    .srec
    .usersignatures)
list(TRANSFORM output_extensions PREPEND "${Gruppe_27_Pong_Debug_output_dir}/${Gruppe_27_Pong_Debug_image_base_name}")
