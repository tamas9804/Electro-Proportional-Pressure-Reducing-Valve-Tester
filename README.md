# Elektronikus nyomáscsökkentő szelep tesztelő

## 1. Megvalósítandó funkciók

### 1.1. Mérési funkciók 
* **Szelepek folytonosságának mérése:** Gyors szakadás- és zárlatvizsgálat a szelepek tekercsén.
* **Nagy pontosságú ellenállásmérés:** A szelepek tekercsellenállásának precíziós mérése **0,1%-os pontossággal**, ami lehetővé teszi a minimális menetzárlatok vagy degradációk kiszűrését is.
* **Többcsatornás architektúra:** A rendszernek alkalmasnak kell lennie egyszerre két szelep fogadására és mérésére/tesztelésére.

### 1.2. Vezérlési és hajtás funkciók 
* **Teljes kivezérlés:** A szelepek direkt, folyamatos (100%-os) kivezérlése a maximális elmozdulás és működés ellenőrzéséhez.
* **PWM kivezérlés:** Impulzusszélesség-modulált (PWM) jel biztosítása a proporcionális szelepek áram- és nyomásszabályzási karakterisztikájának teszteléséhez.
* **Polaritásváltás (H-híd konfiguráció):** A hardvernek felkészültnek kell lennie a polaritás szoftveres felcserélésére, biztosítva a jövőbeli kompatibilitást a többállású vagy irányváltó szelepekkel.

### 1.3. Hardver és elektromos védelem 
* **Széles tápfeszültség-tartomány:** Az áramkörnek stabilan kell üzemelnie **12V és 24V DC** közötti betáplálási feszültségen, alkalmazkodva az ipari és járműipari szabványokhoz.
* **Fordított polaritás elleni védelem:** Védelem a tápvezetékek téves felcserélése ellen a belső elektronika megóvása érdekében.
* **Túláramvédelem:** A fő tápágat egy **10A-es biztosíték** védi a zárlatokból vagy túlterhelésekből adódó károk ellen.

### 1.4. Felhasználói felület és szoftver 
* **Beépített menürendszer:** A műszernek rendelkeznie kell egy lokális kijelzőn és fizikai nyomógombokon keresztül kezelhető, jól strukturált menürendszerrel. A szoftvernek kötelezően biztosítania kell az alábbi dedikált menüpontokat:
  * **Teljes teszt:** Egy automatizált tesztszekvencia, amely felhasználói beavatkozás nélkül, egymás után elvégzi a szakadásvizsgálatot, az ellenállásmérést, és végül teljesen kivezérli a szelepet.
  * **Szakadásvizsgálat:** Külön menüpont a tekercs folytonosságának gyors és célzott ellenőrzésére.
  * **Ellenállásmérés:** Dedikált funkció a szelep tekercsellenállásának precíziós mérésére.
  * **Teljes kivezérlés:** A szelep 100%-os, folyamatos statikus meghajtása az azonnali működés-ellenőrzéshez.
  * **PWM kivezérlés:** Impulzusszélesség-modulált (PWM) jel kiadása a szelep proporcionális mozgásának teszteléséhez.
  * **Beállítások:** Rendszerkonfigurációs menü, ahol a felhasználó megadhatja a tesztelt szelepek számát, illetve kiválaszthatja a kívánt nyelvet.
* **Többnyelvűség:** A szoftver képesnek kell lennie több nyelv (Magyar, Angol, Német) dinamikus kezelésére és tartós tárolására.A műszernek újraindítás után is megkell jegyeznie az elmentett szelepek számát, illetve a beállított nyelvet

##  2. Kiválasztott hardverkomponensek 

A projekt áramköri kialakítása során a fő szempont a megbízhatóság, a költséghatékonyság, valamint az ipari (12-24V) környezetnek való megfelelés volt. Az áramkör az alábbi főbb alkatrészekre épül:

### 2.1 Vezérlés és Megjelenítés
* **Arduino Nano (ATmega328P):** Költséghatékony, könnyen programozható, és az I/O lábszáma, valamint a számítási kapacitása tökéletesen elegendő a műszer funkcióinak (állapotgép, I2C kommunikáció, PWM) ellátására.
* **16x4 LCD kijelző (I2C modullal):** A teszteredmények és a menürendszer megjelenítéséért felel. Az I2C bővítőnek köszönhetően mindössze két adatvonalat (SDA, SCL) igényel, így rengeteg portot spórol meg a mikrokontrolleren, és a programozása is rendkívül egyszerű.

### 2.2 Precíziós Mérésadatgyűjtés
* **ADS1115 (16-bites ADC):** A mikrokontroller beépített, alacsonyabb felbontású analóg-digitális átalakítója helyett ez a dedikált IC garantálja a rendkívül pontos (akár 0,1%-os) áram- és feszültségmérést.
* **LM317 feszültségszabályzó (Áramgenerátoros módban):** A precíziós ellenállásmérés alapja. Stabil, állandó mérőáramot biztosít a szelepek tekercsére, ami elengedhetetlen a pontos ellenállás kiszámításához.
* **Feszültségosztó hálózat:** Egy dedikált, egyszerű és gyors mérőáramkör a tekercsek folytonosságának azonnali ellenőrzésére.

