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

#ifndef SOLARMODULEMANAGERNONFREEOPENCV_H
#define SOLARMODULEMANAGERNONFREEOPENCV_H

#include "IComponentManager.h"

#include "api/display/I2DOverlay.h"
#include "api/display/I3DOverlay.h"
#include "api/display/IImageViewer.h"
#include "api/display/ISideBySideOverlay.h"

#include "api/features/IContoursExtractor.h"
#include "api/features/IContoursFilter.h"
#include "api/features/IDescriptorMatcher.h"
#include "api/features/IDescriptorsExtractor.h"
#include "api/features/IDescriptorsExtractorSBPattern.h"
#include "api/features/IKeypointDetector.h"
#include "api/features/ISBPatternReIndexer.h"
#include "api/features/IKeypointsReIndexer.h"

#include "api/geom/I2DTransform.h"
#include "api/geom/I3DTransform.h"
#include "api/geom/IImage2WorldMapper.h"

#include "api/input/devices/ICamera.h"
#include "api/input/devices/ICameraCalibration.h"
#include "api/input/files/IMarker2DNaturalImage.h"
#include "api/input/files/IMarker2DSquaredBinary.h"

#include "api/solver/pose/IHomographyEstimation.h"
#include "api/solver/pose/IHomographyValidation.h"

#include "api/image/IImageConvertor.h"
#include "api/image/IImageFilter.h"
#include "api/image/IImageLoader.h"
#include "api/image/IPerspectiveController.h"


#include "api/solver/pose/IPoseEstimation.h"

#include "SolAROpencvNonFreeAPI.h"

using namespace std;

namespace xpcf  = org::bcom::xpcf;
namespace SolAR {
namespace MODULES {
namespace NONFREEOPENCV {
namespace UUID{

// declaration of components UUIDs
const string DESCRIPTORS_EXTRACTOR_SIFT="3787eaa6-d0a0-11e7-8fab-cec278b6b50a";
const string DESCRIPTORS_EXTRACTOR_SURF128="fe14a310-d0a2-11e7-8fab-cec278b6b50a";
const string DESCRIPTORS_EXTRACTOR_SURF64="1a437804-d0a3-11e7-8fab-cec278b6b50a";
const string KEYPOINT_DETECTOR_NONFREEOPENCV="d1f9317c-9519-4671-8ff5-4629773544f2";


}  // End namespace UUID

using namespace SolAR;

// class SolARComponentManagerOpencv declaration
class SOLAROPENCVNONFREE_EXPORT_API SolARModuleManagerOpencvNonFree
{
public:
    SolARModuleManagerOpencvNonFree();
    SolARModuleManagerOpencvNonFree(const char *iniFile);
    ~SolARModuleManagerOpencvNonFree() = default;

    template <class T>
    int createComponent(string uuid, SRef<T> &compRef);

    template <class T>
    SRef<T> createComponent(string uuid);

    inline bool isLoaded() const {return loaded;}

protected:
    SRef<xpcf::IComponentManager> m_xpcfComponentManager;

private:
    bool loaded;
};

template <class T>
SRef<T> SolARModuleManagerOpencvNonFree::createComponent(string uuid)
{
    SRef<T> output;
    if (createComponent(uuid, output))
        return output;
    else
        return nullptr;
}

template <class T>
int SolARModuleManagerOpencvNonFree::createComponent(string uuid, SRef<T> &compRef)
{
    boost::uuids::string_generator gen;
    
    int res;
     if ( uuid == UUID::DESCRIPTORS_EXTRACTOR_SIFT || uuid == UUID::DESCRIPTORS_EXTRACTOR_SURF128 || uuid == UUID::DESCRIPTORS_EXTRACTOR_SURF64) // keypoint descriptors extractors component
    {

        res=m_xpcfComponentManager->createComponent(gen(uuid), gen(api::features::IDescriptorsExtractor::UUID), compRef);
        if (res == -1)
             LOG_ERROR("ORB descriptors extractor component creation has failed");
        return res;
    }

    return -1;
}
}  // End namespace OPENCV
}  // End namespace MODULES
}  // End namespace SolAR

#endif // SOLARMODULEMANAGEROPENCV_H
