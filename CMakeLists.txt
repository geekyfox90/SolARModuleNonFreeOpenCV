cmake_minimum_required(VERSION 2.8)

##################################################
set (VERSION_NUMBER "0.3.1")
project("SolARModuleNonFreeOpenCV")
set (SOURCES 
		src/SolARModuleNonFreeOpencv.cpp
		src/SolARDescriptorsExtractorSIFTOpencv.cpp
		src/SolARDescriptorsExtractorSURF64Opencv.cpp
		src/SolARDescriptorsExtractorSURF128Opencv.cpp
		src/SolARModuleManagerNonFreeOpencv.cpp  
		src/SolARKeypointDetectorNonFreeOpencv.cpp
	)
##################################################

# various macros
include("macros.cmake")
# config setup
setup()
# process packagedependencies.txt
processPackagedependencies()
# define targets (type library)
defineTargets("library" "")



