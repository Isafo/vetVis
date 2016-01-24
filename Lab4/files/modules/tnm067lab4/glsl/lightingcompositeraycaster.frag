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

#include "utils/structs.glsl"
#include "utils/sampler2d.glsl"
#include "utils/sampler3d.glsl"

#include "utils/raycastgeometry.glsl"

uniform VolumeParameters volumeParameters;
uniform sampler3D volume;

uniform sampler2D transferFunction;

uniform ImageParameters entryParameters;
uniform sampler2D entryColor;
uniform sampler2D entryDepth;
uniform sampler2D entryPicking;

uniform ImageParameters exitParameters;
uniform sampler2D exitColor;
uniform sampler2D exitDepth;

uniform ImageParameters outportParameters;

uniform LightParameters lighting;
uniform CameraParameters camera;
uniform float samplingRate;
uniform float isoValue;

uniform int channel;
		  
void main() {
	vec2 texCoords = gl_FragCoord.xy * outportParameters.reciprocalDimensions;
	vec3 entryPoint = texture(entryColor, texCoords).rgb;
	vec3 exitPoint = texture(exitColor, texCoords).rgb;
	float sampleRate = samplingRate;

	if (entryPoint == exitPoint) discard;

	vec4 LCompositeColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	// Perfrom the Compositing 
	LCompositeColor = vec4(entryPoint, 1.0);

	vec3 samplePos; //sample positions inbetween entry and exit points
	vec4 voxel = getNormalizedVoxel(volume, volumeParameters, samplePos);	

	//classify using TransferFunction

	FragData0 = LCompositeColor;
    PickingData = texture(entryPicking, texCoords);
	gl_FragDepth = gl_FragDepth = texture(entryDepth, texCoords).x;
	//as
}
