/*
  ==============================================================================

    DeckGUI.h
    Created: 18 January 2022 4:13:39pm
    Author:  jiaming

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer

{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player,
            juce::AudioFormatManager& formatManager,
            juce::AudioThumbnailCache& thumbCache);

    ~DeckGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    /*Implement Button::Listener*/
    void buttonClicked(juce::Button* button) override;

    /**Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    /**Detects if file is being dragged over deck*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /**Detects if file is dropped onto deck*/
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /**Listen for changes to the waveform*/
    void timerCallback() override;

private:
    // 
    int id;

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::Slider volSlider;
    juce::Label volHeader;
    juce::Slider speedSlider;
    juce::Label speedHeader;
    juce::Slider posSlider;
    juce::Label posHeader;
    juce::Slider reverbSlider;
    juce::Label reverbHeader;

    void loadFile(juce::URL audioURL);

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    juce::SharedResourcePointer< juce::TooltipWindow > sharedTooltip;

    friend class PlaylistComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};