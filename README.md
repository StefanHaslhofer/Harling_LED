# Harling_LED

## Backgound
A project made to visualize music on LED strips to spice up party locations ;).

## List of Parts
|Part|#|Link|
|---|---|---|
|Arduino UNO R3|1|https://de.banggood.com/UNO-R3-ATmega328P-Development-Board-Geekcreit-for-Arduino-products-that-work-with-official-Arduino-boards-p-963697.html?cur_warehouse=CN|
|SparkFun Spectrum Shield|1|https://www.sparkfun.com/products/13116|
|WS2812B LED strip 5m 30LEDs/m|3|https://www.btf-lighting.com/collections/ws2812b-5v/products/ws2812b-led-pixel-strip-30-60-74-96-100-144-pixels-leds-m?variant=44381280108770|
|Power Distribution Board|2|https://www.amazon.de/gp/product/B0814B5P5M/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1|
|DC5V 30A Switch LED Power Supply Transformers Adapter|1|https://www.btf-lighting.com/collections/5v-power-supply/products/dc5v-transformer?variant=43251378192610|
|220Ohm Resistor|6|https://www.amazon.de/BOJACK-Resistors-Assortment-Kit-Ohm/dp/B08FD1XVL6/ref=sr_1_2_sspa?keywords=220%C2%A0ohm+widerstand&qid=1677441894&sr=8-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1|
|20×1M LED Alu-Rail|1|https://www.amazon.de/LED-Profil-StarlandLed-LED-Aluminium-Montageclips-LED-Streifen-Lichter/dp/B081XTN78C/ref=sr_1_5?keywords=led%2Bchannel&qid=1680954753&sr=8-5&th=1|

## V1.0 - 02.2023
Every 10ms the pitch of an audio input is read in and split into six frequency spectra (reaching from "high" to "low") using a *SparkFun Spectrum Shield*. 
The amplitude of each frequency spectrum is displayed on a distinct 2m LED strip. A higher amplitude results in more LEDs being lit.

The frequency composition gives information about many aspects of the song playing, which can be used to develop some creative features.

For mor information on the *SparkFun Spectrum Shield* see: https://learn.sparkfun.com/tutorials/spectrum-shield-hookup-guide
