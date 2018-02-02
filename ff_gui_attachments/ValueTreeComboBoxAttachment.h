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

    ValueTreeComboBoxAttachment.h
    Created: 4 Jun 2016 11:17:03am
    Author:  Daniel Walz / Foleys Finest Audio

  ==============================================================================
*/

#pragma once


//==============================================================================
/**
 \class ValueTreeComboBoxAttachment
 \brief This attachment connects a node in a ValueTree with a combobox
 
 The ValueTreeComboBoxAttachment supports two modes:
 selectSubNodes == true: The Combobox is filled with the sub nodes. As combobox items the
 property is used.

 selectSubNodes == false: The combobox has already it's items and the selected index 
 is stored in the property.
 */
class ValueTreeComboBoxAttachment : public juce::ComboBox::Listener,
                                    public juce::ValueTree::Listener
{
public:
    /**
     Create a ValueTreeComboBoxAttachment
     It handles updates from a ValueTree to a ComboBox and vice versa.
     If you set \param selectSubNodes, the ComboBox will get the child nodes as options.
     The selected child node will get the property "selected" == 1.
     */
    ValueTreeComboBoxAttachment (juce::ValueTree& attachToTree,
                                 juce::ComboBox* comboBoxToAttach,
                                 juce::Identifier indexProperty,
                                 bool shouldSelectSubNodes,
                                 juce::UndoManager* undoManagerToUse = nullptr)
    :   tree (attachToTree),
        property (indexProperty),
        selectSubNodes (shouldSelectSubNodes),
        undoMgr (undoManagerToUse)
    {
        // Don't attach an invalid valuetree!
        jassert (tree.isValid());
        comboBox = comboBoxToAttach;

        if (selectSubNodes) {
            updateChoices ();
        }
        else {
            if (tree.hasProperty (property)) {
                comboBox->setSelectedItemIndex (tree.getProperty(property));
            }
            else {
                tree.setProperty (property, comboBox->getSelectedItemIndex(), undoMgr);
            }
        }
        tree.addListener (this);
        comboBox->addListener (this);
    }

    ~ValueTreeComboBoxAttachment ()
    {
        tree.removeListener (this);
        if (comboBox) {
            comboBox->removeListener (this);
        }
    }

    /** Updates the ValueTree's property if the ComboBox has changed */
    void comboBoxChanged (juce::ComboBox *comboBoxThatHasChanged) override
    {
        if (! updating) {
            updating = true;
            if (comboBox == comboBoxThatHasChanged) {
                if (selectSubNodes) {
                    const int idx = comboBox->getSelectedItemIndex ();
                    for (int i=0; i < tree.getNumChildren(); ++i) {
                        if (i == idx) {
                            tree.getChild (i).setProperty (FF::propSelected, 1, undoMgr);
                        }
                        else {
                            tree.getChild (i).removeProperty (FF::propSelected, undoMgr);
                        }
                    }
                }
                else {
                    tree.setProperty (property, comboBox->getSelectedItemIndex (), undoMgr);
                }
            }
            updating = false;
        }
    }

    /** Updates the ComboBox property if the ValueTree has changed */
    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &changedProperty) override
    {
        if (! updating) {
            updating = true;
            if (selectSubNodes) {
                if (treeWhosePropertyHasChanged.getParent() == tree) {
                    if (changedProperty == property) {
                        updateChoices();
                    }
                    else if (changedProperty == FF::propSelected) {
                        for (int i=0; i < tree.getNumChildren(); ++i) {
                            if (tree.getChild (i).hasProperty (FF::propSelected)
                                && static_cast<int> (tree.getChild (i).getProperty (FF::propSelected)) > 0)
                            {
                                comboBox->setSelectedItemIndex (i);
                            }
                        }
                    }
                }
            }
            else {
                if (treeWhosePropertyHasChanged == tree && changedProperty == property && comboBox) {
                    if (tree.hasProperty (property)) {
                        const int idx = tree.getProperty (property);
                        comboBox->setSelectedItemIndex (idx);
                    }
                }
            }
            updating = false;
        }
    }
    
    /** If the ValueTree has new child nodes, they will be added as options in the ComboBox */
    void valueTreeChildAdded (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override
    {
        if (selectSubNodes) {
            updateChoices ();
        }
    }
    /** If child nodes were removed from the ValueTree, the options of the ComboBox are updated */
    void valueTreeChildRemoved (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
    {
        if (selectSubNodes) {
            updateChoices ();
        }
    }
    void valueTreeChildOrderChanged (juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (juce::ValueTree &treeWhoseParentHasChanged) override {}
    void valueTreeRedirected (juce::ValueTree &treeWhichHasBeenChanged) override {}


private:

    /**
     This method updates the ComboBoxes choices
     */
    void updateChoices ()
    {
        comboBox->clear();
        for (int i=0; i < tree.getNumChildren(); ++i) {
            juce::ValueTree child = tree.getChild (i);
            comboBox->addItem (child.getProperty (property, child.getType().toString()), 100 + i);
            if (child.hasProperty (FF::propSelected) && static_cast<int> (child.getProperty (FF::propSelected)) != 0) {
                comboBox->setSelectedId (100+i);
            }
        }
    }

    juce::ValueTree                                 tree;
    juce::Component::SafePointer<juce::ComboBox>    comboBox;
    juce::Identifier                                property;
    bool                                            selectSubNodes;
    juce::UndoManager*                              undoMgr  = nullptr;
    bool                                            updating = false;
};
