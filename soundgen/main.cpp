// main.cpp
// Author: Yaz Khabiri
// 

#define _USE_MATH_DEFINES

#include <cstdio>
#include <cmath>

//#include <Windows.h>
//#pragma comment( lib, "Winmm.lib" )

#define chirp expChirp
#define PI 3.14159265359
#define SAMPLE_RATE 44100.

// https://en.wikipedia.org/wiki/Chirp#Linear
double linChirp(double time, double freqStart, double freqEnd, double sweepTime)
{
	double k, sound;
	double phaseStart = 0.0;

	k = (freqEnd - freqStart) / sweepTime;
	sound = sin(phaseStart + 2.0 * PI * (freqStart * time + k / 2.0 * time * time));

	//chirp *= clamp(sign(sweepTime - time), 0.0, 1.0);

	return sound;
}

// https://en.wikipedia.org/wiki/Chirp#Exponential
double expChirp(double time, double freqStart, double freqEnd, double sweepTime)
{
	double k, sound;
	double phaseStart = 0.0;

	k = pow(freqEnd / freqStart, 1.0 / sweepTime);
	sound = sin(phaseStart + 2.0 * PI * freqStart * (pow(k, time) - 1.0) / log(k));

	//chirp *= clamp(sign(sweepTime - time), 0.0, 1.0);

	return sound;
}

double wav(double time, double freq)
{
	return sin(2.0*PI*freq*time);
}

// linear loop percent
double fade(double intervalPercent)
{
	return exp(-intervalPercent * 2.7);
}

double linShepard(double time)
{
	double sound = 0.0;
	double sweepTime = 2.0;
	const double f = 27.5;
	double orgtime = time;
	time = fmod(time, sweepTime);

	double loopPercent = time / sweepTime;

	sound += 0.125 * linChirp(time, f, f * 2, sweepTime);
	sound += 0.125 * linChirp(time, f * 2, f * 4, sweepTime);
	sound += 0.125 * linChirp(time, f * 4, f * 8, sweepTime);
	sound += 0.125 * linChirp(time, f * 8, f * 16, sweepTime);
	sound += 0.125 * linChirp(time, f * 16, f * 32, sweepTime);
	sound += 0.125 * linChirp(time, f * 32, f * 64, sweepTime);
	sound += 0.125 * linChirp(time, f * 64, f * 128, sweepTime);
	sound += 0.125 * linChirp(time, f * 128, f * 256, sweepTime);

	return (sound);
}

double expShepard(double time)
{
	double sound = 0.0;
	double sweepTime = 2.0;
	const double f = 27.5;

	// old method, doubling frequency, time = time % sweepTime
	//sound += 0.125 * chirp(orgtime, f * 1,   f * 2,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 2,   f * 4,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 4,   f * 8,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 8,   f * 16,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 16,  f * 32,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 32,  f * 64,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 64,  f * 128, sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 128, f * 256, sweepTime);

	
	// new method, double sweepTime, use same freq range
	sound += 0.125 * chirp(time + sweepTime * 0., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 1., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 2., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 3., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 4., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 5., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 6., f, f*2, sweepTime);
	sound += 0.125 * chirp(time + sweepTime * 7., f, f*2, sweepTime);

	return sound;
}

int main()
{
	FILE *f;
	int curSample = 0;
	double curTime = 0;
	const double totalTime = 4.0;

	fopen_s(&f, "shepard.wav", "wb");
	
	const int totalSamples = (int)ceil( totalTime * SAMPLE_RATE );
	double *wav = new double[totalSamples];

	while (curTime < totalTime)
	{
		wav[curSample] = expShepard(curTime);

		curTime = ++curSample / SAMPLE_RATE;
	}

	fwrite(wav, sizeof(double), totalSamples, f);
	fclose(f);

	delete[] wav;

	//PlaySound(TEXT("shepard.wav"), NULL, SND_FILENAME);

	return 0;
}