/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ValueTreeSliderAttachment.h"
#include "ValueTreeComboBoxAttachment.h"
#include "ValueTreeRadioButtonGroupAttachment.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
    ScopedPointer<Slider> slider;
    ScopedPointer<Slider> slider2;
    ValueTree tree;

    ScopedPointer<ComboBox> combo;
    ScopedPointer<ComboBox> combo1;

    ScopedPointer<ValueTreeSliderAttachment> attachment;
    ScopedPointer<ValueTreeSliderAttachment> attachment2;
    ScopedPointer<ValueTreeComboBoxAttachment> comboAttachment;
    ScopedPointer<ValueTreeComboBoxAttachment> comboAttachment1;
    ScopedPointer<ValueTreeRadioButtonGroupAttachment> buttonGroupAttachment;
};


#endif  // MAINCOMPONENT_H_INCLUDED
