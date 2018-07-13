cmake_minimum_required(VERSION 3.7.2)

##################################################
set (VERSION_NUMBER "0.4.0")
project("SolARModuleNonFreeOpenCV")
set (SOURCES 
		src/SolARModuleNonFreeOpencv.cpp
		src/SolARDescriptorsExtractorSIFTOpencv.cpp
		src/SolARDescriptorsExtractorSURF64Opencv.cpp
		src/SolARDescriptorsExtractorSURF128Opencv.cpp
		src/SolARKeypointDetectorNonFreeOpencv.cpp
	)

install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/xpcf_SolARModuleNonFreeOpenCV_registry.xml" DESTINATION $ENV{BCOMDEVROOT}/.xpcf/SolAR/	)

##################################################

# various macros
include("$ENV{BCOMDEVROOT}/bcomBuild/SolARFramework/solarmacros.cmake")
# config setup
setup()
# process packagedependencies.txt
processPackagedependencies()
# define targets (type library)
defineTargets("library" "")



