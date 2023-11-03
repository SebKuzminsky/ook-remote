# Introduction

This is a transmitter for 433 MHz On-Off Keyed/Amplitude Shift Keyed
remote control signals.

FIXME: stretch goal is to also have it receive and decode signals from
other 433 MHz OOK remotes, so it can clone them.

Built on a Seeed Studios XIAO RP2040 and a FS1000A (433 MHz ASK
transmitter).

* <https://wiki.seeedstudio.com/XIAO-RP2040/>

* <https://www.ebay.com/itm/204169085885>


# Reverse engineering the original remote's signal

The immediate goal is to clone the remote for a WEN 3410 air filter
<https://wenproducts.com/products/Air-Filtration-System-Item-3410>.

I used a HackRF One (<https://greatscottgadgets.com/hackrf/one/>) and
GQRX to find the frequency of the original remote.

I then used `hackrf_transfer` to record the signals sent by the "on"
and "off" buttons:

Capture:

```
$ hackrf_transfer -s 2000000 -f 433920000 -r on.iq
$ hackrf_transfer -s 2000000 -f 433920000 -r off.iq
```

Replay:

```
$ hackrf_transfer -s 2000000 -f 433920000 -a1 -x24 -t on.iq
$ hackrf_transfer -s 2000000 -f 433920000 -a1 -x24 -t on.iq
$ hackrf_transfer -s 2000000 -f 433920000 -a1 -x24 -t on.iq
$ hackrf_transfer -s 2000000 -f 433920000 -a1 -x24 -t off.iq
```

I used audacity to clean up the recordings:

* File / New

* File / Import / Raw Data...
    * Encoding: Signed 8-bit PCM
    * Channels: 2
    * Sample Rate: 2000000

* Select the waveform

* "Trim audio outside selection" (tool icon looks like `---/\/\/\/---`)

* File / Export / Export Audio...
    * Header: RAW (header-less)
    * Encoding: Signed 8-bit PCM


## Demodulating with Universal Radio Hacker

I used Universal Radio Hacker (URH, <https://github.com/jopohl/urh>) to
analyze the recorded signals.  URH needs the filename extensions to be
`.complex16s`.

This is what the remote sends when I click the "On" button.  We can see
one "header" burst, followed by three similar-looking burst:

!["on" waveform picture](pics/on-waveform.png)

Zooming in on the second burst we can see the On-Off Keying, where the
constant-frequency carrier wave is sometimes transmitted and sometimes
not:

!["on" waveform zoomed picture](pics/on-waveform-zoom.png)

Zooming in more and selecting what looks like a single symbol/bit we
get an approximate symbol size of 621 samples:

![one bit picture](pics/on-one-symbol.png)

I then switched to the demodulated signal view and adjusted the noise
and center to clean up the decoding.  This is starting to look good:

![demodulated picture](pics/on-demodulated.png)

Loading the "off" signal, demodulating it in the same way, and comparing
them.  We see that there's a small difference in the middle of the
data burst:

![on-vs-off picture](pics/on-vs-off.png)


# Other similar projects & info

* <http://www.ignorantofthings.com/2018/11/reverse-engineering-433mhz-remote.html>

* <https://forums.raspberrypi.com/viewtopic.php?t=58420>

* <http://www.hoagieshouse.com/RaspberryPi/RCSockets/RCPlug.html>


## Modulation theory

On-Off Keying: <https://en.wikipedia.org/wiki/On%E2%80%93off_keying>

Amplitude Shift Keying: <https://en.wikipedia.org/wiki/Amplitude-shift_keying>


## Radio tranceiver

* Martyn Davies' excellent explanation of ASK,
  OOK, and the FS1000A/XY-MK-5V transceiver:
  <https://www.youtube.com/watch?v=w6V9NyXwohI>

* <https://theorycircuit.com/fs1000a-433mhz-rf-transmitter-and-receiver-brief-note/>

* <https://components101.com/modules/433-mhz-rf-transmitter-module>

* cc1101 based project: <https://github.com/mcore1976/cc1101-tool>
