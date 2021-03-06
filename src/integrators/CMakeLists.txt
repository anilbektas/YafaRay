include_directories(${YAF_INCLUDE_DIRS})

add_definitions(-DBUILDING_YAFRAYPLUGIN)

add_library(directlight SHARED directlight.cc common.cc)
target_link_libraries(directlight yafaraycore)

add_library(photonmap SHARED photonintegr.cc common.cc)
target_link_libraries(photonmap yafaraycore)

if(WITH_OPENCL)
	include_directories(${OPENCL_INCLUDE_DIR})
	add_library(photonmapGPU SHARED photonintegrGPU.cc photonintegrGPU_test.cc common.cc)
	target_link_libraries(photonmapGPU yafaraycore opencl_wrapper)
	install (TARGETS photonmapGPU ${YAF_TARGET_TYPE} DESTINATION ${YAF_PLUGIN_DIR})
endif(WITH_OPENCL)

add_library(pathtrace SHARED pathtracer.cc common.cc)
target_link_libraries(pathtrace yafaraycore)

add_library(bidirpath SHARED bidirpath.cc)
target_link_libraries(bidirpath yafaraycore)

add_library(EmissionIntegrator SHARED EmissionIntegrator.cc)
target_link_libraries(EmissionIntegrator yafaraycore)

add_library(SingleScatterIntegrator SHARED SingleScatterIntegrator.cc)
target_link_libraries(SingleScatterIntegrator yafaraycore)

add_library(EmptyVolumeIntegrator SHARED EmptyVolumeIntegrator.cc)
target_link_libraries(EmptyVolumeIntegrator yafaraycore)

add_library(DebugIntegrator SHARED DebugIntegrator.cc)
target_link_libraries(DebugIntegrator yafaraycore)

add_library(SkyIntegrator SHARED SkyIntegrator.cc)
target_link_libraries(SkyIntegrator yafaraycore)

install (TARGETS directlight photonmap pathtrace bidirpath
		EmissionIntegrator SingleScatterIntegrator EmptyVolumeIntegrator DebugIntegrator SkyIntegrator
		${YAF_TARGET_TYPE} DESTINATION ${YAF_PLUGIN_DIR})