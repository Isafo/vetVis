#--------------------------------------------------------------------
# Inviwo TNM067Lab4 Module
ivw_module(TNM067Lab4)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/tnm067lab4raycaster.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab4module.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab4moduledefine.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/tnm067lab4raycaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab4module.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lightingcompositeraycaster.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/mipraycaster.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/compositeraycaster.frag
	${CMAKE_CURRENT_SOURCE_DIR}/glsl/isoraycaster.frag
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)