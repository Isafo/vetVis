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

#include "mesh2drenderer.h"

#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/meshdrawerfactory.h>
#include <inviwo/core/processors/processor.h>

#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/rendering/meshdrawergl.h>
#include <modules/opengl/shader/shader.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/openglutils.h>
#include <inviwo/core/datastructures/coordinatetransformer.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
ProcessorClassIdentifier(Mesh2DRenderer,  "org.inviwo.Mesh2DRenderer")
ProcessorDisplayName(Mesh2DRenderer,  "2D Mesh Renderer")
ProcessorTags(Mesh2DRenderer, Tags::GL);
ProcessorCategory(Mesh2DRenderer, "Geometry Rendering");
ProcessorCodeState(Mesh2DRenderer, CODE_STATE_EXPERIMENTAL);

Mesh2DRenderer::Mesh2DRenderer()
    : Processor()
    , inport_("geometry.inport")
    , imageInport_("imageInport")
    , outport_("image.outport")
    , shader_("mesh2drendering.vert", "mesh2drendering.frag")
{
    addPort(inport_);
    addPort(imageInport_);
    addPort(outport_);

    inport_.onChange(this, &Mesh2DRenderer::updateDrawers);
}

Mesh2DRenderer::~Mesh2DRenderer() {
    
}
    
void Mesh2DRenderer::process() {
    if (imageInport_.isConnected()) {
        utilgl::activateTargetAndCopySource(outport_, imageInport_ , COLOR_ONLY);
    }
    else {
        utilgl::activateAndClearTarget(outport_, COLOR_ONLY);
    }
    shader_.activate();

    mat4 proj = glm::ortho(-0.0f, 1.0f, -0.0f, 1.0f, -200.0f, 100.0f);
    shader_.setUniform("projectionMatrix", proj);

    auto a = proj * vec4(0, 0, 0, 1);
    auto b = proj * vec4(1, 1, 0, 1);
    auto c = proj * vec4(100, 100, 0, 1);


    //utilgl::GlBoolState depthTest(GL_DEPTH_TEST, true);
    utilgl::PolygonModeState polygon(GL_LINES, 1 , 1);

    for (auto& drawer : drawers_) {
        utilgl::setShaderUniforms(shader_, *(drawer.second->getGeometry()), "geometry_");
        drawer.second->draw();
    }

    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

bool Mesh2DRenderer::isReady() const
{
    if (imageInport_.isConnected()) {
        return Processor::isReady();
    }
    else {
        return inport_.isReady();
    }
}


void Mesh2DRenderer::updateDrawers()
{
    auto changed = inport_.getChangedOutports();
    DrawerMap temp;
    std::swap(temp, drawers_);

    std::map<const Outport*, std::vector<std::shared_ptr<const Mesh>>> data;
    for (auto& elem : inport_.getSourceVectorData()) {
        data[elem.first].push_back(elem.second);
    }

    for (auto elem : data) {
        auto ibegin = temp.lower_bound(elem.first);
        auto iend = temp.upper_bound(elem.first);

        if (util::contains(changed, elem.first) || ibegin == temp.end() ||
            static_cast<long>(elem.second.size()) != std::distance(ibegin, iend)) {  // data is changed or new.

            for (auto geo : elem.second) {
                if (auto renderer = MeshDrawerFactory::getPtr()->create(geo.get())) {
                    drawers_.emplace(std::make_pair(elem.first, std::move(renderer)));
                }
            }
        }
        else {  // reuse the old data.
            drawers_.insert(std::make_move_iterator(ibegin), std::make_move_iterator(iend));
        }
    }
}

} // namespace

