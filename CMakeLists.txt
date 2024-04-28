cmake_minimum_required(VERSION 3.24)
project(PQsignature)

set(CMAKE_CXX_STANDARD 11)
include_directories(.)
link_directories(.)


add_executable(PQsignature main.cpp src/User.cpp PQ/User.h src/Polynomial.cpp PQ/Polynomial.h src/helpers.cpp PQ/helpers.h src/KeyPair.cpp PQ/KeyPair.h CONFIG.ini src/Commitments.cpp PQ/Commitments.h src/Round.cpp PQ/Round.h src/Keccak.cpp PQ/Keccak.h src/MasterReasponse.cpp PQ/MasterResponse.h src/Signature.cpp PQ/Signature.h src/Controller.cpp PQ/Controller.h src/qrcodegen.cpp PQ/qrcodegen.h src/Controller.cpp src/Group_.cpp PQ/Group_.h src/hmac_drbg.cpp src/hmac_sha256.cpp src/sha256.cpp PQ/hmac_drbg.h PQ/hmac_sha256.h PQ/sha256.h)
target_link_libraries(PQsignature libNTL_lib.a)