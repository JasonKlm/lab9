// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "DAC.h"
#include "Sound.h"
#include "../inc/Timer.h"
#include "Math.h"

//void SysTick_IntArm(uint32_t period, uint32_t priority){
//  // write this
//}

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
const int16_t* sounds[4] = {startup, type, fail, time};
//int soundIndices[polyphonyCount]; // stores the index of each sound that is playing
int sfxIndex;
uint32_t soundSizes[4] = {26791, 1362, 9835, 6836};
uint32_t sound = 0;
uint32_t newSoundIndex = 0;
uint32_t sfxFrequency[4] = {11025, 11025, 11025, 11025};

void Sound_Init(uint32_t period, uint32_t priority){
    SysTick->LOAD = period - 1; // The LOAD value will create an interrupt every (LOAD + 1) * 12.5ns period = 113 for 11khz
    SysTick->CTRL = 7; // The CTRL register must be set to 7 to count SysTick from bus clock, arm the interrupt, and enable SysTick

    SCB->SHP[1] = priority << 30; // Priority is bits 31-30 of SCB->SHP[1]

    DAC_Init(); // Initialize DAC
}


//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(uint32_t period){
  SysTick->LOAD = period - 1; // The LOAD value will create an interrupt every (LOAD + 1) * 12.5ns
  SysTick->VAL = 0; // Begin counting
}
void Sound_Start_FrequencyHz(uint32_t frequency) {
    sfxIndex = 0;
    // frequency in Hz must become period in 12.5ns
    Sound_Start(80000000 / frequency);
}
void Sound_Effect_Start(uint32_t sound) {
    Sound_Start_FrequencyHz(sfxFrequency[sound]);
}
void Sound_Type(void){
sound = 1;
Sound_Effect_Start(sound);
}

void Sound_Failed(void){
sound = 2;
Sound_Effect_Start(sound);
}
void Sound_Timeout(void){
sound = 3;
Sound_Effect_Start(sound);
}
void Sound_Startup(void){
sound = 0;
Sound_Effect_Start(sound);

}

void Sound_Stop(void){
  SysTick->LOAD = 0; // Disarm interrupts by resetting bit 1 of CTRL
}
extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){
  if (sound > 0) { // Buttons play sfx
      DAC_Out(*(sounds[sound] + sfxIndex));
      sfxIndex++;
      if (sfxIndex > soundSizes[sound]) {
          sfxIndex = 0;
          Sound_Stop();
      }
  }
//void SysTick_Handler(void){
//    int outVal = 0;
//    int numSounds = 0;
//    for (int i = 0; i < polyphonyCount; i++) {
//        if (soundIndices[i] >= 0) {
//            numSounds++;
//            outVal += *(sounds[sound] + soundIndices[i]) - 2048;
//            soundIndices[i]++;
//            if (soundIndices[i] > soundSizes[sound]) {
//                soundIndices[i] = -1;
//                numSounds--;
//            }
//        }
//    }
//    if (numSounds != 0)
//        DAC_Out((outVal / numSounds) + 2048);

}
