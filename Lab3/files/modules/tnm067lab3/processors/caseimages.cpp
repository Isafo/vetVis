/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include "caseimages.h"

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
ProcessorClassIdentifier(CaseImages,  "org.inviwo.CaseImages")
ProcessorDisplayName(CaseImages,  "CaseImages")
ProcessorTags(CaseImages, Tags::None);
ProcessorCategory(CaseImages, "Undefined");
ProcessorCodeState(CaseImages, CODE_STATE_EXPERIMENTAL);

CaseImages::CaseImages()
    : Processor() 
    , image_("image_", DataVec4UINT8::get())
    , case_("case","Case" , 1 , 0 , 15)
{
    addPort(image_);
    addProperty(case_);
}

CaseImages::~CaseImages() {
    
}
    
void CaseImages::process() {
    auto img = std::make_shared<Image>(size2_t(2, 2), DataVec4UINT8::get());
    auto data = static_cast<glm::u8vec4*>(img->getColorLayer()->getEditableRepresentation<LayerRAM>()->getData());
    auto C = case_.get();
    bool d = C >= 8;
    if (d)
        C -= 8;
    bool c = C >= 4;
    if (c)
        C -= 4;
    bool b = C >= 2;
    if (b)
        C -= 2;
    bool a = C >= 1;
    data[0] = glm::u8vec4(glm::u8vec3(a ? 255 : 0), 255);
    data[1] = glm::u8vec4(glm::u8vec3(b ? 255 : 0), 255);
    data[2] = glm::u8vec4(glm::u8vec3(d ? 255 : 0), 255);
    data[3] = glm::u8vec4(glm::u8vec3(c ? 255 : 0), 255);

    image_.setData(img);

}

} // namespace

