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

    ValueTreeLabelAttachment.h
    Created: 4 Jun 2016 11:17:03am
    Author:  Oli Larkin / Daniel Walz

  ==============================================================================
*/

#ifndef VALUETREELABELATTACHMENT_H_INCLUDED
#define VALUETREELABELATTACHMENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ValueTreeLabelAttachment : public juce::Label::Listener,
                                public juce::ValueTree::Listener
{
public:
    ValueTreeLabelAttachment (juce::ValueTree& _tree,
                               juce::Label* _label,
                               juce::Identifier _property,
                               juce::UndoManager* _undoMgr = nullptr)
    :   tree (_tree),
        property (_property),
        undoMgr (_undoMgr),
        updating (false)
    {
        // Don't attach an invalid valuetree!
        jassert (tree.isValid());
        label = _label;

        if (tree.hasProperty (property)) {
            label->setText (tree.getProperty(property), juce::dontSendNotification);
        }
        else {
            tree.setProperty (property, label->getText(), undoMgr);
        }

        tree.addListener (this);
        label->addListener (this);
    }

    ~ValueTreeLabelAttachment ()
    {
        tree.removeListener (this);
        if (label) {
            label->removeListener (this);
        }
    }

    void labelTextChanged (juce::Label *_label) override
    {
        if (! updating) {
            updating = true;
            if (label == _label) {
                tree.setProperty (property, label->getText(), undoMgr);
            }
            updating = false;
        }
    }

    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
    {
        if (! updating) {
            updating = true;
            if (treeWhosePropertyHasChanged == tree && label) {
                if (_property == property) {
                    label->setText(tree.getProperty (property), juce::dontSendNotification);
                }
            }

            updating = false;
        }
    }
    
    void valueTreeChildAdded (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {}
    void valueTreeChildRemoved (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
    void valueTreeChildOrderChanged (juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (juce::ValueTree &treeWhoseParentHasChanged) override {}
    void valueTreeRedirected (juce::ValueTree &treeWhichHasBeenChanged) override {}

private:
    juce::ValueTree                             tree;
    juce::Component::SafePointer<juce::Label>   label;
    juce::Identifier                            property;
    juce::UndoManager*                          undoMgr;
    bool                                        updating;
};



#endif  // VALUETREELABELATTACHMENT_H_INCLUDED