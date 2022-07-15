/*
  ==============================================================================

    DeckGUI.cpp
    Created: 18 January 2022 4:13:39pm
    Author:  jiaming

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManager,
                 juce::AudioThumbnailCache& thumbCache
                 ) : player(_player),
                     id(_id),
                     waveformDisplay(id, formatManager, thumbCache)
{
    //Add all components and make visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volHeader);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedHeader);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posHeader);
    addAndMakeVisible(reverbSlider);
    addAndMakeVisible(waveformDisplay);

    //Add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    reverbSlider.addListener(this);

    //Create functional volume slider and header
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        volSlider.getTextBoxHeight()
    );
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volHeader.setText("Volume", juce::dontSendNotification);
    volHeader.attachToComponent(&volSlider, true);

    //Create functional speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); //reaches breakpoint if sliderValue == 0
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        speedSlider.getTextBoxHeight()
    );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedHeader.setText("Speed", juce::dontSendNotification);
    speedHeader.attachToComponent(&speedSlider, true);

    //Create functional position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        posSlider.getTextBoxHeight()
    );
    posHeader.setText("Position", juce::dontSendNotification);
    posHeader.attachToComponent(&posSlider, true);

    //Create functional reverb slider
    reverbSlider.setRange(0.0, 1.0);
    reverbSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        posSlider.getTextBoxHeight()
    );
    reverbHeader.setText("Reverb", juce::dontSendNotification);
    reverbHeader.attachToComponent(&reverbSlider, true);
    reverbSlider.setNumDecimalPlacesToDisplay(2);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    // Background colour
    g.fillAll(Colour::fromRGB(5, 32, 62));
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Outline the component and colour
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

}

void DeckGUI::resized()
{
    /*Slider position to left component*/
    auto sliderLeft = getWidth() / 10;
    /*Align-Right Side component*/
    auto mainRight = getWidth() - getHeight() / 2;
    /*Align-Right Side Button Layout*/
    auto buttonRight = getWidth() - mainRight;

    //          position --     x  , y  , width, height
    playButton.setBounds(mainRight, 0, buttonRight, getHeight() / 4);
    stopButton.setBounds(mainRight, 2 * getHeight() / 8, buttonRight, getHeight() / 4);
    loadButton.setBounds(mainRight, 3 * getHeight() / 6, buttonRight, getHeight() / 2);
    volSlider.setBounds(sliderLeft, 0, mainRight - sliderLeft, getHeight() / 8);
    speedSlider.setBounds(sliderLeft, getHeight() / 10, mainRight - sliderLeft, getHeight() / 8);
    reverbSlider.setBounds(sliderLeft, 2 * getHeight() / 10, mainRight - sliderLeft, getHeight() / 8);
    posSlider.setBounds(sliderLeft, 3 * getHeight() / 10, mainRight - sliderLeft, getHeight() / 8);
    waveformDisplay.setBounds(0, 4 * getHeight() / 8, mainRight, 4 * getHeight() / 8);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked ");
        player->play();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("Load button was clicked ");
        juce::FileChooser chooser{ "Select a file" };
        if (chooser.browseForFileToOpen())
        {
            loadFile(juce::URL{ chooser.getResult() });
        }
    }
}

//Add the current value to the slider changes
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        DBG("Volume slider moved " << slider->getValue());
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        DBG("Speed slider moved " << slider->getValue());
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        DBG("Position slider moved " << slider->getValue());
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &reverbSlider)
    {
        DBG("Position slider moved " << slider->getValue());
        player->setDamping(slider->getValue());
        player->setWetLevel(slider->getValue());
        player->setDryLevel(slider->getValue());
    }
}

/*  Drag & Drop Music File  */
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag called. "
        + std::to_string(files.size()) + " file(s) being dragged.");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    DBG("DeckGUI::filesDropped at " + std::to_string(x)
        + "x and " + std::to_string(y) + "y");
    if (files.size() == 1)
    {
        loadFile(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{
    //check if the relative position is greater than 0
    //otherwise loading file causes error
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}