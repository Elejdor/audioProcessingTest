#pragma once

#include <vector>

namespace juce
{
	class StreamingSocket;

	namespace dsp
	{
		class FFT;
	}
}

class Game
{
public:
	Game();
	~Game();

	void Init();
	void AddSample(float value);

private:
	void Compute();
	void SendChunk(float* data, int size);

private:
	juce::StreamingSocket*	m_tcp;
	std::vector< float >	m_samples;
	juce::dsp::FFT*			m_fft;

	bool					m_initialized;

	const int				c_port = 1338;

	enum
	{
		c_fftOrder = 10,
		c_fftSize = 1 << c_fftOrder

	};

	float m_fftData[2 * c_fftSize];
};