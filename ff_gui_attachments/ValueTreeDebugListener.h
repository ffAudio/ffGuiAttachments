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

    ValueTreeDebugListener.h
    Created: 12 Jun 2016 11:17:03am
    Author:  Daniel Walz / Foleys Finest Audio

  ==============================================================================
*/

#pragma once

/**
 To debug the callbacks of a ValueTree::Listener simply attach a ValueTreeDebugListener to the tree and see 
 debug information in the console.
 */
class ValueTreeDebugListener : public juce::ValueTree::Listener
{
public:
    /**
     Create a Debug listener to write callbacks to DBG.
     If you want to see callbacks from child trees too set @param includeChildren to true.
     @param dumpTreeLevel is not yet implemented.
     */
    ValueTreeDebugListener (juce::ValueTree& treeToWatch, int shouldIncludeChildren=false, const int requestedDumpTreeLevel=0)
      : tree (treeToWatch),
        includeChildren (shouldIncludeChildren),
        dumpTreeLevel (requestedDumpTreeLevel)
    {
        // Don't attach an invalid valuetree!
        jassert (tree.isValid());
        tree.addListener (this);
        DBG ("Debug listener attached to " + debugStringForTree (tree));
    }

    ~ValueTreeDebugListener ()
    {
        tree.removeListener (this);
    }

    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override
    {
        if (includeChildren || treeWhosePropertyHasChanged == tree) {
            DBG (debugStringForTree (treeWhosePropertyHasChanged) + " property \"" + property.toString() + "\"" +
                 (treeWhosePropertyHasChanged.hasProperty(property) ?
                  " new value: " + treeWhosePropertyHasChanged.getProperty (property).toString() : " was removed" ));
        }
    }

    void valueTreeChildAdded (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override
    {
        if (includeChildren || parentTree == tree) {
            DBG (debugStringForTree (parentTree) + " has new child with type: " + childWhichHasBeenAdded.getType().toString());
        }
    }

    void valueTreeChildRemoved (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
    {
        if (includeChildren || parentTree == tree) {
            DBG (debugStringForTree(parentTree) + " lost child with type: " +
                 childWhichHasBeenRemoved.getType().toString() + " at index: " + juce::String (indexFromWhichChildWasRemoved));
        }
    }

    void valueTreeChildOrderChanged (juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override
    {
        if (includeChildren || parentTreeWhoseChildrenHaveMoved == tree) {
            DBG (debugStringForTree (parentTreeWhoseChildrenHaveMoved) + " changed index from " +
                 juce::String (oldIndex) + " to new index " + juce::String (newIndex));
        }
    }

    void valueTreeParentChanged (juce::ValueTree &treeWhoseParentHasChanged) override
    {
        if (includeChildren || treeWhoseParentHasChanged == tree) {
            DBG (debugStringForTree (treeWhoseParentHasChanged) + " parent changed");
        }
    }

    void valueTreeRedirected (juce::ValueTree &treeWhichHasBeenChanged) override
    {
        if (includeChildren || treeWhichHasBeenChanged == tree) {
            DBG (debugStringForTree (treeWhichHasBeenChanged) + " was redirected");
        }
    }

private:

    /** returns a descriptive string for _tree */
    juce::String debugStringForTree (juce::ValueTree& debuggedTree) const
    {
        if (debuggedTree == tree) {
            return juce::String ("ValueTree <" + debuggedTree.getType().toString() + ">");
        }
        else {
            int level = getChildOrder (debuggedTree);
            if (level < 0) {
                return juce::String ("ValueTree <" + debuggedTree.getType().toString() + "> no child of watched tree");
            }
            else {
                return juce::String ("ValueTree <" + debuggedTree.getType().toString() + "> child level " + juce::String (level));
            }
        }
    }

    /** returns the level from _tree to watched tree */
    int getChildOrder (juce::ValueTree& debuggedTree) const
    {
        if (debuggedTree.isAChildOf(tree)) {
            int level = 0;
            juce::ValueTree seeker = debuggedTree;
            while (seeker.isValid()) {
                if (seeker == tree) {
                    return level;
                }
                seeker = seeker.getParent();
                ++level;
            }
        }
        return -1;
    }

    juce::ValueTree tree;
    bool            includeChildren;
    int             dumpTreeLevel;
};
