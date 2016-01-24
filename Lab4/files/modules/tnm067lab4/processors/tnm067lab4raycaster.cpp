/*********************************************************************************
*
* Inviwo - Interactive Visualization Workshop
*
* Copyright (c) 2014-2015 Inviwo Foundation
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

#include "tnm067lab4raycaster.h"
#include <inviwo/core/io/serialization/versionconverter.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/volume/volumeutils.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

ProcessorClassIdentifier(TNM067Lab4Raycaster, "org.inviwo.TNM067Lab4Raycaster");
ProcessorDisplayName(TNM067Lab4Raycaster, "TNM067Lab4 Raycaster");
ProcessorTags(TNM067Lab4Raycaster, Tags::GL);
ProcessorCategory(TNM067Lab4Raycaster, "Volume Rendering");
ProcessorCodeState(TNM067Lab4Raycaster, CODE_STATE_EXPERIMENTAL);

TNM067Lab4Raycaster::TNM067Lab4Raycaster()
	: Processor()
	, shaderLightingCompositing_("lightingcompositeraycaster.frag", true)
	, shaderMIP_("mipraycaster.frag", true)
	, shaderISO_("isoraycaster.frag", true)
	, shaderComposite_("compositeraycaster.frag", true)
	, volumePort_("volume")
	, entryPort_("entry")
	, exitPort_("exit")
	, outport_("outport")
	, transferFunction_("transferFunction", "Transfer function", TransferFunction(), &volumePort_)
	, channel_("channel", "Render Channel")
	, samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
	, isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f)
	, camera_("camera", "Camera")
	, lighting_("lighting", "Lighting", &camera_)
	, mipRayCastingKeyEvent_("mipRayCastingKeyEvent", "MIP Raycasting", new KeyboardEvent('M'),
	new Action(this, &TNM067Lab4Raycaster::switchToMIP))
	, compositeRayCastingKeyEvent_("compositeRayCastingKeyEvent", "Composite Raycasting", new KeyboardEvent('C'),
	new Action(this, &TNM067Lab4Raycaster::switchToComposite))
	, isoRayCastingKeyEvent_("isoRayCastingKeyEvent", "ISO Raycasting", new KeyboardEvent('I'),
	new Action(this, &TNM067Lab4Raycaster::switchToISO))
	, lcompositeRayCastingKeyEvent_("lightedCompositeRayCastingKeyEvent", "Lighting Composite Raycasting", new KeyboardEvent('L'),
	new Action(this, &TNM067Lab4Raycaster::switchToLightingComposite))
	, shaderMode_("mip") {

	/*shaderLightingCompsiting_.onReload([this]() { invalidate(INVALID_RESOURCES); });
	shaderMIP_.onReload([this]() { invalidate(INVALID_RESOURCES); });
	shaderISO_.onReload([this]() { invalidate(INVALID_RESOURCES); });
	shaderComposite_.onReload([this]() { invalidate(INVALID_RESOURCES); });*/

	addPort(volumePort_, "VolumePortGroup");
	addPort(entryPort_, "ImagePortGroup1");
	addPort(exitPort_, "ImagePortGroup1");
	addPort(outport_, "ImagePortGroup1");

	channel_.addOption("Channel 1", "Channel 1", 0);
	channel_.setCurrentStateAsDefault();

	volumePort_.onChange(this, &TNM067Lab4Raycaster::onVolumeChange);

	addProperty(channel_);
	addProperty(transferFunction_);	
	
	addProperty(samplingRate_);
	addProperty(isoValue_);	

	addProperty(camera_);
	addProperty(lighting_);

	addProperty(mipRayCastingKeyEvent_);
	addProperty(compositeRayCastingKeyEvent_);
	addProperty(isoRayCastingKeyEvent_);
	addProperty(lcompositeRayCastingKeyEvent_);
}

TNM067Lab4Raycaster::~TNM067Lab4Raycaster() {}

void TNM067Lab4Raycaster::initializeResources() {
	utilgl::addShaderDefines(shaderLightingCompositing_, lighting_);
	shaderLightingCompositing_.build();

	utilgl::addShaderDefines(shaderMIP_, lighting_);
	shaderMIP_.build();

	utilgl::addShaderDefines(shaderISO_, lighting_);
	shaderISO_.build();

	utilgl::addShaderDefines(shaderComposite_, lighting_);
	shaderComposite_.build();
}

void TNM067Lab4Raycaster::onVolumeChange() {
	if (volumePort_.hasData()) {
		size_t channels = volumePort_.getData()->getDataFormat()->getComponents();

		if (channels == channel_.size()) return;

		std::vector<OptionPropertyIntOption> channelOptions;
		for (size_t i = 0; i < channels; i++) {
			channelOptions.emplace_back("Channel " + toString(i), "Channel " + toString(i),
				static_cast<int>(i));
		}
		channel_.replaceOptions(channelOptions);
		channel_.setCurrentStateAsDefault();
	}
}

void TNM067Lab4Raycaster::process() {

	Shader* shader = nullptr;

	if (shaderMode_ == "mip") {
		shader = &shaderMIP_;
	}
	else if (shaderMode_ == "iso") {
		shader = &shaderISO_;
	}
	else if (shaderMode_ == "composite") {
		shader = &shaderComposite_;
	}
	else {
		shader = &shaderLightingCompositing_;
	}

	if (!shader) {
		return;
	}

	utilgl::activateAndClearTarget(outport_);
	shader->activate();

	TextureUnitContainer units;
	utilgl::bindAndSetUniforms(*shader, units, volumePort_);
	utilgl::bindAndSetUniforms(*shader, units, transferFunction_);
	utilgl::bindAndSetUniforms(*shader, units, entryPort_, COLOR_DEPTH_PICKING);
	utilgl::bindAndSetUniforms(*shader, units, exitPort_, COLOR_DEPTH);
	utilgl::setUniforms(*shader, outport_, camera_, lighting_, samplingRate_, isoValue_, channel_);

	utilgl::singleDrawImagePlaneRect();

	shader->deactivate();
	utilgl::deactivateCurrentTarget();
}

void TNM067Lab4Raycaster::switchToMIP(Event*) {
	shaderMode_ = "mip";
	LogInfo("MIP shader is in use");
	invalidate(INVALID_RESOURCES);
}

void TNM067Lab4Raycaster::switchToComposite(Event*) {
	shaderMode_ = "composite";	
	LogInfo("Composite shader is in use");
	invalidate(INVALID_RESOURCES);
}

void TNM067Lab4Raycaster::switchToISO(Event*) {
	shaderMode_ = "iso";
	LogInfo("ISO shader is in use");
	invalidate(INVALID_RESOURCES);
}

void TNM067Lab4Raycaster::switchToLightingComposite(Event*) {
	shaderMode_ = "lightComposite";
	LogInfo("LightingComposite shader is in use");
	invalidate(INVALID_RESOURCES);
}


}  // namespace
