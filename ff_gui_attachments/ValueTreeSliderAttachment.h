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

    ValueTreeSliderAttachment.h
    Created: 4 Jun 2016 11:17:03am
    Author:  Daniel Walz / Foleys Finest Audio

  ==============================================================================
*/

#pragma once

/**
 \class ValueTreeSliderAttachment
 \brief This class updates a Slider to a property in a ValueTree
 */
class ValueTreeSliderAttachment : public juce::Slider::Listener,
                                  public juce::ValueTree::Listener
{
public:
    /**
     Creates a ValueTreeSliderAttachment. The Slider gets it's values from properties of the ValueTree node.
     You can specify the names of the corresponding properties here.
    */
    ValueTreeSliderAttachment (juce::ValueTree& attachToTree,
                               juce::Slider* _slider,
                               juce::Identifier valueProperty,
                               juce::UndoManager* undoManagerToUse = nullptr,
                               juce::Identifier propertyForMinimum = FF::propMinimumDefault,
                               juce::Identifier propertyForMaximum = FF::propMaximumDefault,
                               juce::Identifier propertyForInterval = FF::propIntervalDefault)
    :   tree     (attachToTree),
        property (valueProperty),
        undoMgr  (undoManagerToUse),
        propMinimum  (propertyForMinimum),
        propMaximum  (propertyForMaximum),
        propInterval (propertyForInterval)
    {
        // Don't attach an invalid valuetree!
        jassert (tree.isValid());
        slider = _slider;

        if (tree.hasProperty (propMinimum) && tree.hasProperty (propMaximum)) {
            slider->setRange (tree.getProperty (propMinimum), tree.getProperty (propMaximum), tree.getProperty (propInterval, 0));
        }

        if (tree.hasProperty (property)) {
            slider->setValue (tree.getProperty(property));
        }
        else {
            tree.setProperty (property, slider->getValue(), undoMgr);
        }

        tree.addListener (this);
        slider->addListener (this);
    }

    ~ValueTreeSliderAttachment ()
    {
        tree.removeListener (this);
        if (slider) {
            slider->removeListener (this);
        }
    }

    /**
     This updates the ValueTree's property to reflect the Slider's position
     */
    void sliderValueChanged (juce::Slider *sliderThatChanged) override
    {
        if (! updating) {
            updating = true;
            if (slider == sliderThatChanged) {
                tree.setProperty (property, slider->getValue(), undoMgr);
            }
            updating = false;
        }
    }

    /**
     This updates the Slider to reflect the ValueTree's property
     */
    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &changedProperty) override
    {
        if (! updating) {
            updating = true;
            if (treeWhosePropertyHasChanged == tree && slider) {
                if (changedProperty == property) {
                    slider->setValue (tree.getProperty (property));
                }
                else if (property == propMinimum || property == propMaximum || property == propInterval) {
                    if (tree.hasProperty (propMinimum) && tree.hasProperty (propMaximum)) {
                        slider->setRange (tree.getProperty (propMinimum), tree.getProperty (propMaximum), tree.getProperty (propInterval, 0));
                    }
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
    juce::Component::SafePointer<juce::Slider>  slider;
    juce::Identifier                            property;
    juce::UndoManager*                          undoMgr = nullptr;
    juce::Identifier                            propMinimum;
    juce::Identifier                            propMaximum;
    juce::Identifier                            propInterval;
    bool                                        updating = false;
};
