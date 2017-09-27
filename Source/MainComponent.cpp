/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

#include "Game.h"
#include "SpectrogramComponent.h"

class MainContentComponent : public AudioAppComponent
{
public:

	//==============================================================================
	MainContentComponent()
	{
		setSize(800, 600);
		setAudioChannels(1, 0);

		m_game.Init();
	}

	~MainContentComponent()
	{
		shutdownAudio();
	}

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		// Your audio-processing code goes here!

		// For more details, see the help for AudioProcessor::getNextAudioBlock()
		const float* inBuffer = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			m_game.AddSample(inBuffer[i]);
		}

		//float* outBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

		//for (int i = 0; i < bufferToFill.numSamples; ++i)
		//{
		//	outBuffer[ i ] = 0.0f;
		//}
	}


	void releaseResources() override
	{
		Logger::getCurrentLogger()->writeToLog("Releasing resources");
	}

	//==============================================================================
	void paint(Graphics& g) override
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

		// You can add your drawing code here!
	}

	void resized() override
	{
	}

private:
	Game m_game;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }
