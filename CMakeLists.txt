cmake_minimum_required(VERSION 3.25)
project(ion-dtn C CXX)

set(
  ${PROJECT_NAME}_INSTALL_TARGETS ON
  CACHE BOOL "(default: ON)"
)

function(dtn_add_program MODULE_NAME PROGRAM_NAME PROGRAM_SOURCE)
  add_executable(${PROGRAM_NAME} ${PROGRAM_SOURCE})
  target_link_libraries(${PROGRAM_NAME} PRIVATE ${MODULE_NAME})
  set_target_properties(
    ${PROGRAM_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  if(${PROJECT_NAME}_INSTALL_TARGETS)
    install(TARGETS ${PROGRAM_NAME} RUNTIME)
  endif()
endfunction()

# ==== ici ====

find_package(Threads REQUIRED)

add_library(ici SHARED)
target_sources(ici PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/bulk/STUB_BULK/bulk.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/crypto/NULL_SUITES/csi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/crypto/csi_debug.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/bulk.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/cbor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/crc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/crypto.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/csi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/ion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/ionsec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/lyst.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/memmgr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/platform.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/platform_sm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/psm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/radix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/rfx.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrhash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrlist.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrmgt.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrstring.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrtable.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sdrxn.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/smlist.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/smrbt.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/sptrace.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/include/zco.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/cbor.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/crc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/ion.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/ionsec.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/lyst.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/lystP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/memmgr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/platform.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/platform_sm.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/psm.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/radix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/radixP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/rfx.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/smlist.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/smrbt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/sptrace.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/library/zco.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrcatlg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrhash.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrlist.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrmgt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrstring.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrtable.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/sdr/sdrxn.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ici/test/owltsim.c
)
target_compile_definitions(ici PUBLIC SPACE_ORDER=3 IMC_EXT)
target_compile_features(ici PUBLIC c_std_99)
target_include_directories(
  ici PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ici>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ici/crypto>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ici/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ici/library>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ici/sdr>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ici PUBLIC m Threads::Threads)
set_target_properties(
  ici PROPERTIES
  OUTPUT_NAME ici
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
if(${PROJECT_NAME}_INSTALL_TARGETS)
  install(
    DIRECTORY ici
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h"
  )
  install(TARGETS ici LIBRARY)
  install(
    PROGRAMS
      ${CMAKE_CURRENT_SOURCE_DIR}/ionstart
      ${CMAKE_CURRENT_SOURCE_DIR}/ionstop
      ${CMAKE_CURRENT_SOURCE_DIR}/ionstart.awk
    TYPE BIN
  )
endif()

dtn_add_program(ici ionadmin ici/utils/ionadmin.c)
dtn_add_program(ici ionwarn ici/utils/ionwarn.c)
dtn_add_program(ici psmwatch ici/utils/psmwatch.c)
dtn_add_program(ici rfxclock ici/daemon/rfxclock.c)

# ==== bp ====

add_library(bp SHARED)
target_sources(bp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bibe/bibe.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bibe/bibe.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bibe/bibeP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/instr/bpsec_instr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/instr/bpsec_instr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_event.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_eventset.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_eventset.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_rule.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/policy/bpsec_policy_rule.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/bcb_aes_gcm_sc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/bcb_aes_gcm_sc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/bib_hmac_sha2_sc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/bib_hmac_sha2_sc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/ion_test_sc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/ion_test_sc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/rfc9173_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/rfc9173_utils.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sc_util.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sc_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sc_value.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sc_value.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sci.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sci.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sci_structs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sci_valmap.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/sci/sci_valmap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/utils/bpsec_asb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/utils/bpsec_asb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/utils/bpsec_asb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/utils/bpsec_util.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/bpsec/utils/bpsec_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/cgr/libcgr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/dtn2/dtn2fw.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/imc/imcfw.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/imc/libimcfw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/include/bp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/include/eureka.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/ipn/ipnfw.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/ipn/libipnfw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/bei.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/bei.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/bpP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/cgr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/eureka.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bae/bae.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bae/bae.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpq/bpq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpq/bpq.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpsec/bcb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpsec/bcb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpsec/bib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/bpsec/bib.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/hcb/hcb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/hcb/hcb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/imc/imc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/imc/imc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/meb/meb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/meb/meb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/pnb/pnb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/pnb/pnb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/snw/snw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/ext/snw/snw.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/libbp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/library/libbpP.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/saga/saga.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/saga/saga.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/stcp/libstcpcla.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/stcp/stcpcla.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/udp/libudpcla.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/udp/udpcla.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/utils/bpsecadmin_config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/bpv7/utils/jsmn.h
)
target_compile_definitions(bp PUBLIC BP_EXTENDED BPQ_EXT)
target_include_directories(
  bp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/bibe>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/bpsec/instr>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/bpsec/policy>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/bpsec/sci>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/bpsec/utils>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/dtn2>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/imc>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/ipn>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/bae>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/bpq>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/bpsec>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/hcb>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/imc>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/meb>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/pnb>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/library/ext/snw>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/bpv7/saga>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(bp PUBLIC ici)
set_target_properties(
  bp PROPERTIES
  OUTPUT_NAME bp
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
if(${PROJECT_NAME}_INSTALL_TARGETS)
  install(
    DIRECTORY bpv7
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h"
  )
  install(TARGETS bp LIBRARY)
endif()

dtn_add_program(bp bpadmin bpv7/utils/bpadmin.c)
dtn_add_program(bp bpcancel bpv7/utils/bpcancel.c)
dtn_add_program(bp bpchat bpv7/test/bpchat.c)
dtn_add_program(bp bpclm bpv7/daemon/bpclm.c)
dtn_add_program(bp bpclock bpv7/daemon/bpclock.c)
dtn_add_program(bp bpcounter bpv7/test/bpcounter.c)
dtn_add_program(bp bpdriver bpv7/test/bpdriver.c)
dtn_add_program(bp bpecho bpv7/test/bpecho.c)
dtn_add_program(bp bping bpv7/test/bping.c)
dtn_add_program(bp bplist bpv7/utils/bplist.c)
dtn_add_program(bp bprecvfile bpv7/utils/bprecvfile.c)
dtn_add_program(bp bpsendfile bpv7/utils/bpsendfile.c)
dtn_add_program(bp bpsink bpv7/test/bpsink.c)
dtn_add_program(bp bpsource bpv7/test/bpsource.c)
dtn_add_program(bp bpstats bpv7/utils/bpstats.c)
dtn_add_program(bp bptrace bpv7/utils/bptrace.c)
dtn_add_program(bp bptransit bpv7/daemon/bptransit.c)
dtn_add_program(bp bpversion bpv7/utils/bpversion.c)
dtn_add_program(bp ipnadmin bpv7/ipn/ipnadmin.c)
dtn_add_program(bp ipnadminep bpv7/ipn/ipnadminep.c)
dtn_add_program(bp ipnfw bpv7/ipn/ipnfw.c)
dtn_add_program(bp lgagent bpv7/utils/lgagent.c)
dtn_add_program(bp lgsend bpv7/utils/lgsend.c)
dtn_add_program(bp stcpcli bpv7/stcp/stcpcli.c)
dtn_add_program(bp stcpclo bpv7/stcp/stcpclo.c)
dtn_add_program(bp udpcli bpv7/udp/udpcli.c)
dtn_add_program(bp udpclo bpv7/udp/udpclo.c)

# ==== ltp ====

add_library(ltp SHARED)
target_sources(ltp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/include/ltp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/include/sda.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/library/libltp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/library/libltpP.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/library/ltpei.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/library/ltpei.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/library/ltpP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/sda/libsda.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/udp/libudplsa.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ltp/udp/udplsa.h
)
target_include_directories(
  ltp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ltp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ltp/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ltp/library>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ltp/library/ext>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ltp PUBLIC bp ici)
set_target_properties(
  ltp PROPERTIES
  OUTPUT_NAME ltp
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
if(${PROJECT_NAME}_INSTALL_TARGETS)
  install(
    DIRECTORY ltp
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h"
  )
  install(TARGETS ltp LIBRARY)
endif()

dtn_add_program(ltp ltpadmin ltp/utils/ltpadmin.c)
dtn_add_program(ltp ltpcli bpv7/ltp/ltpcli.c)
dtn_add_program(ltp ltpclo bpv7/ltp/ltpclo.c)
dtn_add_program(ltp ltpclock ltp/daemon/ltpclock.c)
dtn_add_program(ltp ltpdeliv ltp/daemon/ltpdeliv.c)
dtn_add_program(ltp ltpmeter ltp/daemon/ltpmeter.c)
dtn_add_program(ltp udplsi ltp/udp/udplsi.c)
dtn_add_program(ltp udplso ltp/udp/udplso.c)

# ==== cfdp ====

add_library(cfdp SHARED)
target_sources(cfdp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/include/bputa.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/include/cfdp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/include/cfdpops.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/library/cfdpP.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/library/libcfdp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/library/libcfdpops.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/library/libcfdpP.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cfdp/utils/bpcp.h
)
target_include_directories(
  cfdp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cfdp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cfdp/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cfdp/library>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(cfdp PUBLIC bp ici)
set_target_properties(
  cfdp PROPERTIES
  OUTPUT_NAME cfdp
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
if(${PROJECT_NAME}_INSTALL_TARGETS)
  install(
    DIRECTORY cfdp
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h"
  )
  install(TARGETS cfdp LIBRARY)
endif()

#dtn_add_program(cfdp bpcp cfdp/utils/bpcp.c)
#dtn_add_program(cfdp bpcpd cfdp/utils/bpcpd.c)
dtn_add_program(cfdp bputa cfdp/bp/bputa.c)
dtn_add_program(cfdp cfdpadmin cfdp/utils/cfdpadmin.c)
dtn_add_program(cfdp cfdpclock cfdp/daemon/cfdpclock.c)
dtn_add_program(cfdp cfdptest cfdp/test/cfdptest.c)

add_executable(ionrestart restart/utils/ionrestart.c)
target_link_libraries(ionrestart PRIVATE cfdp ltp bp ici)
set_target_properties(
  ionrestart PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(${PROJECT_NAME}_INSTALL_TARGETS)
  install(TARGETS ionrestart RUNTIME)
endif()
