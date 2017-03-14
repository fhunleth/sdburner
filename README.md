# Raspberry Pi SDCard Programmer

The project contains the GUI and programming logic for the Raspberry Pi SDCard
programmer that we use at [CoderDojoDC](http://coderdojodc.com). Students and
mentors use it to program or reset the MicroSD cards that are used in our
Raspberry Pi workstations. Through some pain, we found that programming SDCards
using laptops was harder and more time consuming than it looked even with tools
like Etcher.io.

Here's Alexa explaining how to use the programmer (she helped make it):

[![Demo video](https://img.youtube.com/vi/PTbPzjv045U/0.jpg)](https://www.youtube.com/watch?v=PTbPzjv045U)

To build one yourself, you'll need the following:

1. Raspberry Pi 2 (It should work on the other Raspberry Pi's too, but the
   Buildroot project will need to be updated.)
2. Adafruit [PiTFT 2.2" HAT w/ buttons](https://www.adafruit.com/products/2315)
3. USB card reader
4. Standoffs, other hardware to put keep it together

The software side is not that easy to build right now, but if enough people are
interested, we'll write up instructions. In the end, it's just copying an image
to a MicroSD card and putting it in the Raspberry Pi 2, but for now you'll need
to build it yourself. The image builder can be found at
[fhunleth-buildroot-experiments](https://github.com/fhunleth/fhunleth-buildroot-experiments)
using the `rpi2_sdburner_defconfig` configuration. Buildroot knowledge is
probably required... Sorry. :(

The program uses [fwup](https://github.com/fhunleth/fwup) to write the image to
the SDCard. This simplified a few things in the program, but a step to creating
the software image that gets copied to the MicroSD card. We use an image from
Dexter Industries for our Raspberry Pis. It is really just Raspbian with a few
additional programs installed by default. `fwup` compresses the image so that it
doesn't take much space up on the MicroSD card being used for the programmer and
it also has nice logic for detecting the SDCard, reporting progress and errors,
and has some optimizations for programming the cards quickly (well, quickly for
how large they are).

The `fwup` programmer needs a `.fw` file. To make it, create a `fwup`
configuration file that looks like this:

```
file-resource disk.img {
        host-path = "2017.01.22_Dexter_Industries_jessie.img"
}

task complete {
    on-resource disk.img { raw_write(0) }
}
```

Replace `2017.01.22_Dexter_Industries_jessie.img` with the name of the image
that you want programmed. Then run `fwup`:

```
fwup -c -f myconf.conf -o 2017.01.22_Dexter_Industries_jessie.fw
```
Again, replace the files above with the ones that you're using. The `.fw` file
goes in the boot (FAT32) partition of the MicroSD card image created by
Buildroot. It's currently hardcoded in the Buildroot configuration, so you'll
get an error when it gets to that point. If you rename your file to fix the
error, it will work (albeit in an ugly way).

