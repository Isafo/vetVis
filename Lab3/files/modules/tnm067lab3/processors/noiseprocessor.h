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

#ifndef IVW_NOISEPROCESSOR_H
#define IVW_NOISEPROCESSOR_H

#include <modules/tnm067lab3/tnm067lab3moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/boolproperty.h>

#include <random>
#include <inviwo/core/properties/minmaxproperty.h>


namespace inviwo {

    class ImageRAM;

class IVW_MODULE_TNM067LAB3_API NoiseProcessor : public Processor { 
    enum class NoiseType {
        Random,
        Perlin
    };
public:
    InviwoProcessorInfo();
    NoiseProcessor();
    virtual ~NoiseProcessor();

    virtual void process();



protected:
    ImageOutport noise_;

    IntVec2Property size_;
    TemplateOptionProperty<NoiseType> type_;
    IntMinMaxProperty levels_;
    FloatProperty persistence_;

    CompositeProperty randomness_;
    BoolProperty useSameSeed_;
    IntProperty seed_;
private:
    void randomNoise(Image *img, float minv, float maxv); 
    void randomNoiseSingle(Image *img, float minv, float maxv);
    void perlinNoise(Image *img);


    std::random_device rd_;
    std::mt19937 mt_;

};

} // namespace

#endif // IVW_NOISEPROCESSOR_H

