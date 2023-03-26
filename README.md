# Harling_LED

## Backgound
A project made to visualize music on LED strips to spice up party locations ;).

## List of Parts
|Part|#|Link|
|---|---|---|
|Arduino UNO R3|1|https://de.banggood.com/UNO-R3-ATmega328P-Development-Board-Geekcreit-for-Arduino-products-that-work-with-official-Arduino-boards-p-963697.html?cur_warehouse=CN|
|SparkFun Spectrum Shield|1|https://www.sparkfun.com/products/13116|
|WS2812B LED strip 5m 30LEDs/m|3|https://www.amazon.de/gp/product/B08C2XD34L/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1|
|3.2-46V to 1.25-35V Step-Down Voltage Regulator|1|https://www.amazon.de/-/en/dp/B07K3W78SQ?psc=1&ref=ppx_yo2ov_dt_b_product_details|
|Power Distribution Board|2|https://www.amazon.de/gp/product/B0814B5P5M/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1|
|DC5V 30A Switch LED Power Supply Transformers Adapter|1|https://www.btf-lighting.com/collections/5v-power-supply/products/dc5v-transformer?variant=43251378192610|
|220Ohm Resistor|6|https://www.amazon.de/BOJACK-Resistors-Assortment-Kit-Ohm/dp/B08FD1XVL6/ref=sr_1_2_sspa?keywords=220%C2%A0ohm+widerstand&qid=1677441894&sr=8-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1|

## V1.0 - 02.2023
Every 10ms the pitch of an audio input is read in and split into six frequency spectra (reaching from "high" to "low") using a *SparkFun Spectrum Shield*. 
The amplitude of each frequency spectrum is displayed on a distinct 2m LED strip. A higher amplitude results in more LEDs being lit.

The frequency composition gives information about many aspects of the song playing, which can be used to develop some creative features.
For example, the program recognizes a sudden surge in lower frequencies as drop.

For mor information on the *SparkFun Spectrum Shield* see: https://learn.sparkfun.com/tutorials/spectrum-shield-hookup-guide
