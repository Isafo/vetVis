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

#include <modules/tnm067lab3/tnm067lab3module.h>
#include <modules/tnm067lab3/processors/marchingsquares.h>
#include <modules/tnm067lab3/processors/mesh2drenderer.h>
#include <modules/tnm067lab3/processors/noiseprocessor.h>
#include <modules/tnm067lab3/processors/caseimages.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo {

TNM067Lab3Module::TNM067Lab3Module() : InviwoModule() {
    setIdentifier("TNM067Lab3");
    
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(InviwoApplication::PATH_MODULES, "tnm067lab3/glsl");
    
    // Register objects that can be shared with the rest of inviwo here:
    
    // Processors
    registerProcessor(MarchingSquares);
    registerProcessor(Mesh2DRenderer);
    registerProcessor(NoiseProcessor);
    registerProcessor(CaseImages);
    
    // Properties
    // registerProperty(TNM067Lab3Property);
    
    // Readers and writes
    // registerDataReader(new TNM067Lab3Reader());
    // registerDataWriter(new TNM067Lab3Writer());
    
    // Data converters
    // registerRepresentationConverter(new TNM067Lab3Disk2RAMConverter());

    // Ports
    // registerPort(TNM067Lab3Outport);
    // registerPort(TNM067Lab3Inport);

    // PropertyWidgets
    // registerPropertyWidget(TNM067Lab3PropertyWidgetQt, TNM067Lab3Property, "Default");
    
    // Dialogs
    // registerDialog("tnm067lab3", TNM067Lab3DialogQt);
    
    // Other varius things
    // registerCapabilities(Capabilities* info);
    // registerData(Data* data);
    // registerDataRepresentation(DataRepresentation* dataRepresentation);
    // registerSettings(new SystemSettings());
    // registerMetaData(MetaData* meta);   
    // registerPortInspector(PortInspector* portInspector);
    // registerProcessorWidget(std::string processorClassName, ProcessorWidget* processorWidget);
    // registerDrawer(GeometryDrawer* renderer);
    // registerResource(Resource* resource);    
}

} // namespace
