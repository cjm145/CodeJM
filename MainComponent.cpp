#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure to set the size of the component after
    // added child components.
    setSize(944, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // Use this function to initialise any resources needed, however
    // it will be called on the audio thread, not the GUI thread.

    // Referred to AudioProcessor::prepareToPlay()

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // Referring to AudioProcessor::releaseResources()
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::Colour::fromRGB(5, 32, 62)));
    g.fillAll(Colour::fromRGB(5, 32, 62));


}

void MainComponent::resized()
{
    // To update components of their positions according to the resizing

    int columns = 100;
    auto playlistRight = 28 * getWidth() / columns;
    playlistComponent.setBounds(0, 0, playlistRight, getHeight());
    deckGUI1.setBounds(playlistRight, 0, getWidth() - playlistRight, getHeight() / 2);
    deckGUI2.setBounds(playlistRight, getHeight() / 2, getWidth() - playlistRight, getHeight() / 2);

}