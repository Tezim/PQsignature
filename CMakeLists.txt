cmake_minimum_required(VERSION 3.24)
project(PQsignature)

set(CMAKE_CXX_STANDARD 11)
include_directories(.)
link_directories(.)


add_executable(PQsignature main.cpp src/User.cpp includes/PQ/User.h src/Polynomial.cpp includes/PQ/Polynomial.h src/helpers.cpp includes/PQ/helpers.h src/KeyPair.cpp includes/PQ/KeyPair.h CONFIG.ini src/Commitments.cpp includes/PQ/Commitments.h src/Round.cpp includes/PQ/Round.h src/Keccak.cpp includes/PQ/Keccak.h src/MasterReasponse.cpp includes/PQ/MasterResponse.h src/Signature.cpp includes/PQ/Signature.h src/Controller.cpp includes/PQ/Controller.h src/qrcodegen.cpp includes/PQ/qrcodegen.h src/Controller.cpp src/Group_.cpp includes/PQ/Group_.h src/hmac_drbg.cpp src/hmac_sha256.cpp src/sha256.cpp includes/PQ/hmac_drbg.h includes/PQ/hmac_sha256.h includes/PQ/sha256.h)
target_link_libraries(PQsignature libNTL_lib.a)