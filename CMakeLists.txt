cmake_minimum_required(VERSION 3.10)
project(big)

set(
    SRC_FILES
    src/main.cpp
    src/big.h
    src/big.arithmetic.h
    src/big.cache.h
    src/big.expression.h
    src/big.literals.h
    src/big.negation.h
    src/big.operands.h
    src/big.operations.h
    src/big.options.h
    src/big.powers.h
    src/big.strategy.h
    src/big.utils.h
)

set(
    DOC_FILES
    README.md
    MANUAL.md
    LICENSE.txt
)

add_executable(big ${SRC_FILES} ${DOC_FILES})
set_target_properties(big PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED On)
source_group("Doc Files" FILES ${DOC_FILES})
