/**
 * @copyright Copyright (c) 2017 B-com http://www.b-com.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xpcf/xpcf.h"

#include "SolARModuleOpencv_traits.h"
#include "SolARModuleNonFreeOpencv_traits.h"

#include "api/image/IImageLoader.h"
#include "api/display/IImageViewer.h"
#include "api/display/I2DOverlay.h"
#include "api/features/IDescriptorMatcher.h"
#include "api/features/IKeypointDetector.h"
#include "api/features/IDescriptorsExtractor.h"
#include "api/display/ISideBySideOverlay.h"

#include <iostream>

#include <string>
#include <vector>

using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;

namespace xpcf  = org::bcom::xpcf;

int run(int argc,char** argv)
{

    // load libraries
    SRef<xpcf::IComponentManager> xpcfComponentManagerOpenCV = xpcf::getComponentManagerInstance();
    org::bcom::xpcf::XPCFErrorCode returnErrCode = xpcfComponentManagerOpenCV->load("$BCOMDEVROOT/.xpcf/SolAR/xpcf_SolARModuleOpenCV_registry.xml");
    // instantiate module managers
   /*
    if (!xpcfComponentManagerOpenCV->isLoaded()) // xpcf library load has failed
    {
        LOG_ERROR("SolARModuleOpenCV library load has failed")
        return -1;
    }
    */
    SRef<xpcf::IComponentManager> xpcfComponentManagerNonFreeOpenCV = xpcf::getComponentManagerInstance();
   
    org::bcom::xpcf::XPCFErrorCode returnErrCode2 = xpcfComponentManagerNonFreeOpenCV->load("$BCOMDEVROOT/.xpcf/SolAR/xpcf_SolARModuleNonFreeOpenCV_registry.xml");
    
    // instantiate module managers
    
    /*
    if (!xpcfComponentManagerNonFreeOpenCV->isLoaded()) // xpcf library load has failed
    {
        LOG_ERROR("SolARModuleNonFreeOpenCV library load has failed")
        return -1;
    }
    */


 // declarations and creation of components
    SRef<image::IImageLoader> imageLoader1 = xpcfComponentManagerOpenCV->create<SolAR::MODULES::OPENCV::SolARImageLoaderOpencv>()->bindTo<image::IImageLoader>();
    SRef<image::IImageLoader> imageLoader2 = xpcfComponentManagerOpenCV->create<SolAR::MODULES::OPENCV::SolARImageLoaderOpencv>()->bindTo<image::IImageLoader>();
    SRef<features::IDescriptorMatcher> matcher = xpcfComponentManagerOpenCV->create<SolAR::MODULES::OPENCV::SolARDescriptorMatcherKNNOpencv>()->bindTo<features::IDescriptorMatcher>();
    SRef<display::IImageViewer> viewer = xpcfComponentManagerOpenCV->create<SolAR::MODULES::OPENCV::SolARImageViewerOpencv>()->bindTo<display::IImageViewer>();
    SRef<display::ISideBySideOverlay> overlay = xpcfComponentManagerOpenCV->create<SolAR::MODULES::OPENCV::SolARSideBySideOverlayOpencv>()->bindTo<display::ISideBySideOverlay>();

    SRef<features::IKeypointDetector> keypointsDetector = xpcfComponentManagerNonFreeOpenCV->create<SolAR::MODULES::NONFREEOPENCV::SolARKeypointDetectorNonFreeOpencv>()->bindTo<features::IKeypointDetector>();
    SRef<features::IDescriptorsExtractor> extractorSIFT = xpcfComponentManagerNonFreeOpenCV->create<SolAR::MODULES::NONFREEOPENCV::SolARDescriptorsExtractorSIFTOpencv>()->bindTo<features::IDescriptorsExtractor>();

    if (!imageLoader1 || !imageLoader2 || !keypointsDetector || !extractorSIFT || !matcher || !viewer || !overlay)
    {
        LOG_ERROR("One or more component creations have failed");
        return -1;
    }
 
 
    SRef<Image>                                        image1;
    SRef<Image>                                        image2;
    std::vector< SRef<Keypoint>>        keypoints1;
    std::vector< SRef<Keypoint>>        keypoints2;
    SRef<DescriptorBuffer>                             descriptors1;
    SRef<DescriptorBuffer>                             descriptors2;
    std::vector<DescriptorMatch>                       matches;
    std::vector<SRef<Point2Df>>                        matchedKeypoints1;
    std::vector<SRef<Point2Df>>                        matchedKeypoints2;
    SRef<Image>                                        viewerImage;

    // The escape key to exit the sample
    char escape_key = 27;

 // components initialisation
    // nothing to do

 // Start
    // Load the first image
    imageLoader1->bindTo<xpcf::IConfigurable>()->getProperty("filePath")->setStringValue(argv[1],0);
    if (imageLoader1->getImage(image1) != FrameworkReturnCode::_SUCCESS)
    {
       LOG_ERROR("Cannot load image with path {}", argv[1]);
       return -1;
    }

    // Load the second image
    imageLoader1->bindTo<xpcf::IConfigurable>()->getProperty("filePath")->setStringValue(argv[2],0);
    if (imageLoader2->getImage(image2) != FrameworkReturnCode::_SUCCESS)
    {
       LOG_ERROR("Cannot load image with path {}", argv[1]);
       return -1;
    }    
  
    // Detect the keypoints of the first image
    keypointsDetector->detect(image1, keypoints1);

    // Detect the keypoints of the second image
    keypointsDetector->detect(image2, keypoints2);

    // Compute the SIFT descriptor for each keypoint extracted from the first image
    extractorSIFT->extract(image1, keypoints1, descriptors1);

    // Compute the SIFT descriptor for each keypoint extracted from the second image
    extractorSIFT->extract(image2, keypoints2, descriptors2);

    // Compute the matches between the keypoints of the first image and the keypoints of the second image
    matcher->match(descriptors1, descriptors2, matches);

    // Reorder the keypoints that match in to vector of 2D point (nth point of first vector matching with nth point of the second vector)
    // For OpenCV module testing. A keypoint reindexer component is available in the module "Tools"
    matchedKeypoints1.clear();
    matchedKeypoints2.clear();

    for(unsigned int i = 0; i < matches.size(); i++ )
    {
        matchedKeypoints1.push_back(xpcf::utils::make_shared<Point2Df>(keypoints1[ matches[i].getIndexInDescriptorA()]->getX(),keypoints1[ matches[i].getIndexInDescriptorA()]->getY()));
        matchedKeypoints2.push_back(xpcf::utils::make_shared<Point2Df>(keypoints2[ matches[i].getIndexInDescriptorB()]->getX(),keypoints2[ matches[i].getIndexInDescriptorB()]->getY()));
    }

    // Draw the matches in a dedicated image
    overlay->drawMatchesLines(image1, image2, viewerImage, matchedKeypoints1, matchedKeypoints2);

    bool proceed = true;
    viewer->bindTo<xpcf::IConfigurable>()->getProperty("title")->setStringValue("show matches");
    viewer->bindTo<xpcf::IConfigurable>()->getProperty("exitKey")->setIntegerValue(27);     
    while (proceed)
    {
        // Display the image with matches in a viewer. If escape key is pressed, exit the loop.
        if (viewer->display(viewerImage) == FrameworkReturnCode::_STOP)
        {
            proceed = false;
            LOG_INFO("End of DescriptorMatcherOpenCVStaticTest");
        }
    }
    
    return 0;
}


int printHelp(){
        printf(" usage :\n");
        printf(" exe firstImagePath secondImagePath configFilePath\n");
        return 1;
}


int main(int argc, char **argv){
    if(argc == 4){
        run(argc,argv);
         return 1;
    }
    else
        return(printHelp());
}





