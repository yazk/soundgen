#include <cstdio>

#define _USE_MATH_DEFINES
#include <cmath>
#include <Windows.h>

#pragma comment( lib, "Winmm.lib" )

#define chirp linChirp
#define PI 3.14159265359

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

#define SAMPLE_RATE 44100.

double sweepExtra(double freq, double sweepTime)
{
	return 2.0 * PI * freq * sweepTime / SAMPLE_RATE;
}

//double mainSound(double time)
//{
//	double sound = 0.0;
//	double sweepTime = 2.0;
//	double f = 27.5;
//	double orgtime = time;
//	time = fmod(time, sweepTime);
//
//	// example:
//	// 27.5 -> 55
//	// 55   -> 110
//	// 110  -> 220
//	// 220  -> 440
//	// etc
//
//	//sound += 0.125 * expChirp(time, f, f*2,       sweepTime );
//	//sound += 0.125 * expChirp(time, f*2, f*4,     sweepTime );
//	//sound += 0.125 * expChirp(time, f*4, f*8,     sweepTime );
//	//sound += 0.125 * expChirp(time, f*8, f*16,    sweepTime );
//	//sound += 0.125 * expChirp(time, f*16, f*32,   sweepTime );
//	//sound += 0.125 * expChirp(time, f*32, f*64,   sweepTime );
//	//sound += 0.125 * expChirp(time, f*64, f*128,  sweepTime );
//	//sound += 0.125 * expChirp(time, f*128, f*256, sweepTime );
//
//	if (orgtime < sweepTime)
//	{
//		sound += 1.0 * expChirp(time - 29. / 44100., f * 16, f * 32, sweepTime);
//
//		// set 1
//		//sound += 0.125 * expChirp(time-4./SAMPLE_RATE, f * 32, f * 64, sweepTime);
//
//		//sound += 0.125 * expChirp(time, f * 64, f * 128, sweepTime);
//	}
//	else
//	{
//		//sound += 0.125 * expChirp(time, f * 16, f * 32, sweepTime);
//
//		sound += 1.0 * expChirp(time, f * 32, f * 64, sweepTime);
//
//		// set 1
//		//sound += 0.125 * expChirp(time, f * 64, f * 128, sweepTime);
//	}
//
//	//sound += 0.14 * wav(time, freq*0.25);
//	//sound += 0.14 * wav(time, freq*0.50);
//	//sound += 0.14 * wav(time, freq);
//	//sound += 0.14 * wav(time, freq*2.0);
//	//sound += 0.14 * wav(time, freq*4.0);
//	//sound += 0.14 * wav(time, freq*8.0);
//	//sound += 0.14 * wav(time, freq*16.0);
//
//	return (sound);
//}



// linear loop percent
double fade(double intervalPercent)
{
	return exp(-intervalPercent * 2.7);
}

double mainSound(double time)
{
	double sound = 0.0;
	double sweepTime = 2.0;
	const double f = 27.5;
	double orgtime = time;
	time = fmod(time, sweepTime);

	// example:
	// 27.5 -> 55
	// 55   -> 110
	// 110  -> 220
	// 220  -> 440
	// etc

	double loopPercent = time/sweepTime;

	//sound += 0.125 * expChirp(time, f, f*2,       sweepTime );
	//sound += 0.125 * expChirp(time, f*2, f*4,     sweepTime );
	//sound += 0.125 * expChirp(time, f*4, f*8,     sweepTime );
	//sound += 0.125 * expChirp(time, f*8, f*16,    sweepTime );
	//sound += 0.125 * expChirp(time, f*16, f*32,   sweepTime );
	//sound += 0.125 * expChirp(time, f*32, f*64,   sweepTime );
	//sound += 0.125 * expChirp(time, f*64, f*128,  sweepTime );
	//sound += 0.125 * expChirp(time, f*128, f*256, sweepTime );

	////if (abs(orgtime - sweepTime) < 1. / SAMPLE_RATE)
	////{
	////	sound += 0.25 * linChirp(time, f * 16, f * 32, sweepTime);
	////	sound += 0.25 * linChirp(time, f * 32, f * 64, sweepTime);
	////
	////	sound += 0.25 * linChirp(time, f * 32, f * 64, sweepTime);
	////	sound += 0.25 * linChirp(time, f * 64, f * 128, sweepTime);
	////}
	///*else*/ if (orgtime < sweepTime)
	//{
	//	// set 2
	//	sound += 0.5 * linChirp(time, f * 16, f * 32, sweepTime );
	//	
	//	// set 1
	//	sound += 0.5 * linChirp(time, f * 32, f * 64, sweepTime);
	//	
	//	//sound += 0.125 * expChirp(time, f * 64, f * 128, sweepTime);
	//}
	//else
	//{
	//	//sound += 0.125 * expChirp(time, f * 16, f * 32, sweepTime);
	//	
	//	// set 2
	//	sound += 0.5 * linChirp(time, f * 32, f * 64, sweepTime);
	//	
	//	// set 1
	//	sound += 0.5 * linChirp(time, f * 64, f * 128, sweepTime);
	//}

	if (orgtime < sweepTime)
	{
		sound += 0.125 * chirp(time, f, f * 2, sweepTime);
		sound += 0.125 * chirp(time, f * 2, f * 4, sweepTime);
		sound += 0.125 * chirp(time, f * 4, f * 8, sweepTime);
		sound += 0.125 * chirp(time, f * 8, f * 16, sweepTime);
		sound += 0.125 * chirp(time, f * 16, f * 32, sweepTime);
		sound += 0.125 * chirp(time, f * 32, f * 64, sweepTime);
		sound += 0.125 * chirp(time, f * 64, f * 128, sweepTime);
		//sound += 0.125 * chirp(time, f * 128, f * 256, sweepTime);
	}
	else if (orgtime < sweepTime * 2)
	{
		//sound += 0.125 * chirp(time, f, f * 2, sweepTime);
		sound += 0.125 * chirp(time, f * 2, f * 4, sweepTime);
		sound += 0.125 * chirp(time, f * 4, f * 8, sweepTime);
		sound += 0.125 * chirp(time, f * 8, f * 16, sweepTime);
		sound += 0.125 * chirp(time, f * 16, f * 32, sweepTime);
		sound += 0.125 * chirp(time, f * 32, f * 64, sweepTime);
		sound += 0.125 * chirp(time, f * 64, f * 128, sweepTime);
		sound += fade(loopPercent) * 0.125 * chirp(time, f * 128, f * 256, sweepTime);
	}
	else
	{
		//sound += 0.125 * chirp(time, f, f * 2, sweepTime);
		sound += (1. - fade(loopPercent)) * 0.125 * 0.125 * chirp(time, f * 2, f * 4, sweepTime);
		sound += 0.125 * chirp(time, f * 4, f * 8, sweepTime);
		sound += 0.125 * chirp(time, f * 8, f * 16, sweepTime);
		sound += 0.125 * chirp(time, f * 16, f * 32, sweepTime);
		sound += 0.125 * chirp(time, f * 32, f * 64, sweepTime);
		sound += 0.125 * chirp(time, f * 64, f * 128, sweepTime);
		sound += 0.125 * chirp(time, f * 128, f * 256, sweepTime);
	}

	return (sound);
}


int main()
{
	FILE *f;
	int curSample = 0;
	double curTime = 0;
	const double totalTime = 6.0;

	fopen_s(&f, "shepard.wav", "wb");
	
	const int totalSamples = (int)ceil( totalTime * SAMPLE_RATE );
	double *wav = new double[totalSamples];

	while (curTime < totalTime )
	{
		wav[curSample] = mainSound(curTime);

		// curTime = ++i * 1.0 / 44100.0;
		curTime = ++curSample / SAMPLE_RATE;
	}

	fwrite(wav, sizeof(double), totalSamples, f);
	fclose(f);

	delete[] wav;

	//PlaySound(TEXT("shepard.wav"), NULL, SND_FILENAME);

	

	return 0;
}