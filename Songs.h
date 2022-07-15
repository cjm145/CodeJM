/*
  ==============================================================================

    Songs.h
    Created: 26 February 2022 1:32:25pm
    Author:  jiaming

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Songs
{
    public:
        Songs(juce::File _file);
        juce::File file;
        juce::URL URL;
        juce::String title;
        juce::String length;
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;

};