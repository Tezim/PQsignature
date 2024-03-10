cmake_minimum_required(VERSION 3.24)
project(PQsignature)

set(CMAKE_CXX_STANDARD 11)
include_directories(.)
link_directories(.)


add_executable(PQsignature main.cpp structures/User.cpp structures/User.h structures/Polynomial.cpp structures/Polynomial.h crypt/helpers.cpp crypt/helpers.h structures/KeyPair.cpp structures/KeyPair.h CONFIG.h structures/Commitments.cpp structures/Commitments.h structures/Round.cpp structures/Round.h crypt/Keccak.cpp crypt/Keccak.h structures/MasterReasponse.cpp structures/MasterResponse.h structures/Signature.cpp structures/Signature.h controlInterface/Controller.cpp controlInterface/Controller.h QR/qrcodegen.cpp QR/qrcodegen.h controlInterface/Controller.cpp)
target_link_libraries(PQsignature libNTL_lib.a)