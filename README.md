# Elektronikus nyomáscsökkentő szelep tesztelő

## Bevezetés: Megvalósítandó funkciók

### 1. Mérési funkciók 
* **Szelepek folytonosságának mérése:** Gyors szakadás- és zárlatvizsgálat a szelepek tekercsén.
* **Nagy pontosságú ellenállásmérés:** A szelepek tekercsellenállásának precíziós mérése **0,1%-os pontossággal**, ami lehetővé teszi a minimális menetzárlatok vagy degradációk kiszűrését is.
* **Többcsatornás architektúra:** A rendszernek alkalmasnak kell lennie egyszerre két szelep fogadására és mérésére/tesztelésére.

### 2. Vezérlési és hajtás funkciók 
* **Teljes kivezérlés:** A szelepek direkt, folyamatos (100%-os) kivezérlése a maximális elmozdulás és működés ellenőrzéséhez.
* **PWM kivezérlés:** Impulzusszélesség-modulált (PWM) jel biztosítása a proporcionális szelepek áram- és nyomásszabályzási karakterisztikájának teszteléséhez.
* **Polaritásváltás (H-híd konfiguráció):** A hardvernek felkészültnek kell lennie a polaritás szoftveres felcserélésére, biztosítva a jövőbeli kompatibilitást a többállású vagy irányváltó szelepekkel.

### 3. Hardver és elektromos védelem 
* **Széles tápfeszültség-tartomány:** Az áramkörnek stabilan kell üzemelnie **12V és 24V DC** közötti betáplálási feszültségen, alkalmazkodva az ipari és járműipari szabványokhoz.
* **Fordított polaritás elleni védelem:** Védelem a tápvezetékek téves felcserélése ellen a belső elektronika megóvása érdekében.
* **Túláramvédelem:** A fő tápágat egy **10A-es biztosíték** védi a zárlatokból vagy túlterhelésekből adódó károk ellen.

### 4. Felhasználói felület és szoftver 
* **Beépített menürendszer:** A műszernek rendelkeznie kell egy lokális kijelzőn és fizikai nyomógombokon keresztül kezelhető, jól strukturált menürendszerrel. A szoftvernek kötelezően biztosítania kell az alábbi dedikált menüpontokat:
  * **Teljes teszt:** Egy automatizált tesztszekvencia, amely felhasználói beavatkozás nélkül, egymás után elvégzi a szakadásvizsgálatot, az ellenállásmérést, és végül teljesen kivezérli a szelepet.
  * **Szakadásvizsgálat:** Külön menüpont a tekercs folytonosságának gyors és célzott ellenőrzésére.
  * **Ellenállásmérés:** Dedikált funkció a szelep tekercsellenállásának precíziós mérésére.
  * **Teljes kivezérlés:** A szelep 100%-os, folyamatos statikus meghajtása az azonnali működés-ellenőrzéshez.
  * **PWM kivezérlés:** Impulzusszélesség-modulált (PWM) jel kiadása a szelep proporcionális mozgásának teszteléséhez.
  * **Beállítások:** Rendszerkonfigurációs menü, ahol a felhasználó megadhatja a tesztelt szelepek számát, illetve kiválaszthatja a kívánt nyelvet.
* **Többnyelvűség:** A szoftver képesnek kell lennie több nyelv (Magyar, Angol, Német) dinamikus kezelésére és tartós tárolására.A műszernek újraindítás után is megkell jegyeznie az elmentett szelepek számát, illetve a beállított nyelvet
