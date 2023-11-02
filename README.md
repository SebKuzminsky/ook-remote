# Introduction

This is a transmitter for 433 MHz On-Off Keyed/Amplitude Shift Keyed
remote control signals.

FIXME: stretch goal is to also have it receive and decode signals from
other 433 MHz OOK remotes, so it can clone them.

Built on a Seeed Studios XIAO RP2040 and a cheap-ass 433 MHz OOK
transmitter from Ebay.

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

I then used the Universal Radio Hacker (URH,
<https://github.com/jopohl/urh>) to analyze the recorded signals.
URH needs the filename extensions to be `.complex16s`.


# Other similar projects & info

<https://forums.raspberrypi.com/viewtopic.php?t=58420>
<http://www.hoagieshouse.com/RaspberryPi/RCSockets/RCPlug.html>
