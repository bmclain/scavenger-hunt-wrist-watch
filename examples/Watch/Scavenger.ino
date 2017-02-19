const PROGMEM unsigned char letters[45][8] = {
  {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C}, //0
  {0x8, 0x18, 0x28, 0x8, 0x8, 0x8, 0x8, 0x8}, //1
  {0x7E, 0x2, 0x2, 0x7E, 0x40, 0x40, 0x40, 0x7E}, //2
  {0x3E, 0x2, 0x2, 0x3E, 0x2, 0x2, 0x3E, 0x0}, //3
  {0x8, 0x18, 0x28, 0x48, 0xFE, 0x8, 0x8, 0x8}, //4
  {0x3C, 0x20, 0x20, 0x3C, 0x4, 0x4, 0x3C, 0x0}, //5
  {0x3C, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x3C, 0x0}, //6
  {0x3E, 0x22, 0x4, 0x8, 0x8, 0x8, 0x8, 0x8}, //7
  {0x0, 0x3E, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x3E}, //8
  {0x3E, 0x22, 0x22, 0x3E, 0x2, 0x2, 0x2, 0x3E}, //9
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {0x8, 0x14, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x22}, //A
  {0x3C, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x3C, 0x0}, //B
  {0x3C, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3C, 0x0}, //C
  {0x7C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x0}, //D
  {0x7C, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x7C}, //E
  {0x7C, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40}, //F
  {0x3C, 0x40, 0x40, 0x40, 0x40, 0x44, 0x44, 0x3C}, //G
  {0x44, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x44, 0x44}, //H
  {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C}, //I
  {0x3C, 0x8, 0x8, 0x8, 0x8, 0x8, 0x48, 0x30}, //J
  {0x0, 0x24, 0x28, 0x30, 0x20, 0x30, 0x28, 0x24}, //K
  {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7C}, //L
  {0x81, 0xC3, 0xA5, 0x99, 0x81, 0x81, 0x81, 0x81}, //M
  {0x0, 0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x0}, //N
  {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C}, //O
  {0x3C, 0x22, 0x22, 0x22, 0x3C, 0x20, 0x20, 0x20}, //P
  {0x1C, 0x22, 0x22, 0x22, 0x22, 0x26, 0x22, 0x1D}, //Q
  {0x3C, 0x22, 0x22, 0x22, 0x3C, 0x24, 0x22, 0x21}, //R
  {0x0, 0x1E, 0x20, 0x20, 0x3E, 0x2, 0x2, 0x3C}, //S
  {0x0, 0x3E, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8}, //T
  {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1C}, //U
  {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18}, //V
  {0x0, 0x49, 0x49, 0x49, 0x49, 0x2A, 0x1C, 0x0}, //W
  {0x0, 0x41, 0x22, 0x14, 0x8, 0x14, 0x22, 0x41}, //X
  {0x41, 0x22, 0x14, 0x8, 0x8, 0x8, 0x8, 0x8}, //Y
  {0x0, 0x7F, 0x2, 0x4, 0x8, 0x10, 0x20, 0x7F}, //Z
};
void convertCharToBool(unsigned char c, bool b[8])
{
  for (int i = 0; i < 8; ++i) {
    b[i] = (c & (1 << i)) != 0;
  }
}
static int x = 0;
static int frame = 0;
const PROGMEM char string_0[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";   // "String 0" etc are strings to store - change to suit.
const PROGMEM char string_1[] = "GARBAGE";
PGM_P const string_table[] PROGMEM =
{   
  string_0,
  string_1
};

void mode_scavenger(uint8_t action) {
  char message[36];
  strcpy_P(message, (char*)pgm_read_word(&(string_table[0])));
  int messageBits = strlen(message) * 8;
  int messageLen = strlen(message);
  if (action != ACTION_NONE) {
    // Reset our static variable to zero
    x = 0;
    frame = 0;
    watch.fillScreen(0);
    // If we got here either through someone holding left/right or
    // the watch woke up here
    if (action >= ACTION_HOLD_LEFT)
    {
      // Set the depth and plex
      uint8_t depth = 4, plex = LED_PLEX_2;
      // Reconfigure display if needed
      if ((watch.getDepth() != depth) || (watch.getPlex() != plex))
        fps = watch.setDisplayMode(depth, plex, true);
    }
    // Reset sleep timeout on ANY button action
    watch.setTimeout(messageBits * 9);
  }
  watch.fillScreen(0);
  bool marquee[16][8];
  bool letter[8];
  int counter = 0;
  int shifterMin = ((int)(x / 8));
  int shifterMax = ((int)(x / 8));
  if (shifterMax < messageLen) {
    shifterMax++;
  }
  for (int i = shifterMin; i <= shifterMax; i++) {
    for (int j = 0; j < 8; j++) {
      int index = ((int)message[i]) - 48;
      convertCharToBool(pgm_read_word(&letters[index][j]), letter);
      for (int k = 0; k < 8; k++) {
        marquee[counter + k][j] = letter[7 - k];
      }
    }
    counter = counter + 8;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (marquee[i+(x%8)][j] && i+x < messageBits) {
        watch.drawPixel(i, j, 200);
      }
    }
  }  
  if (frame >= 8) {
    if (x < messageBits) {
      x++;
    }
    frame = 0;
  } else {
    frame++;
  }
}


