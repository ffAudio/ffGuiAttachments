/*******************************************************************************
    juce module: ff_gui_attachments
    author:      Daniel Walz

    started:     June, 2016
    
*******************************************************************************/

In this module some additions to JUCE (www.juce.com) are provided to attach
a Slider, ComboBox or RadioButtonGroup to a leaf in a ValueTree.
This is especially useful to keep GUI components in sync with a ValueTree,
e.g. th tree behind AudioProcessorValueTreeState. So you can store values
that are not exposed to the host as parameters.

They are used exatly the same as AudioProcessorValueTree::SliderAttachment. 
In the ValueTreeSliderAttachment you can also supply a range for the slider.

Have fun...
Daniel
