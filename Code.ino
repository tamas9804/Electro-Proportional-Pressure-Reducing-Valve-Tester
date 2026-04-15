#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <avr/pgmspace.h>
#include <String.h>
#include <EEPROM.h>
#include <avr/wdt.h>



#define GoRightButton A2
#define GoLeftButton 2
#define StartButton A1

enum class SystemMode : uint8_t {
  MainMenu,
  Settings,
  Testing
};

enum class MenuStates : uint8_t {
  FullTest,
  ContTest,
  OhmTest,
  FullOutTest,
  PwmTest,
  Settings 
};

enum class SettingStates : uint8_t{
  ValveNumber,
  Language,
  Save
};

enum class AppLanguage : uint8_t {
  HU = 0,
  EN = 1,
  DE = 2
};

enum TextId {
  TXT_APP_TITLE = 0,
  TXT_BOOTING,
  TXT_MENU_FULL_TEST,
  TXT_MENU_CONT_TEST,
  TXT_MENU_OHM_TEST,
  TXT_MENU_FULL_OUT_TEST,
  TXT_MENU_PWM_TEST,
  TXT_MENU_SETTINGS,
  TXT_SET_VALVE_NUM,
  TXT_SET_LANGUAGE,
  TXT_BTN_SAVE,
  TXT_SAVE_SUCCESS,
  TXT_RESTART
};

SystemMode currentMode = SystemMode::MainMenu;

const uint8_t  LanguageArrayNumber = 10;

const char langEN[][17] PROGMEM = {
    "Valve Tester",
    "Booting.",
    "1. FULL TEST",
    "2. CONT. TEST",
    "3. OHM TEST",
    "4. FULL OUT TEST",
    "5. PWM TEST",
    "6. SETTINGS",
    "Valve number: ",
    "Language: ",
    "Save",
    "Save done",
    "Restart"
};

const char langHU[][17] PROGMEM = {
    "Szelep Teszter",
    "Betoltes.",
    "1. Teljes Teszt",
    "2. Folyt. Meres",
    "3. Ell. Meres",
    "4. Kivez. Teszt",
    "5. PWM Teszt",
    "6. Beallitasok",
    "Szelep szam: ",
    "Nyelv: ",
    "Mentes",
    "Mentes Sikeres",
    "Ujrainditas"
};

const char langDE[][17] PROGMEM = {
    "Ventil Tester",    
    "Laden.",         
    "1. Volltest",     
    "2. Fortsetzen",    
    "3. Kontr. Mess.",  
    "4. Ansteuer. T.",  
    "5. PWM Test",      
    "6. Einstellungen", 
    "Ventil Nr.: ",     
    "Sprache: ",
    "Speichern",
    "Gespeichert",
    "Neustart"         
};

char textBuffer[17];

const uint8_t  FirstStartNumber = 78;
const uint8_t  FirstStartNumberAddress = 0;
const uint8_t  ValveAddress = 4;
const uint8_t  LanguageAddress = 6;

const uint8_t  MaxMenuNumber = 5;
const uint8_t  MinMenuNumber = 0;

const uint8_t  MaxSettingNumber = 2;
const uint8_t  MinSettingNumber = 0;

const uint8_t  SaveButtonNumber = 2;

const uint8_t LcdAddress = 0x27;

hd44780_I2Cexp lcd(LcdAddress);

bool SettingIndicator = false;

uint8_t  ValveNumber = 1;
String Language = "HU";

uint8_t  TemporaryValveNumber = ValveNumber;

uint8_t  MaxValveNumber = 2;
uint8_t  MinValveNumber = 1;

AppLanguage LanguageNumber = AppLanguage::HU;
AppLanguage TemporaryLanguageNumber = AppLanguage::HU;
AppLanguage MaxLanguageNumber = AppLanguage::DE;
AppLanguage MinLanguageNumber = AppLanguage::HU;

MenuStates  MenuCurrentState = MenuStates::FullTest;
MenuStates MenuLastState = static_cast<MenuStates>(255);

SettingStates  SettingCurrentState = SettingStates::ValveNumber;
SettingStates  SettingLastState = static_cast<SettingStates>(255);

uint8_t  lastGoRightButtonState = 1;
uint8_t  lastGoLeftButtonState = 1;
uint8_t  lastStartButtonState = 1;


char* getText(TextId id) {
  switch (LanguageNumber) {
    case AppLanguage::HU:
      strcpy_P(textBuffer, langHU[id]);
      break;
    case AppLanguage::EN:
      strcpy_P(textBuffer, langEN[id]);
      break;
    case AppLanguage::DE:
      strcpy_P(textBuffer, langDE[id]);
      break;
  }
  return textBuffer;
}


void lcdBooting()
{
  lcd.begin(16,4);
  lcd.backlight();

  switch (LanguageNumber) {
      case AppLanguage::HU: 
        lcd.setCursor(1, 0); 
        break;
      case AppLanguage::EN: 
        lcd.setCursor(2, 0); 
        break;
      case AppLanguage::DE: 
        lcd.setCursor(1, 0); 
        break;
    }  
  lcd.print(getText(TXT_APP_TITLE));

  switch (LanguageNumber) {
    case AppLanguage::HU: 
      lcd.setCursor(3, 1); 
      break;
    case AppLanguage::EN: 
      lcd.setCursor(4, 1);
      break;
    case AppLanguage::DE: 
      lcd.setCursor(5, 1); 
      break;
  }
  lcd.print(getText(TXT_BOOTING));

  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);

  lcd.clear();
  lcd.print(getText(TXT_MENU_FULL_TEST));
}

void doFirstStart()
{
  int FirstStartReadedCheckNumber;
  EEPROM.get(FirstStartNumberAddress, FirstStartReadedCheckNumber);

  if (FirstStartReadedCheckNumber != FirstStartNumber)
  {
    EEPROM.update(ValveAddress,1);
    EEPROM.update(LanguageAddress,0);
    EEPROM.put(FirstStartNumberAddress,78);
  }
  

}

void setup() {
  Serial.begin(9600);

  pinMode(GoRightButton, INPUT);
  pinMode(GoLeftButton, INPUT);
  pinMode(StartButton, INPUT);

  doFirstStart();
  
  EEPROM.get(ValveAddress, ValveNumber);
  TemporaryValveNumber = ValveNumber;
  Serial.println(TemporaryValveNumber);

  uint8_t buffer;
  EEPROM.get(LanguageAddress, buffer); 
  Serial.println(buffer);

  LanguageNumber = static_cast<AppLanguage>(buffer);
  TemporaryLanguageNumber = static_cast<AppLanguage>(buffer);
  lcdBooting();
}

void DetectRightButton(void)
{
  if(digitalRead(GoRightButton) == 0 && lastGoRightButtonState == 1)
  {
    switch(currentMode)
    {
      case SystemMode::MainMenu:
      { 
        uint8_t nextIndex = static_cast<uint8_t>(MenuCurrentState) + 1;

        if (nextIndex > static_cast<uint8_t>(MenuStates::Settings))
        {
          nextIndex = static_cast<uint8_t>(MenuStates::FullTest);
        }

        MenuCurrentState = static_cast<MenuStates>(nextIndex);
        break;
      } 

      case SystemMode::Settings:
      { 
        uint8_t nextIdx = static_cast<uint8_t>(SettingCurrentState) + 1;
        if (nextIdx > static_cast<uint8_t>(SettingStates::Save)) {
            nextIdx = static_cast<uint8_t>(SettingStates::ValveNumber);
          }
        SettingCurrentState = static_cast<SettingStates>(nextIdx);
        break;
      } 
      
      case SystemMode::Testing:
        break;
    }   
  }

  lastGoRightButtonState = digitalRead(GoRightButton);
}

void DetectLeftButton(void)
{
  if(digitalRead(GoLeftButton) == 0 && lastGoLeftButtonState == 1)
  {
    switch(currentMode)
    {
      case SystemMode::MainMenu:
      { 
        uint8_t prevIndex = static_cast<uint8_t>(MenuCurrentState);
        if (prevIndex == 0)
        {
          prevIndex = static_cast<uint8_t>(MenuStates::Settings);
        }
        else
        {
          prevIndex--;
        }

        MenuCurrentState = static_cast<MenuStates>(prevIndex);
        break;
      } 

      case SystemMode::Settings:
      { 
        uint8_t prevIdx = static_cast<uint8_t>(SettingCurrentState);
        if (prevIdx == 0) {
          prevIdx = static_cast<uint8_t>(SettingStates::Save);
        } else {
          prevIdx--;
        }
        SettingCurrentState = static_cast<SettingStates>(prevIdx);
        break;
      }
    }
  }
  lastGoLeftButtonState = digitalRead(GoLeftButton);
}

void softwareReset() {
  wdt_enable(WDTO_15MS);
  while (1) {

  }
}

void ChangeMenu(void)
{

  if (MenuCurrentState != MenuLastState)
  {
    lcd.clear();

    switch (MenuCurrentState)
    {
      case MenuStates::FullTest:
        lcd.print(getText(TXT_MENU_FULL_TEST));
        break;

      case MenuStates::ContTest:
        lcd.print(getText(TXT_MENU_CONT_TEST));
        break;

      case MenuStates::OhmTest:
        lcd.print(getText(TXT_MENU_OHM_TEST));
        break;

      case MenuStates::FullOutTest:
        lcd.print(getText(TXT_MENU_FULL_OUT_TEST));
        break;

      case MenuStates::PwmTest:
        lcd.print(getText(TXT_MENU_PWM_TEST));
        break;

      case MenuStates::Settings:
        lcd.print(getText(TXT_MENU_SETTINGS));
        break;
    } 
    MenuLastState = MenuCurrentState;
  }
}


void DetectStartButton()
{
  if (digitalRead(StartButton) == 0 && lastStartButtonState == 1)
  { 
    switch(currentMode)
    {
      case SystemMode::MainMenu:
        if (MenuCurrentState == MenuStates::Settings)
        {
          currentMode = SystemMode::Settings;
          SettingCurrentState = SettingStates::ValveNumber; 
          SettingLastState = static_cast<SettingStates>(255);
        }
        else
        {
          currentMode = SystemMode::Testing;
        }
        break; 

      case SystemMode::Settings:
        switch(SettingCurrentState)
        {
          case SettingStates::ValveNumber:
            TemporaryValveNumber++;
            if (TemporaryValveNumber > MaxValveNumber) {
              TemporaryValveNumber = MinValveNumber;
            }
            SettingLastState = static_cast<SettingStates>(255); 
            break;

          case SettingStates::Language:
            {
              uint8_t nextLang = static_cast<uint8_t>(TemporaryLanguageNumber) + 1;
              
              if (nextLang > static_cast<uint8_t>(MaxLanguageNumber)) {
                nextLang = static_cast<uint8_t>(MinLanguageNumber);
              }
              
              TemporaryLanguageNumber = static_cast<AppLanguage>(nextLang);
            }
            break;

          case SettingStates::Save:
            EEPROM.update(ValveAddress, TemporaryValveNumber);
            EEPROM.update(LanguageAddress, static_cast<uint8_t>(TemporaryLanguageNumber));
            
            displaySaveSuccess();
            
            softwareReset(); 
            break;
        }
        break;
        
      case SystemMode::Testing:
        break;
    }
  }
  lastStartButtonState = digitalRead(StartButton);
}

int getColForBlinking()
{
  switch(SettingCurrentState)
  {
    case SettingStates::ValveNumber:
      return strlen(getText(TXT_SET_VALVE_NUM));; // HU: "Szelep szam: "
      break;

    case SettingStates::Language:
      return strlen(getText(TXT_SET_LANGUAGE));
      break;

    case SettingStates::Save:
      return 0;
  }
  return 0;
}

int getRowForBlinking()
{
  switch(SettingCurrentState)
  {
    case SettingStates::ValveNumber: return 1;
    case SettingStates::Language:    return 2;
    case SettingStates::Save:        return 3;
  }
  return 0;
}

String getDataForBlinking()
{
  switch(SettingCurrentState)
  {
    case SettingStates::ValveNumber:
      return String(TemporaryValveNumber);

    case SettingStates::Language:
      switch(TemporaryLanguageNumber)
      {
        case AppLanguage::HU: return "HU";
        case AppLanguage::EN: return "EN";
        case AppLanguage::DE: return "DE";
      }
      break;

    case SettingStates::Save:
      return getText(TXT_BTN_SAVE);
  }
  return "";
}

void displaySaveSuccess()
{
  lcd.clear();
  
  switch(LanguageNumber) {
    case AppLanguage::HU: lcd.setCursor(1,0); break;
    case AppLanguage::EN: lcd.setCursor(3,0); break;
    case AppLanguage::DE: lcd.setCursor(3,0); break;
  }
  lcd.print(getText(TXT_SAVE_SUCCESS));

  switch(LanguageNumber) {
    case AppLanguage::HU: lcd.setCursor(1,1); break;
    case AppLanguage::EN: lcd.setCursor(3,1); break;
    case AppLanguage::DE: lcd.setCursor(3,1); break;
  }
  lcd.print(getText(TXT_RESTART));

  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
}

void blinkText(int col, int row, String text) {
  static unsigned long previousTime = 0;
  static bool isVisible = true;
  
  static int lastCol = -1;
  static int lastRow = -1;
  static String lastText = "";
  
  const long blinkInterval = 500;
  unsigned long currentTime = millis();

  if (col != lastCol || row != lastRow || text != lastText) {
    
    if (lastCol != -1 && lastRow != -1) {
      lcd.setCursor(lastCol, lastRow);
      lcd.print(lastText);
    }

    isVisible = true;          
    previousTime = currentTime; 
    lastCol = col;
    lastRow = row;
    lastText = text;
  } 
  else if (currentTime - previousTime >= blinkInterval) {
    previousTime = currentTime;
    isVisible = !isVisible;
  }

  lcd.setCursor(col, row);
  if (isVisible) {
    lcd.print(text);
  } else {
    int len = text.length();
    for (int i = 0; i < len; i++) {
      lcd.print(" ");
    }
  }
}

void blinkText(int col, int row, int number) {
  blinkText(col, row, String(number));
}

void drawSettingsMenu() {
  if (SettingCurrentState != SettingLastState) {
    lcd.clear();
  
    lcd.print(getText(TXT_MENU_SETTINGS)); 

    lcd.setCursor(0, 1);
    lcd.print(getText(TXT_SET_VALVE_NUM));

    if (SettingCurrentState != SettingStates::ValveNumber) {
      lcd.print(TemporaryValveNumber);
    }


    lcd.setCursor(0, 2);
    lcd.print(getText(TXT_SET_LANGUAGE));

    if (SettingCurrentState != SettingStates::Language) {
      switch(TemporaryLanguageNumber) {
        case AppLanguage::HU: lcd.print("HU"); break;
        case AppLanguage::EN: lcd.print("EN"); break;
        case AppLanguage::DE: lcd.print("DE"); break;
      }
    }

    lcd.setCursor(0, 3);
    if (SettingCurrentState != SettingStates::Save) {
      lcd.print(getText(TXT_BTN_SAVE));
    }

    SettingLastState = SettingCurrentState;
  }

  blinkText(getColForBlinking(), getRowForBlinking(), getDataForBlinking());
}


void loop() {

  DetectRightButton();
  DetectLeftButton();
  DetectStartButton();

  switch(currentMode) {
    case SystemMode::MainMenu:
       ChangeMenu();
       break;

    case SystemMode::Settings:
       drawSettingsMenu();
       break;

    case SystemMode::Testing:
       break;
  }
}

  

