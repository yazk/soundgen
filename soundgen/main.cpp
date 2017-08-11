// main.cpp
// Author: Yaz Khabiri

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
	return exp(-pow(intervalPercent, 4.0) * 7.);
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

double expShepard(double time, double f1, double f2, double sweepTime)
{
	double sound = 0.0;

	// old method, doubling frequency, time = time % sweepTime
	//sound += 0.125 * chirp(orgtime, f * 1,   f * 2,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 2,   f * 4,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 4,   f * 8,   sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 8,   f * 16,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 16,  f * 32,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 32,  f * 64,  sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 64,  f * 128, sweepTime);
	//sound += 0.125 * chirp(orgtime, f * 128, f * 256, sweepTime);
	
	time = fmod(time, sweepTime);
	double intervalPercent = time / sweepTime;

	// new method, double sweepTime, use same freq range
	// better because there is no continuity issue (phase) between intervals
#define NUM_OCTAVES 8
	sound += (1- fade(intervalPercent)) *chirp(time + sweepTime * 0., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 1., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 2., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 3., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 4., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 5., f1, f2, sweepTime);
	sound += chirp(time + sweepTime * 6., f1, f2, sweepTime);
	sound += fade(intervalPercent) * chirp(time + sweepTime * 7., f1, f2, sweepTime);

	sound *= 1. / NUM_OCTAVES;

	return sound;
}

// https://en.wikipedia.org/wiki/File:DescenteInfinie.ogg
#define Period 20.
#define A0 0.7

float a(float t)
{
	return A0 * (1. + cos(PI * t / Period)) / 4.;
}

float w(float t, float freq)
{
	return 2. * PI * freq * 1. / pow(2., t / Period);
}

float s(float t, float freq)
{
	return a(t) * sin(w(t, freq) * t) + a(t + Period) * sin(2. * w(t, freq) * t);
}

double expShepard_wikipedia(double time)
{
	double sound = 0.0;

	sound += s(time, 440.);
	sound += s(time, 523.25);

	sound += s(time, 440.*2.);
	sound += s(time, 523.25*2.);

	sound += s(time, 440.*4.);
	sound += s(time, 523.25*4.);

	sound += s(time, 440.*8.);
	sound += s(time, 523.25*8.);

	return 0.25 * sound;
}

int main()
{
	FILE *f;
	int curSample = 0;
	double curTime = 0;
	const double totalTime = 60;

	fopen_s(&f, "shepard.wav", "wb");
	
	if (!f)
	{
		printf("file faile to open\n");
		return 0;
	}

	const int totalSamples = (int)ceil( totalTime * SAMPLE_RATE );
	double *wav = new double[totalSamples];

	while (curTime < totalTime)
	{
		wav[curSample] = expShepard(curTime, 27.5, 55, 15.0);
		//wav[curSample] = expShepard_wikipedia(curTime);

		curTime = ++curSample / SAMPLE_RATE;
	}

	fwrite(wav, sizeof(double), totalSamples, f);

	fclose(f);

	delete[] wav;

	//PlaySound(TEXT("shepard.wav"), NULL, SND_FILENAME);

	return 0;
}