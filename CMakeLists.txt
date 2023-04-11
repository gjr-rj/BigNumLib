cmake_minimum_required( VERSION 3.0 )
project( bignumlib )

set( LIBRARY_NAME bignum )
set( ROOT_PATH ../ )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_PATH}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin )

add_library( ${LIBRARY_NAME} STATIC
            src/bignummath.c
            src/bignum.c
)

target_include_directories( ${LIBRARY_NAME} PRIVATE
                           src/include
                           include
)

SET( APP_TEST_EXE bignumlibQA )

ADD_EXECUTABLE( ${APP_TEST_EXE}
                test/bignumQA.c
                test/testbase.c
                test/testmath.c
                test/testmathperformance.c
) 

target_include_directories(${APP_TEST_EXE} PRIVATE
                           include
                           test/include
)

TARGET_LINK_LIBRARIES( ${APP_TEST_EXE}
                       ${LIBRARY_NAME}
)
