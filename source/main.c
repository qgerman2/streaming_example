//based on https://github.com/devkitPro/nds-examples/tree/master/templates/arm9
//more information at https://maxmod.devkitpro.org/ref/tut/streaming.html
/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
//.WAV STREAMING
#include <fat.h>
#include <maxmod9.h>

mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format);
FILE* file;

void playWav() {
	//load audio file
	file = fopen("rap.wav", "rb");
	//initialize maxmod system
	//https://maxmod.devkitpro.org/ref/functions/mmInit.html
	mm_ds_system sys;
	sys.mod_count 			= 0;
	sys.samp_count			= 0;
	sys.mem_bank			= 0;
	sys.fifo_channel		= FIFO_MAXMOD;
	mmInit( &sys );
	//create a stream
	//https://maxmod.devkitpro.org/ref/functions/mm_stream.html
	mm_stream mystream;
    mystream.sampling_rate = 44100;
    mystream.buffer_length = 16000;
    mystream.callback = stream;
    mystream.format = MM_STREAM_16BIT_STEREO;
    mystream.timer = MM_TIMER2;
    mystream.manual = 1;
    mmStreamOpen(&mystream);
}
//this fills mm_stream mystream buffer
//https://maxmod.devkitpro.org/ref/functions/mm_stream_func.html
mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format) {
	if (file) {
		int res = fread(dest, 4, length, file);
		if (res != length) {
			mmStreamClose();
			fclose(file);
			length = res - (res % 4);
		} else {
			length = res;
		}
	}
	return length;
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	iprintf("Hello World!");
	//initialize libfat
	fatInitDefault();
	playWav();
	while(1) {
		//update maxmod stream
		mmStreamUpdate();
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		if(pressed & KEY_START) break;
	}

}
