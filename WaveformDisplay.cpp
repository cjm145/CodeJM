/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 15 February 2022 11:22:25pm
    Author:  jiaming

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id,
                                juce::AudioFormatManager& formatManager,
                                juce::AudioThumbnailCache& thumbCache
                                 ) : audioThumb(1000, formatManager, thumbCache),
                                     fileLoaded(false),
                                     position(0),
                                     id(_id)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // clear the background
    g.fillAll(Colour::fromRGB(5, 32, 75));

    // draw an outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    // Set colour of the WaveformDisplay
    g.setColour(Colour::fromRGB(70, 183, 232));

    g.setFont(18.0f);
    /*Show Deck Player 1 & 2 by getting from the */
    g.drawText("Deck Player " + std::to_string(id), getLocalBounds(),
        juce::Justification::topLeft, true);

    if (fileLoaded)
    {
        g.setFont(15.0f);
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        g.setColour(juce::Colours::greenyellow);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.setColour(juce::Colours::white);
        g.drawText(fileName, getLocalBounds(),
            juce::Justification::bottomLeft, true);
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    DBG("WaveformDisplay::loadURL called");
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("WaveformDisplay::loadURL file loaded");
        fileName = audioURL.getFileName();
        repaint();
    }
    else
    {
        DBG("WaveformDisplay::loadURL file NOT loaded");
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
