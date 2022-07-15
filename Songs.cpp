/*
  ==============================================================================

    Songs.cpp
    Created: 26 February 2022 1:32:25pm
    Author:  jiaming

  ==============================================================================
*/

#include "Songs.h"
#include <filesystem>

Songs::Songs(juce::File _file) : file(_file),
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    DBG("Created new track with title: " << title);
}

bool Songs::operator==(const juce::String& other) const
{
    return title == other;
}