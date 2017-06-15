/*
 ==============================================================================
 
 Copyright (c) 2016, Daniel Walz
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software without
 specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.
 
 ==============================================================================
 
 BEGIN_JUCE_MODULE_DECLARATION
 
 ID:            ff_gui_attachments
 vendor:        Foleys Finest Audio UG
 version:       0.9.1
 name:          Attachment classes to connect ValueTree and GUI components
 description:   helpers to get geometric information about channels
 dependencies:  juce_core, juce_gui_basics, juce_data_structures
 website:       https://foleysfinest.com/
 license:       BSD 3-clause
 
 END_JUCE_MODULE_DECLARATION
 
 ==============================================================================
 */

#pragma once

/**
 \mainpage
 
 In this module some additions to JUCE (www.juce.com) are provided to attach
 a Slider, ComboBox or RadioButtonGroup to a leaf in a ValueTree.
 This is especially useful to keep GUI components in sync with a ValueTree,
 e.g. th tree behind AudioProcessorValueTreeState. So you can store values
 that are not exposed to the host as parameters.
 
 \see ValueTreeSliderAttachment, ValueTreeComboBoxAttachment, ValueTreeRadioButtonGroupAttachment, ValueTreeLabelAttachment
 
 They are used exatly the same as AudioProcessorValueTree::SliderAttachment.
 In the ValueTreeSliderAttachment you can also supply a range for the slider.
 
 Have fun...
 Daniel
 
 */


#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace FF {
    static juce::Identifier propSelected        ("selected");
    static juce::Identifier propMinimumDefault  ("minimum");
    static juce::Identifier propMaximumDefault  ("maximum");
    static juce::Identifier propIntervalDefault ("interval");
};

#include "ValueTreeSliderAttachment.h"
#include "ValueTreeComboBoxAttachment.h"
#include "ValueTreeRadioButtonGroupAttachment.h"
#include "ValueTreeLabelAttachment.h"
#include "ValueTreeDebugListener.h"

