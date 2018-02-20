#include <iostream>

using namespace std;
#include "IComponentManager.h"

#include "SolARModuleManagerNonFreeOpencv.h"
#include "SolARModuleManagerOpencv.h"
#include "SolARModuleManagerTools.h"
#include "SolAROpenCVHelper.h"

//#include "datastructure/SolARDescriptorMatch.h"
#include "SharedBuffer.hpp"

#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;
using namespace SolAR::api::solver::pose;
namespace xpcf  = org::bcom::xpcf;

#include <string>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

/* print this help message to explain which arguments to pass*/
/* the content of the message displayed is in the readme.adoc file*/
int printHelp(){
    LOG_INFO("Missing parameters");
    const char *filename = "readme.adoc";
    std::ifstream ifs(filename);
    if(!ifs){
        LOG_ERROR("File {} does not exist", filename)
        return 1;
    }
    std::string line;
    while(std::getline(ifs,line))
        LOG_INFO("{}", line)
    ifs.close();
    return 1;
}



int main(int argc, char *argv[])
{

//    SolARLog::init();
    LOG_ADD_LOG_TO_CONSOLE();

/*
    if(argc<5) {
        printHelp();
         return -1;
    }
	*/

    LOG_INFO("program is running");


    /* instantiate module manager*/
    /* this is needed in dynamic mode */
    MODULES::OPENCV::SolARModuleManagerOpencv opencvModule(argv[1]);
    if (!opencvModule.isLoaded()) // xpcf library load has failed
    {
        LOG_ERROR("XPCF library load has failed")
        return -1;
    }

    MODULES::TOOLS::SolARModuleManagerTools toolsModule(argv[1]);
    if (!toolsModule.isLoaded()) // xpcf library load has failed
    {
        LOG_ERROR("XPCF library load has failed")
        return -1;
    }


	MODULES::OPENCV::SolARModuleManagerOpencvNonFree opencvNonFreeModule(argv[1]);
    if (!opencvNonFreeModule.isLoaded()) // xpcf library load has failed
    {
        LOG_ERROR("XPCF library load has failed")
        return -1;
    }

    // declare and create components
    LOG_INFO("Start creating components");
    SRef<input::devices::ICamera> camera = opencvModule.createComponent<input::devices::ICamera>(MODULES::OPENCV::UUID::CAMERA);
    SRef<display::IImageViewer> imageViewer = opencvModule.createComponent<display::IImageViewer>(MODULES::OPENCV::UUID::IMAGE_VIEWER);
    SRef<input::files::IMarker2DNaturalImage> marker = opencvModule.createComponent<input::files::IMarker2DNaturalImage>(MODULES::OPENCV::UUID::MARKER2D_NATURAL_IMAGE);
    SRef<features::IKeypointDetector> kpDetector = opencvModule.createComponent<features::IKeypointDetector>(MODULES::OPENCV::UUID::KEYPOINT_DETECTOR);

	SRef<features::IDescriptorMatcher>  matcher = opencvModule.createComponent<features::IDescriptorMatcher>(MODULES::OPENCV::UUID::DESCRIPTOR_MATCHER_KNN);
    SRef<solver::pose::IHomographyEstimation> homographyEstimation = opencvModule.createComponent<solver::pose::IHomographyEstimation>(MODULES::OPENCV::UUID::HOMOGRAPHY_ESTIMATION);
    SRef<solver::pose::IHomographyValidation> homographyValidation = toolsModule.createComponent<solver::pose::IHomographyValidation>(MODULES::TOOLS::UUID::HOMOGRAPHY_VALIDATION);
    SRef<features::IKeypointsReIndexer>   keypointsReindexer = toolsModule.createComponent<features::IKeypointsReIndexer>(MODULES::TOOLS::UUID::KEYPOINTS_REINDEXER);
    SRef<solver::pose::IPoseEstimation> poseEstimation = opencvModule.createComponent<solver::pose::IPoseEstimation>(MODULES::OPENCV::UUID::POSE_ESTIMATION);
    SRef<display::I2DOverlay> overlay2DComponent = opencvModule.createComponent<display::I2DOverlay>(MODULES::OPENCV::UUID::OVERLAY2D);
    SRef<display::ISideBySideOverlay> overlaySBSComponent = opencvModule.createComponent<display::ISideBySideOverlay>(MODULES::OPENCV::UUID::OVERLAYSBS);
    SRef<display::I3DOverlay> overlay3DComponent = opencvModule.createComponent<display::I3DOverlay>(MODULES::OPENCV::UUID::OVERLAY3D);
    SRef<geom::IImage2WorldMapper> img_mapper = toolsModule.createComponent<geom::IImage2WorldMapper>(MODULES::TOOLS::UUID::IMAGE2WORLD_MAPPER);
    SRef<geom::I2DTransform> transform2D = toolsModule.createComponent<geom::I2DTransform>(MODULES::TOOLS::UUID::TRANSFORM2D);

	std::cout<<MODULES::OPENCV::UUID::DESCRIPTORS_EXTRACTOR_SIFT<<std::endl;
   	SRef<features::IDescriptorsExtractor> descriptorExtractor = opencvNonFreeModule.createComponent<features::IDescriptorsExtractor>(MODULES::OPENCV::UUID::DESCRIPTORS_EXTRACTOR_SIFT);


    /* in dynamic mode, we need to check that components are well created*/
    /* this is needed in dynamic mode */
    if (!camera || !imageViewer || !marker || !kpDetector || !descriptorExtractor || !matcher || !homographyEstimation || !homographyValidation ||
        !keypointsReindexer || !poseEstimation || !overlay2DComponent || !overlaySBSComponent || !overlay3DComponent || !img_mapper || !transform2D )
    {
        LOG_ERROR("One or more component creations have failed");
        return -1;
    }
    LOG_INFO("All components have been created");

	std::cout<<"This is the end..."<<std::endl;
	// 
	//
	// end of the common code (simple,compile-time, run-time)
     return 0;
}
