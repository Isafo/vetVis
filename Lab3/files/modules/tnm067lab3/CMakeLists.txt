#--------------------------------------------------------------------
# Inviwo TNM067Lab3 Module
ivw_module(TNM067Lab3)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab3processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/caseimages.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/marchingsquares.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/mesh2drenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/noiseprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab3processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/caseimages.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/marchingsquares.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/mesh2drenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/noiseprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add shaders
#set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/tnm067lab3processor.frag
#)
#ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
#set(TEST_FILES
#    ${CMAKE_CURRENT_SOURCE_DIR}/tests/tnm067lab3-test.cpp
#)
#ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)