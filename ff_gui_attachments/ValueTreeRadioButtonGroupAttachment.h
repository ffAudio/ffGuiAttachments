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
    Author:  Daniel Walz / Foleys Finest Audio

  ==============================================================================
*/

#pragma once

/**
 \class ValueTreeRadioButtonGroupAttachment
 \brief The ValueTreeRadioButtonGroupAttachment keeps a radio button group in sync
 with a leaf in a ValueTree.
 
 If selectSubNodes is set to true, for the selected radio button, a property 
 selected is set to 1 in the node having the same componentID as the given 
 property in the attachment.
 */
class ValueTreeRadioButtonGroupAttachment : public juce::ValueTree::Listener,
                                            public juce::Button::Listener
{
public:
    /**
     Create a ValueTreeRadioButtonGroupAttachment
     It handles updates from a ValueTree to a RadioButtonGroup and vice versa.
     If you set \param selectSubNodes, the selected child node corresponding to
     the Button with the same ComponentID will get the property "selected" == 1.
     */
    ValueTreeRadioButtonGroupAttachment (juce::ValueTree& attachToTree,
                                         juce::Array<juce::Button*>& _buttons,
                                         juce::Identifier indexProperty,
                                         bool shouldSelectSubNodes,
                                         juce::UndoManager* undoManagerToUse = nullptr)
    :   tree (attachToTree),
        property (indexProperty),
        selectSubNodes (shouldSelectSubNodes),
        undoMgr (undoManagerToUse),
        updating (false)
    {

        for (int i=0; i < _buttons.size(); ++i) {
            juce::Button* b = _buttons.getUnchecked (i);
            if (! buttons.contains (b)) {
                buttons.add (b);
                b->addListener (this);
            }
        }

        if (tree.getNumChildren() < 1) {
            for (int i=0; i < buttons.size(); ++i) {
                juce::Button* b = buttons.getUnchecked (i);
                juce::ValueTree child = juce::ValueTree ("option");
                child.setProperty (property, b->getComponentID(), undoMgr);
                tree.addChild (child, -1, undoMgr);
            }
        }
        else {
            for (int i=0; i < buttons.size(); ++i) {
                juce::Button* b = buttons.getUnchecked (i);
                for (int k=0; k < tree.getNumChildren(); ++k) {
                    juce::ValueTree child = tree.getChild (k);
                    if (child.hasProperty (property) && child.getProperty (property) == b->getComponentID()) {
                        if (child.hasProperty (FF::propSelected) && static_cast<int> (child.getProperty (FF::propSelected)) != 0) {
                            b->setToggleState (true, juce::dontSendNotification);
                        }
                        else {
                            b->setToggleState (false, juce::dontSendNotification);
                        }
                    }
                }
            }
        }

        tree.addListener (this);
    }

    ~ValueTreeRadioButtonGroupAttachment ()
    {
        tree.removeListener (this);
        for (auto b : buttons) {
            if (b) {
                b->removeListener (this);
            }
        }
    }

    void buttonClicked (juce::Button*) override {}

    void buttonStateChanged (juce::Button *buttonThatHasChanged) override
    {
        if (selectSubNodes) {
            if (buttonThatHasChanged->getToggleState()) {
                for (int i=0; i < tree.getNumChildren(); ++i) {
                    juce::ValueTree child = tree.getChild (i);
                    if (child.hasProperty (property)) {
                        if (child.getProperty (property) == buttonThatHasChanged->getComponentID()) {
                            child.setProperty (FF::propSelected, 1, undoMgr);
                        }
                        else {
                            child.removeProperty (FF::propSelected, undoMgr);
                        }
                    }
                }
            }
        }

    }

    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
    {
        if (! updating) {
            updating = true;
            if (selectSubNodes) {
                if (treeWhosePropertyHasChanged.getParent() == tree
                    && _property == juce::Identifier (FF::propSelected))
                {
                    if (treeWhosePropertyHasChanged.hasProperty (FF::propSelected)) {
                        juce::String selected = treeWhosePropertyHasChanged.getProperty (property);
                        for (int i=0; i < buttons.size(); ++i) {
                            juce::Component::SafePointer<juce::Button> b = buttons.getUnchecked (i);
                            if (b->getComponentID() == selected) {
                                b->setToggleState (true, juce::sendNotification);
                            }
                        }
                    }
                }
            }
            else {
                if (treeWhosePropertyHasChanged == tree && _property == property) {
                    juce::String selected = tree.getProperty (property, "");
                    for (int i=0; i < buttons.size(); ++i) {
                        juce::Component::SafePointer<juce::Button> b = buttons.getUnchecked (i);
                        if (b->getComponentID() == selected) {
                            b->setToggleState (true, juce::sendNotification);
                        }
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
    juce::ValueTree    tree;
    juce::Array<juce::Component::SafePointer<juce::Button> > buttons;
    juce::Identifier   property;
    bool               selectSubNodes;
    juce::UndoManager* undoMgr  = nullptr;
    bool               updating = false;;

};
