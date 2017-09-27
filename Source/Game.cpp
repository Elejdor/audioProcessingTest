#include "Game.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include <limits>

namespace helper
{
	float GetMax(float* data, int size)
	{
		float max = std::numeric_limits< float >::min();

		for (int i = 0; i < size; ++i)
		{
			if (data[i] > max)
			{
				max = data[i];
			}
		}

		return max;
	}
}

Game::Game()
	: m_tcp(nullptr)
	, m_initialized(false)
{
	m_samples.reserve(c_fftSize * 2);
	m_fft = new juce::dsp::FFT(c_fftOrder);
}

Game::~Game()
{
	if (m_tcp->isConnected())
		m_tcp->close();

	delete m_tcp;
	m_tcp = nullptr;

	delete m_fft;
	m_fft = nullptr;
}

void Game::Init()
{
	jassert(!m_initialized, "Game was already initialized.");
	m_initialized = true;

	m_tcp = new juce::StreamingSocket();
	m_tcp->connect("127.0.0.1", c_port, 3000);

	jassert(m_tcp->isConnected(), "Could not connect");
}

void Game::AddSample(float value)
{
	if (!m_initialized)
		return;

	m_samples.push_back(value);

	if (m_samples.size() == c_fftSize)
	{
		zeromem(m_fftData, sizeof(m_fftData));
		memcpy(m_fftData, m_samples.data(), m_samples.size());
		Compute();

		m_samples.clear();
	}
}

void Game::Compute()
{
	m_fft->performFrequencyOnlyForwardTransform(m_fftData);

	if (!m_initialized)
		return;

	// I need to send only first 100 FFT samples, it should be around 0-1000 Hz
	float sendBuffer[100];
	const int size = sizeof(float) * 100;
	memcpy(sendBuffer, m_fftData, size);

	SendChunk(sendBuffer, size);
}

void Game::SendChunk(float* data, int size)
{
	if (m_tcp->waitUntilReady(false, 0) == 1)
	{
		jassert(m_tcp->isConnected(), "TCP is not connected");

		const int sent = m_tcp->write(data, size);
		jassert(size == sent, "Could not send this amount of data.");
	}
}


