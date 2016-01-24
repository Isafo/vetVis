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
	float sampleRate = 1/(length(exitPoint*volumeParameters.dimensions - entryPoint*volumeParameters.dimensions)*samplingRate);	


	if (entryPoint == exitPoint) discard;

	vec4 ISOColor = vec4(1.0, 0.0, 0.0, 1.0);
	
	// Perfrom the ISO raycasting calculation
	ISOColor = vec4(0.0, 0.0, 0.0, 0.0);

	// Calculate the ray
	vec3 rayPosition = entryPoint;
	vec3 lastPos = rayPosition;
	vec3 rayDir = normalize(exitPoint - entryPoint);
	float nsample = 0;
	vec4 vSample = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 lastValue = vSample;

	vec3 pos = vec3(0.0, 0.0, 0.0);
	vec4 preciseISO = vec4(0.0, 0.0, 0.0, 0.0); 
	vec4 t = vec4(0.0, 0.0, 0.0, 0.0); 

	while(nsample < length(exitPoint - entryPoint)) {
		
		//classify using TransferFunction
		vSample =  getNormalizedVoxel(volume, volumeParameters, rayPosition);
		if (vSample.x > isoValue) {
			
			t = (isoValue - lastValue) / (vSample - lastValue);
			pos = (t.x*lastPos + (1-t.x)*rayPosition);

			preciseISO = getNormalizedVoxel(volume, volumeParameters, pos);

			ISOColor = preciseISO;// vec4(preciseISO.x, preciseISO.x, preciseISO.x, preciseISO.x);
			ISOColor = vec4(pos,1.0);
			break;
		}

		// step in the ray direction
		lastValue = vSample;
		lastPos = rayPosition;
		rayPosition = rayPosition + (rayDir * sampleRate);
		nsample = nsample + sampleRate;
	}



	vec3 samplePos; //sample positions inbetween entry and exit points
	vec4 voxel = getNormalizedVoxel(volume, volumeParameters, samplePos);	



	FragData0 = ISOColor;
    PickingData = texture(entryPicking, texCoords);	
    gl_FragDepth = texture(entryDepth, texCoords).x;
}
