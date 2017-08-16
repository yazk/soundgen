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

	//sound *= clamp(sign(sweepTime - time), 0.0, 1.0);

	return sound;
}

// https://en.wikipedia.org/wiki/Chirp#Exponential
double expChirp(double time, double freqStart, double freqEnd, double sweepTime)
{
	double k, sound;
	double phaseStart = 0.0;

	k = pow(freqEnd / freqStart, 1.0 / sweepTime);
	sound = sin(phaseStart + 2.0 * PI * freqStart * (pow(k, time) - 1.0) / log(k));

	//sound *= clamp(sign(sweepTime - time), 0.0, 1.0);

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

double linShepard(double time, double f = 27.5, double sweepTime = 2.0)
{
	double sound = 0.0;

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

	return sound;
}

double envelope(double time, double octave)
{
	//return 1.0;
	//return -0.5 * cos(2. * PI * (time + octave) / 8.) + 0.5;
	return -0.125 * cos(2. * PI * (time + octave) / 8.) + 0.5;
}

double expShepard(double time, double f1, double f2, double sweepTime)
{
	double sound = 0.0;

	time = fmod(time, sweepTime);

	double intervalPercent = time / sweepTime;

	// old method, doubling frequency, time = time % sweepTime
	// f*1 -> f*2 + f*2 -> f*4, etc

	// new method, double sweepTime, use same freq range
	// better because there is no continuity issue (phase) between intervals
	// fade top and bottom frequencies
#define NUM_OCTAVES 8
	sound += envelope(intervalPercent, 0) * (1 - fade(intervalPercent)) *	expChirp(time + sweepTime * 0., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 1) *								expChirp(time + sweepTime * 1., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 2) *								expChirp(time + sweepTime * 2., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 3) *								expChirp(time + sweepTime * 3., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 4) *								expChirp(time + sweepTime * 4., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 5) *								expChirp(time + sweepTime * 5., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 6) *								expChirp(time + sweepTime * 6., f1, f2, sweepTime);
	sound += envelope(intervalPercent, 7) * fade(intervalPercent) *		expChirp(time + sweepTime * 7., f1, f2, sweepTime);

	sound *= 1. / NUM_OCTAVES;

	return sound;
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

	const int totalSamples = (int)ceil(totalTime * SAMPLE_RATE);
	double *wav = new double[totalSamples];

	while (curTime < totalTime)
	{
		wav[curSample] = 0;
		wav[curSample] += expShepard(curTime, 27.5, 55, 15.0);
		//wav[curSample] += mainSound(curTime);
		//wav[curSample] += expShepard(curTime, 29.14, 58.27, 15.0);
		//wav[curSample] += expShepard(curTime, 30.87, 61.74, 15.0);
		//wav[curSample] += expShepard(curTime, 32.70, 65.41, 15.0);
		//wav[curSample] += expShepard(curTime, 34.65, 69.30, 15.0);
		//wav[curSample] += expShepard(curTime, 36.71, 73.42, 15.0);
		//wav[curSample] += expShepard(curTime, 38.89, 77.78, 15.0);
		//wav[curSample] += expShepard(curTime, 41.20, 82.41, 15.0);
		//wav[curSample] += expShepard(curTime, 43.65, 87.31, 15.0);
		//wav[curSample] += expShepard(curTime, 46.25, 92.50, 15.0);
		//wav[curSample] += expShepard(curTime, 49.00, 98.00, 15.0);
		//wav[curSample] += expShepard(curTime, 51.91, 103.83, 15.0);
		//wav[curSample] *= 1./12.;
		//wav[curSample] = expShepard_wikipedia(curTime);

		curTime = ++curSample / SAMPLE_RATE;
	}

	fwrite(wav, sizeof(double), totalSamples, f);

	fclose(f);

	delete[] wav;

	//PlaySound(TEXT("shepard.wav"), NULL, SND_FILENAME);

	return 0;
}