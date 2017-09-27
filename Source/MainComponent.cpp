/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class MainContentComponent : public AudioAppComponent
{
public:

	//==============================================================================
	MainContentComponent()
		: m_fft(fftOrder)
		, m_nextBlockReady(false)
	{
		setSize(800, 600);
		setAudioChannels(1, 0);
		m_samples.reserve(fftSize * 2);
		bool connected = m_tcp.connect("127.0.0.1", 1338);
		jassert(connected, "Failed to connect");
	}

	~MainContentComponent()
	{
		m_tcp.close();
		shutdownAudio();
	}

	void PushSample(float sample)
	{
		m_samples.push_back(sample);
		if (m_samples.size() == fftSize)
		{
			zeromem(m_fftData, sizeof(m_fftData));
			memcpy(m_fftData, m_samples.data(), m_samples.size());
			ComputeAndSend();

			m_samples.clear();
		}
	}

	void ComputeAndSend()
	{
		m_fft.performFrequencyOnlyForwardTransform(m_fftData);

		float sendBuffer[100];
		const int size = sizeof(float) * 100;
		memcpy(sendBuffer, m_fftData, size);

		if (m_tcp.waitUntilReady(false, 0) == 1)
		{
			jassert(m_tcp.isConnected(), "TCP is not connected");

			const int sent = m_tcp.write(sendBuffer, size);
			jassert(size == sent, "Could not send this amount of data.");
		}
	}

	float FindMaxFrequency(const float* samples)
	{
		const float halfRate = 24000;
		const float invNSamples = 1.0f / 1024.0f;

		float maxValue = 0.0f;
		int maxIndex = -1;
		for (int i = 0; i < 100; ++i)
		{
			if (samples[i] > maxValue)
			{
				maxValue = samples[i] + 1.0f;
				maxIndex = i;
			}
		}

		float freq = maxIndex * halfRate * invNSamples;
		String msg;
		msg << freq << newLine;

		juce::Logger::getCurrentLogger()->writeToLog(msg);
		return freq;
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
			PushSample(inBuffer[i]);
		}

		//float* outBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

		//for (int i = 0; i < bufferToFill.numSamples; ++i)
		//{
		//	outBuffer[ i ] = 0.0f;
		//}
	}


	void releaseResources() override
	{
		// This will be called when the audio device stops, or when it is being
		// restarted due to a setting change.

		// For more details, see the help for AudioProcessor::releaseResources()
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
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
	}

	void thumbnailChanged()
	{
		repaint();
	}

private:
	//==============================================================================

	// Your private member variables go here...

	enum
	{
		fftOrder = 11,
		fftSize = 1 << fftOrder
	};

	juce::dsp::FFT m_fft;
	std::vector< float > m_samples;
	float m_fftData[2 * fftSize];
	bool m_nextBlockReady;

	juce::StreamingSocket m_tcp;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }
