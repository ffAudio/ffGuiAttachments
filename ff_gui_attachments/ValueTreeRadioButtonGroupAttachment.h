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
 */

/*
  ==============================================================================

    ValueTreeRadioButtonGroupAttachment.h
    Created: 4 Jun 2016 5:00:44pm
    Author:  Daniel Walz

  ==============================================================================
*/

#ifndef VALUETREERADIOBUTTONGROUPATTACHMENT_H_INCLUDED
#define VALUETREERADIOBUTTONGROUPATTACHMENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ValueTreeRadioButtonGroupAttachment : public juce::ValueTree::Listener,
                                            public juce::Button::Listener
{
public:
    ValueTreeRadioButtonGroupAttachment (juce::ValueTree& _tree,
                                         juce::Array<juce::Button*> _buttons,
                                         juce::Identifier& _property,
                                         bool _selectSubNodes)
    {

        for (int i=0; i < _buttons.size(); ++i) {
            juce::Button* b = _buttons.getUnchecked (i);
            if (! buttons.contains (b)) {
                buttons.add (b);
                b->addListener (this);
            }
        }

        tree.addListener (this);
    }

    void buttonStateChanged (juce::Button *buttonThatHasChanged)
    {

    }

private:
    juce::ValueTree tree;
    juce::Array<juce::Component::SafePointer<juce::Button> > buttons;
    juce::Identifier property;

};


#endif  // VALUETREERADIOBUTTONGROUPATTACHMENT_H_INCLUDED
