cmake_minimum_required( VERSION 3.0 )
project( bignumlib )

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
endif()

set( LIBRARY_NAME bignum )
set( ROOT_PATH ../ )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_PATH}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin )

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    enable_language(ASM_MASM)
    set (FILES_MASM src/platform/windows/x64/add.asm)
endif()


add_library( ${LIBRARY_NAME} STATIC
            src/bignummath.c
            src/bignum.c
            ${FILES_MASM}
            
)

target_include_directories( ${LIBRARY_NAME} PRIVATE
                           src/include
                           include
)

SET( APP_TEST_EXE bignumlibQA )

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_LIBRARIES})

ADD_EXECUTABLE( ${APP_TEST_EXE}
                test/bignumQA.c
                test/testbase.c
                test/testmath.c
                test/testmathperformance.c
                test/compopenssl.c
) 

target_include_directories(${APP_TEST_EXE} PRIVATE
                           include
                           test/include
)

TARGET_LINK_LIBRARIES( ${APP_TEST_EXE}
                       ${LIBRARY_NAME}
                       ${OPENSSL_LIBRARIES}
)
