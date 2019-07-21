#include <NeoPixelBus.h>
#include <Strip.h>

//I tried but have no clue on how to properly extend this
//btw this method only works for d4 pin


// NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> bus(STRIP_SIZE);
// NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> bus(STRIP_SIZE);
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod> bus(STRIP_SIZE);

Strip::Strip(){
  bus.Begin();
  _max_bright = 25;
  frame_index = 0;
}

void Strip::fillRGB(uint8_t r, uint8_t g, uint8_t b){
  bus.ClearTo(RgbColor(r, g, b));
  bus.Show();
}

void Strip::fillHSL(uint8_t h, uint8_t s, uint8_t l){
  bus.ClearTo(RgbColor(HslColor((float) REL_UNIT_BYTE *  h, (float) REL_UNIT_BYTE * s, (float) REL_UNIT_BYTE * l)));
  bus.Show();
}

void Strip::setMaxBrightness(byte b){
  _max_bright = b;
}

//cycle each led to test connections
void Strip::test(){
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
  for (int i = 0; i < STRIP_SIZE; i++ ) {
    bus.SetPixelColor(i, RgbColor(255,0,0));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,255,0));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,0,255));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,0,0));
    bus.Show();
    delay(TEST_DELAY);
    digitalWrite(D7, LOW);
    delay(TEST_DELAY);
    digitalWrite(D7, HIGH);
    delay(TEST_DELAY);
  }
  //clear strip again
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
};

void Strip::setRGBRange(byte r, byte g, byte b, int start, int end) {
  if (start < 0 || end > STRIP_SIZE)
    return;

  for (int i = start; i < end; i++ ) {
    bus.SetPixelColor(i , RgbColor(r,g,b));
  }

  bus.Show();
}

void Strip::setHSLRange(byte h, byte s, byte l, int start, int end) {
  if (start < 0 || end > STRIP_SIZE)
    return;

  HslColor color = HslColor((float) REL_UNIT_BYTE *  h, (float) REL_UNIT_BYTE * s, (float) REL_UNIT_BYTE * l);
  for (int i = start; i < end; i++ ) {
    bus.SetPixelColor(i , RgbColor(color));
  }

  bus.Show();
}

void Strip::clear() {
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
}

void  Strip::_randomize() {
  byte px = 0;
  for (px = 0; px < STRIP_SIZE ; px++) {
    pixels[px].hue = byte(random(100));
    pixels[px].br = byte(random(_max_bright));
    pixels[px].sat = 255;
  }
}

void Strip::_eff_0() { //rainbow ?
  byte n = 0;
  byte hue = 0;
  static byte hue_inc = 0;
  hue_inc++;
  for (n = 0; n < STRIP_SIZE ; n++ ) {
    if (n % 16  == 0){
      hue += 16;
    }

    pixels[n].hue = hue + hue_inc;
    pixels[n].sat = 255;
    pixels[n].br = _max_bright;
  }
}

void Strip::_eff_1() { //rainbow 2
  static byte hue_inc = 0;

  if (frame_index % 2 == 0) {
    hue_inc++;
  }

  byte n = 0;
  for (n = 0; n < STRIP_SIZE ; n++ ) {
    pixels[n].hue = (n * 2)  + hue_inc;
    pixels[n].br = _max_bright;
    pixels[n].sat = 255;
  }
}

void Strip::_eff_2() { //split something
  byte n = 0;
  static byte hue = random(255);

  // if (frame_index % 10 == 0) {
    hue++;
  // }

  for (n = 0; n < STRIP_SIZE ; n++ ) {
    if (n % (STRIP_SIZE / 2) == 0) {
      hue += 64;
    }
    pixels[n].hue == hue;
    pixels[n].sat = 255;
    pixels[n].br = _max_bright;
  }
}


void Strip::_aurora2() {
  static byte h_center = 0;
  static byte br_center = 0;
  static int dir = -1;
  byte n = 0;

  if (frame_index == 0 ) {
    //set the base for the effect
    for (n = 0; n < STRIP_SIZE; n++) {
      pixels[n].br = _max_bright;
      pixels[n].sat = 255;
      pixels[n].hue = h_center + n;
    }
  }

  for (n = 0; n < STRIP_SIZE; n++) {
      pixels[n].hue = h_center + n;
      pixels[n].sat = 255;
      pixels[n].br = (br_center + n) * ((float)_max_bright / STRIP_SIZE);
  }

  if (frame_index % 8 == 0) {
    h_center++;
  }

  if (frame_index % 4 == 0) {
    if (br_center == STRIP_SIZE || br_center == 0) {
      dir = dir * -1;
    }
    br_center += dir;
  }




}

void Strip::_aurora() {
  // static byte h_center = 80;
  // static byte brn1 = 14;
  // static byte brn2 = 15;
  // byte br = 80;
  // byte n = 0 ;
  //
  // if (frame_index == 0 ) {
  //   //set the base for the effect
  //   for (n = 0; n < 30; n ++) {
  //     pixels[n].br = _max_bright;
  //     pixels[n].sat = 255;
  //     pixels[n].hue = (n < 15) ? h_center + (15 - n) * 2: h_center - (n - 15) * 2;
  //   }
  // }
  //
  // for (n = 0; n < 30; n++ ) {
  //   if (n < 15) {
  //     pixels[n].br += (n == brn1)? pixels[n].br + 5: br - ((15 - n) * 2);)
  //     pixels[n].br =
  //     pixels[n].hue = ++h_center;
  //   } else {
  //     pixels[n].br = (n == brn2)? pixels[n].br + 5: br - ((n - 15) * 2);
  //     pixels[n].hue = --h_center;
  //   }
  //   pixels[n].sat = 255;
  // }
  //
  // if (frame_index % 2 == 0) {
  //   brn1 = (brn1 == 0) ? 14 : brn1-1;
  //   brn2 = (brn2 == 29) ? 15 : brn2+1;
  //   //h_center++;
  // }

}

void Strip::_eff_3() {
  static byte hue = 0;

  if (frame_index % 4 == 0) {
    hue++;
  }

  byte n = 0;
  byte br = 0;

    for (n = 0; n < STRIP_SIZE; n++ ) {
    if (n < STRIP_SIZE / 2) {
      pixels[n].hue = hue + 128;
      pixels[n].br = _max_bright;
      pixels[n].sat = 255;
    } else {
      pixels[n].hue = hue;
      pixels[n].br = _max_bright;
      pixels[n].sat = 255;
    }

  }
}

void Strip::_eff_4() { //area efect (hue from 0 + increment on half of the strip)
  byte n = 0;
  static byte h = 0;

  for (n = 0; n < STRIP_SIZE ; n++) {
    if( n < (STRIP_SIZE / 2)) {
      pixels[n].hue = n * (255 / STRIP_SIZE) + h;
    } else {
      pixels[n].hue = (STRIP_SIZE - n -1) * (255 / STRIP_SIZE) +  h;
    }

    pixels[n].br = _max_bright;
    pixels[n].sat = 255;
  }

  if (frame_index % 5 == 0) {
    h++;
  }

}

void Strip::_eff_5() {
  byte n = 0;
  static byte h,b = 0;
  static int inc = -1;


  for (n = 0; n < STRIP_SIZE ; n++) {
    if( n < (STRIP_SIZE / 2)) {
      pixels[n].br = n * (_max_bright / STRIP_SIZE  ) ;
    } else {
      pixels[n].br = (STRIP_SIZE - n -1) * (_max_bright / STRIP_SIZE) ;
    }

    pixels[n].hue = n * (255 / STRIP_SIZE) + h;
    pixels[n].sat = 255;
  }

  if (frame_index % 4 == 0) {

  }

}

void Strip::_eff_6() {
  static byte first_run = 1;
  static byte hue_inc = 0;
  static int dirs[STRIP_SIZE];

  if (first_run == 1) {
    for (int i = 0; i < STRIP_SIZE; i++) {
      pixels[i].br = i % 2 * _max_bright;
    }
    first_run = 0;
  }

  if (frame_index % 4 == 0) {
    hue_inc++;
  }

  for (int n = 0; n < STRIP_SIZE; n++ ) {
    //if (frame_index % 1 == 0) {
    if (pixels[n].br == _max_bright) { //we should start reducing
      dirs[n] = -1;
    } else if (pixels[n].br <= 0) {
      dirs[n] = 1;
    }
    pixels[n].br += dirs[n];
  //}

    pixels[n].hue = (n * 2)  + hue_inc;
    pixels[n].sat = 255;
  }
 }

void Strip::resetFrameCount() {
  frame_index = 0;
}

void Strip::nextFrame(char eff_index) {
  byte n = 0;
  if (eff_index == 0x00) {
    _eff_0();
  } else if (eff_index == 0x01) {
    _eff_1();
  } else if (eff_index == 0x02) {
    _eff_2();
  } else if (eff_index == 0x03) {
    _eff_3();
  } else if (eff_index == 0x04) {
    _eff_4();
  } else if (eff_index == 0x05) {
    _eff_5();
  } else if (eff_index == 0x06) {
    _eff_6();
  } else if (eff_index == 0x07) {
    _aurora2();
  } else if (eff_index == 0x08) {
    _aurora();
  }

  for (n = 0; n < STRIP_SIZE; n++ ) {
    bus.SetPixelColor(n, RgbColor(
      HsbColor( pixels[n].hue * REL_UNIT_BYTE,
                pixels[n].sat * REL_UNIT_BYTE,
                pixels[n].br * REL_UNIT_BYTE)));
  }

  bus.Show();
  frame_index++;
}