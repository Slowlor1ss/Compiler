# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

project(CompilerConstruction)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# set compiler to 20
set(CMAKE_CXX_STANDARD 20)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/Antlr/antlr-4.11.1-complete.jar)

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
# Two grammar files
# antlr_target(SampleGrammarLexer Antlr/grammar/TLexer.g4 LEXER
#              PACKAGE antlrcpptest)
# antlr_target(SampleGrammarParser Antlr/grammar/TParser.g4 PARSER VISITOR
#              PACKAGE antlrcpptest
#              DEPENDS_ANTLR SampleGrammarLexer
#              COMPILE_FLAGS -lib ${ANTLR_SampleGrammarLexer_OUTPUT_DIR})

# One grammar file
antlr_target(CricketGrammar Antlr/grammar/Cricket.g4 VISITOR
             PACKAGE antlrcpp)
             #COMPILE_FLAGS -lib ${ANTLR_CricketGrammar_OUTPUT_DIR})

# include generated files in project environment
# Two grammar files
#include_directories(${ANTLR_SampleGrammarLexer_OUTPUT_DIR})
#include_directories(${ANTLR_SampleGrammarParser_OUTPUT_DIR})

# One grammar file
include_directories(${ANTLR_CricketGrammar_OUTPUT_DIR})

# Include main module
file(GLOB compiler_SRC CONFIGURE_DEPENDS
     "Src/*.h"
     "Src/*.cpp"
     "Src/Intermediate/*.h"
     "Src/Intermediate/*.cpp"
)

# add generated grammar to compiler binary target
add_executable(compiler ${compiler_SRC}
                # Two grammar files
               #${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}
               #${ANTLR_SampleGrammarParser_CXX_OUTPUTS}

               # One grammar file
               ${ANTLR_CricketGrammar_CXX_OUTPUTS}
               )
target_link_libraries(compiler antlr4_static)