ffGuiAttachments
================

by Daniel Walz / Foleys Finest Audio UG (haftungsbeschraenkt)
Published under the BSD License (3 clause)

Started June, 2016

In this module some additions to JUCE (www.juce.com) are provided to attach
a Slider, ComboBox or RadioButtonGroup to a leaf in a ValueTree.
This is especially useful to keep GUI components in sync with a ValueTree,
e.g. th tree behind AudioProcessorValueTreeState. So you can store values
that are not exposed to the host as parameters.

API-docs
========

The doxygen API doc can be found here: https://ffaudio.github.io/ffGuiAttachments/

They are used exatly the same as AudioProcessorValueTree::SliderAttachment. 
In the ValueTreeSliderAttachment you can also supply a range for the slider.

Have fun...
Daniel
