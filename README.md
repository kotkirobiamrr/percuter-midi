# Percuter - MIDI
This module adds MIDI - in functionality to Dynacord Percuter and Dynacord Percuter S drum modules via Multi-trigger input. It can be adapted to use with other electronic drums.

You must set fusebits to use external 20 MHz crystal with no dividing (see: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=EE&HIGH=DF&EXTENDED=FF&LOCKBIT=FF ).

## Parts list:
- 1x ATTiny 2313
- 1x 6N138 transoptor
- 1x 7805 voltage regulator
- 1x 20 MHZ crystal
- 1x 1N4148 diode
- 3x 100 nF ceramic capacitor
- 2x 220 ohm resistor
- 1x 10 Kohm resistor
- 1x tact switch (reset button, not required)
- 1x DIP switch x4 (MIDI channel setting)
- 1x DIN 5 (MIDI socket)
- 1x DB 15 (Multi-trigger output socket)

I assembled it on a small prototyping board, so I did't design the PCB.

## Info & tips

- MIDI channel setting is applied once after reset. If you change channel while power is on, you must reset module.

- Drum channels are assigned on the entire range of MIDI notes, starting from 0, repeating every 8, so you can play drums using any part of your MIDI keyboard.
