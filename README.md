# soundgen
Soundgen is a small project used to generate a Shepard tone and output as a WAV file. Audacity can import the file as raw data. The data is output as a 64-bit float, mono sound. The Shepard tone is generated using 8 exponential sweeps each an octave apart starting with the frequency passed in. The generated waveform is a sine wave without additional post processing done on the signal. Additionally, there is an envelope applied to the waveform to emphasize the ascension.

The sound can be heard live on Shadertoy: https://www.shadertoy.com/view/XsjfWy