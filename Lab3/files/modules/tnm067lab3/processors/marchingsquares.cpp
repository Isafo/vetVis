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

#include "marchingsquares.h"
#include <inviwo/core/datastructures/geometry/simplemesh.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/util/interpolation.h>
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/image.h>

namespace inviwo {

	int calcBin(int b[4]);
	bool calcAvg(double val[4], float threshold);
	vec2 evalCoord(int point, int x, int y, size2_t dim);

	int caseTable[16][8] = {{ -1, -1, -1, -1, -1, -1, -1, -1 },
							{ 3, 2, 3, 0, -1, -1, -1, -1 },		// case 1
							{ 2, 1, 2, 3, -1, -1, -1, -1 },
							{ 3, 0, 2, 1, -1, -1, -1, -1, },		// case 3
							{ 1, 0, 1, 2, -1, -1, -1, -1 },
							{ 1, 0, 1, 2, 3, 2, 3, 0 },			// case 5
							{ 1, 0, 2, 3, -1, -1, -1, -1 },
							{ 1, 0, 3, 0, -1, -1, -1, -1 },		// case 7
							{ 0, 1, 0, 3, -1, -1, -1, -1 },
							{ 0, 1, 3, 2, -1, -1, -1, -1 },		// case 9
							{ 0, 1, 2, 1, 2, 3, 0, 3 },
							{ 0, 1, 2, 1, -1, -1, -1, -1 },		// case 11
							{ 1, 2, 0, 3, -1, -1, -1, -1 },
							{ 1, 2, 3, 2, -1, -1, -1, -1 },		// case 13
							{ 2, 3, 0, 3, -1, -1, -1, -1, },
							{ -1, -1, -1, -1, -1, -1, -1, -1 }	// case 15
	};

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
ProcessorClassIdentifier(MarchingSquares,  "org.inviwo.MarchingSquares")
ProcessorDisplayName(MarchingSquares,  "MarchingSquares")
ProcessorTags(MarchingSquares, Tags::None);
ProcessorCategory(MarchingSquares, "Undefined");
ProcessorCodeState(MarchingSquares, CODE_STATE_EXPERIMENTAL);

MarchingSquares::MarchingSquares()
    : Processor() 
    , image_("image")
    , lines_("lines")
    , threshold_("threshold","Threshold" , 0.5f , 0.0f , 1.0f )
{
    addPort(image_);
    addPort(lines_);
    
    addProperty(threshold_);
}

MarchingSquares::~MarchingSquares() {
    
}
    
void MarchingSquares::process() {
    auto mesh = std::make_shared<SimpleMesh>(GeometryEnums::LINES, GeometryEnums::NONE);

    auto inImage = image_.getData()->getColorLayer()->getRepresentation<LayerRAM>();
    auto dims = inImage->getDimensions();

    Image binaryImg(dims, DataUINT8::get());
	auto binaryImgData = static_cast<unsigned char*>(binaryImg.getColorLayer()->getEditableRepresentation<LayerRAM>()->getData());

	ivec2 pos;
	std::vector<int> thresholdImg;

	/*vec3 ms(0.0f, 0.0f, -0.1f);
	vec3 me(dims.x, dims.y, -0.1f);
	mesh->addIndex(mesh->addVertex(ms, vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
	mesh->addIndex(mesh->addVertex(me, vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
	lines_.setData(mesh);
	return;*/

	for (int y = 0; y < dims.y; y++)
		for (int x = 0; x < dims.x; x++) {
			pos = ivec2(x, y);
			if (inImage->getValueAsSingleDouble(pos) > threshold_.get())
				thresholdImg.push_back(1);
			else
				thresholdImg.push_back(0);
		}
	
	//ivec2 dims_scaling(1.0f/dims.x, 1.0f/dims.y);
	ivec2 dims_scaling(1.0f, 1.0f);
	//x, x+1, dims.x*y+x, dims.x*y+x+1
	int b[4];
	double val[4], tempVal[4];
	vec2 coord1, coord2;
	int point1, point2;
	vec2 midPoint[4];
	for (int y = 0; y < dims.y - 1; y++)
		for (int x = 0; x < dims.x - 1; x++) {
			b[0] = thresholdImg.at(dims.x*y + x);
			b[1] = thresholdImg.at(dims.x*y + x + 1);
			b[2] = thresholdImg.at(dims.x*(y + 1) + x + 1);
			b[3] = thresholdImg.at(dims.x*(y + 1) + x);

			// nedan blir fel
			val[0] = inImage->getValueAsSingleDouble(ivec2(x, y));
			val[1] = inImage->getValueAsSingleDouble(ivec2(x + 1, y));
			val[2] = inImage->getValueAsSingleDouble(ivec2(x + 1, y + 1));
			val[3] = inImage->getValueAsSingleDouble(ivec2(x, y + 1));

			// temp ges i R = [0], G = [1], B = [2], A = [3], för första punkten ara punkten 4,5,6,7....
			/*const unsigned char*  temp = static_cast<const unsigned char*> (inImage->getData());
			val[0] = temp[dims.x * y + x] / 255.0;
			val[1] = temp[dims.x * y + (x + 4)] / 255.0;
			val[2] = temp[dims.x * (y + 4) + (x + 4)] / 255.0;
			val[3] = temp[dims.x * (y + 4) + x] / 255.0;*/

			auto c = caseTable[calcBin(b)];
			int i;
			int j = 0;
			for (i = 0;  i < 8; i += 2) {
				if (c[i] == -1)
					break;

				// fetch first 2 points in the case
				point1 = c[i];
				point2 = c[i+1];

				// find the coordinates
				coord1 = evalCoord(point1, x, y, dims);
				coord2 = evalCoord(point2, x, y, dims);

				// interpolate
				float t = (threshold_.get() - val[point1]) / (val[point2] - val[point1]);
				
				midPoint[j] = (t*coord1 + (1-t)*coord2);

				j++;
			}

			// check special case
			if (i > 6) {
				if (!calcAvg(val, threshold_.get())) {
					// bindas med andra två punkter över 1
					if (b[0] == 1) {
						mesh->addIndex(mesh->addVertex(vec3(midPoint[0].x * dims_scaling.x, midPoint[0].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[1].x * dims_scaling.x, midPoint[1].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[2].x * dims_scaling.x, midPoint[2].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[3].x * dims_scaling.x, midPoint[3].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));

					}
					else {
						mesh->addIndex(mesh->addVertex(vec3(midPoint[0].x * dims_scaling.x, midPoint[0].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[3].x * dims_scaling.x, midPoint[3].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[1].x * dims_scaling.x, midPoint[1].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[2].x * dims_scaling.x, midPoint[2].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
					}
				}
				else {
					// bindas med andra två punkter under 1
					if (b[0] == 1) {
						mesh->addIndex(mesh->addVertex(vec3(midPoint[0].x * dims_scaling.x, midPoint[0].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[3].x * dims_scaling.x, midPoint[3].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[1].x * dims_scaling.x, midPoint[1].y * dims_scaling.y, - 0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[2].x * dims_scaling.x, midPoint[2].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));

						//mesh->addIndex(mesh->addVertex(vec3(midPoint[0], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[3], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[1], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[2], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
					}
					else {
						mesh->addIndex(mesh->addVertex(vec3(midPoint[0].x * dims_scaling.x, midPoint[0].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[1].x * dims_scaling.x, midPoint[1].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[2].x * dims_scaling.x, midPoint[2].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						mesh->addIndex(mesh->addVertex(vec3(midPoint[3].x * dims_scaling.x, midPoint[3].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));

						//mesh->addIndex(mesh->addVertex(vec3(midPoint[0], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[1], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[2], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
						//mesh->addIndex(mesh->addVertex(vec3(midPoint[3], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
					}
				}
			}
			// no special case
			else if (i > 1) {
				mesh->addIndex(mesh->addVertex(vec3(midPoint[0].x * dims_scaling.x, midPoint[0].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
				mesh->addIndex(mesh->addVertex(vec3(midPoint[1].x * dims_scaling.x, midPoint[1].y * dims_scaling.y, -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));

				//std::cout << midPoint[0].x << " " << midPoint[0].y << " " << std::endl;
				//std::cout << midPoint[1].x << " " << midPoint[1].y << " " << std::endl;
				//mesh->addIndex(mesh->addVertex(vec3(midPoint[0], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
				//mesh->addIndex(mesh->addVertex(vec3(midPoint[1], -0.1f), vec3(0.9, 0.9, 0), vec4(0.0, 0.0, 1.0, 1.0)));
			}
		}
    lines_.setData(mesh);
}

bool calcAvg(double val[4], float threshold) {
	return ((val[0] + val[1] + val[2] + val[3]) / 4) > threshold;
}

vec2 evalCoord(int point, int x, int y, size2_t dim) {
	vec2 coord;
	if (point == 0) {			// 0
		coord = vec2(x, y);
	}
	else if (point == 1) {		// 1
		coord = vec2(x+1, y);
	}
	else if (point == 2) {		// 2
		coord = vec2(x+1, y+1);
	}
	else if (point == 3) {		// 3
		coord = vec2(x, y+1);
	}
	coord.x = coord.x / dim.x;
	coord.y = coord.y / dim.y;
	return coord;
}

int calcBin(int b[4]) {
	return (b[0] * 8 + b[1] * 4 + b[2] * 2 + b[3] * 1);
}

} // namespace

