# Localizer PSP — ARK-5 Portu

[English](README.md) | **Türkçe**

## Orijinal Proje Hakkında

**Localizer™: Translate your XMB!**, **Draan** tarafından geliştirilen bir PSP homebrew eklentisidir.
Amacı, PSP'nin resmî olarak desteklemediği dillerdeki XMB temalarını çevirebilmektir.
Localizer ile en sevdiğiniz CTF ve F0 temalarını kullanabilir, CXMB gibi eklentilerle temalar arasında geçiş yapabilir ve XMB'nizi istediğiniz dilde görebilirsiniz.

**PSP Genesis Competition 2011** yarışmasına katılım olarak yayımlanan Localizer, Eklentiler kategorisinde *Wololo'nun Favori Açık Kaynak Yarışma Eseri* olarak anılmıştır.

Bu proje, Draan'ın ortaya koyduğu özgün fikri ve işlevselliği korur.

## Bu Port Hakkında

Orijinal sürüm 6.60 ve PRO LME gibi firmware sürümlerini destekliyordu.
Bu port, projeyi **ARK-5 çalıştıran PSP 6.61 firmware**'ine uyarlar (ARK 5.1.0 ile test edilmiştir).

Orijinal kaynak kod gözden geçirildi, firmware uyumluluğuyla ilgili kısımlar analiz edilip ARK-5 ortamı için Anthropic Claude'un yardımıyla düzenlendi. Uyarlanan sürüm gerçek donanımda test edildi.

Amaç, Localizer'ı 6.61 ARK-5 çalıştıran PSP'lere getirirken projenin özgün işlevini ve vizyonunu korumaktır.

### Orijinale göre değişenler

* **6.61 desteği.** 6.61, devkit sürümünü `0x06060110` olarak bildirir. Orijinal eklenti bunu tanımadığı için hiçbir şeyi yamalamadan sessizce çalışıyordu. Artık 6.60 düzeni kullanılıyor (6.60 ve 6.61'deki `paf.prx` / `sysconf_plugin.prx` yamalanan tüm adreslerde aynıdır).
* **Güvenlik kontrolleri.** Sabit ofsetler (çağrı hedefleri, dışa aktarım girişleri) yama yapılmadan önce doğrulanır. Uyuşmazlık varsa eklenti çökmek yerine firmware'e dokunmaz.
* **ARK-5 çökme düzeltmesi.** ARK-5'te eklenti başlar başlamaz konsol kapanıyordu. Gömülü kullanıcı modülünü eklenti yükleyicisinin bağlamından hemen yüklemek de, öncesinde uzun bir `sceKernelDelayThread` beklemek de çökertiyor. Her adımdan önce kısa bir dosya erişimi (`src/kernel/main.c` içindeki `settle()`) yapmak çalışıyor, kernel modülü bunu yapar. `-DLOC_K_SLEEP` bunun yerine 10 ms bekleme seçer, o da test edildi.
* **`localizer.dat` olmadan çökmez.** Veritabanı yoksa eskiden NULL işaretçisine erişiliyordu.
* **İsteğe bağlı hata ayıklama logları** (`./build.sh debug`) – `ms0:/seplugins/localizer.log` ve `localizer_k.log`. Normal sürüm hiçbir şey yazmaz.
* **Güncel araç zinciriyle derlenir** ([pspdev](https://pspdev.github.io), GCC 15), 2011'in PSPSDK'sı gerekmez.
* **Hakkında görseli döngüsü.** Sistem Bilgisi sayfası tek bir görsel yükler (`flash0:/vsh/resource/name_plate.png`). Sayfa her açıldığında eklenti sıradakini verir: `semihnoir.png` → `genesis.png` → PSP'nin özgün görseli.
* **Türkçe veritabanı** (`strings/strings_tr.txt`, `dist/localizer_tr.dat`).

## Kurulum

Gereksinim: **ARK-5 yüklü 6.61 firmware**'li bir PSP (ARK 5.1.0 ile test edildi).

1. **Dosyaları edinin.** Release zip'ini indirip açın veya bu depodan `dist/localizer.prx` ile bir veritabanını (`dist/localizer_tr.dat`) alın. Türkçe release zip'inde veritabanı zaten `localizer.dat` adıyla gelir, `dist/` içinden alıyorsanız `localizer_tr.dat` dosyasının adını `localizer.dat` yapın.
2. **Memory Stick'e kopyalayın.** `localizer.prx` ve `localizer.dat` dosyalarını `ms0:/seplugins/` klasörüne koyun (PSP Go'da `ef0:/seplugins/`). `seplugins` klasörü yoksa oluşturun. İki dosyanın adı tam olarak böyle kalmalıdır.
3. **Eklentiyi etkinleştirin.** ARK eklenti listesini açın: `ms0:/seplugins/PLUGINS.TXT` (yoksa oluşturun, eski CFW'lerde `vsh.txt`) ve şu satırı ekleyin:
   ```
   vsh, ms0:/seplugins/localizer.prx, on
   ```
   PSP Go'da yolda `ef0:/...` kullanın.
4. **Yeniden başlatın** (ARK-5 ile). XMB menüleri artık Türkçe görünür, Sistem Bilgisi sayfası da yazar görsellerini sırayla gösterir.

`localizer.dat` önce `ef0:/seplugins/`, sonra `ms0:/seplugins/` içinde aranır.

Kaldırmak için satırı `off` yapın ya da silin, iki dosyayı da silin.

**Sorun giderme:** hiçbir şey çevrilmiyorsa dosya adlarını, konumunu ve eklenti satırını kontrol edin. Konsol açılışta kapanıyorsa `localizer.prx` yerine `dist/localizer_debug.prx` kullanın, yeniden başlatın ve oluşan `localizer.log` / `localizer_k.log` dosyalarını gönderin (aşağıdaki Durum bölümüne bakın).

## Derleme

Eklenti (Linux / WSL, [pspdev](https://pspdev.github.io) kurulu):

```bash
./build.sh          # dist/localizer.prx
./build.sh debug    # dist/localizer_debug.prx (log dosyalarını yazar)
```

Dil veritabanı (Windows, Draan'ın `Paxor.exe` aracı; kaynağı `src/Paxor3` içinde):

```bat
tools\make_db.bat tr      :: strings\strings_tr.txt -> dist\localizer_tr.dat
```

## Çeviri yazarken bilinmesi gerekenler

* Metin dosyası BOM'lu UTF-16 LE'dir, her satırda bir `'etiket' = 'metin'` girişi bulunur. `%1`, `%2`, `™`, `®` ve görünmeyen düğme karakterlerini (U+F8xx) olduğu gibi bırakın.
* **Paxor, metni ilk `'` karakterinde bitirir.** Değerin içindeki kesme işareti metni sessizce keser, bu yüzden kullanmayın (Türkçede özel adlardan sonraki ekler bu nedenle kesme işaretsiz yazılmıştır).
* XMB yazı tipinde bazı karakterlerin karşılığı yoktur, ekranda `_` olarak görünürler. Türkçede bu `ğ Ğ ş Ş İ` harflerini etkiler, bu yüzden Türkçe dosyada yerlerine `g G s S I` kullanılır (`ı ö ü ç` sorunsuz çalışır).

## Durum

**6.61 + ARK 5.1.0** üzerinde test edildi: `dist/localizer.prx` (sessiz sürüm) yükleniyor, XMB çevriliyor, yazılar artık kesilmiyor ve Hakkında görseli döngüsü çalışıyor.

Bir sorun olursa `dist/localizer_debug.prx` dosyasını deneyin ve `seplugins` içindeki `localizer.log` / `localizer_k.log` dosyalarına bakın.

### Test edilen konsollar / YAPILACAKLAR

| Model | Durum |
|---|---|
| PSP-3000 | test edildi, çalışıyor |
| PSP-1000 | henüz test edilmedi |
| PSP-2000 | henüz test edilmedi |
| PSP Street (E1000) | henüz test edilmedi |
| PSP Go (N1000) | henüz test edilmedi (eklenti önce `ef0:/seplugins/localizer.dat` yoluna bakar, ancak bu yol gerçek donanımda hiç çalıştırılmadı) |

- [ ] PSP-1000'de test et
- [ ] PSP-2000'de test et
- [ ] PSP Street'te (E1000) test et
- [ ] PSP Go'da (N1000) test et

Bunlardan birinde denediyseniz, lütfen model, firmware, ARK sürümü ve (çöküyorsa) `dist/localizer_debug.prx` ile aldığınız loglarla bir issue açın.

## Çeviriler ve katkılar

Proje, **diğer dillere çeviri için pull request'lere açıktır**. `strings/strings_<dil>.txt` dosyasını ekleyin (`strings/strings_eng.txt` dosyasını kopyalayıp değerleri çevirin, yukarıdaki biçim kurallarına uyun), `tools\make_db.bat <dil>` ile derleyin ve PR gönderin. Eklentinin kendisine yönelik düzeltme ve iyileştirmeler de memnuniyetle karşılanır.

## Katkıda Bulunanlar ve Bağlantılar

* **Orijinal geliştirici:** Draan (Herby)
* **Orijinal proje:** [Localizer PSP — GameBrew](https://www.gamebrew.org/wiki/Localizer_PSP)
* **Uyumluluk portu ve uyarlama:** [SemihNoir](https://github.com/semihnoir)
* **Yapay zekâ destekli geliştirme:** Anthropic Claude (katkıda bulunan)
* Draan'ın kendi teşekkürleri (Bubbletune, Davee, krzychuuu & Erokesha, mint) [`docs/README_original_Draan.txt`](docs/README_original_Draan.txt) dosyasındadır, bu dosya yeniden dağıtımlarda eklenti ile birlikte kalmalıdır.

## Lisans / Sorumluluk Reddi

Bu, orijinal projenin **resmî olmayan** bir uyumluluk portudur. Localizer™ Draan'ın özgün eseridir, eklentinin, fikrin ve geliştirmenin tüm hakkı yaratıcısına aittir.
Orijinal README, bilgilerinin korunması şartıyla yeniden dağıtıma izin verir. Kaynakla birlikte başka bir lisans yayımlanmadığı için burada da eklenmemiştir.
**Kullanım riski size aittir.**

## Depo yapısı

```
assets/    Sistem Bilgisi ekranında gösterilen görseller
dist/      derlenmiş eklenti (normal + debug) ve Türkçe veritabanı
docs/      orijinal README dosyaları
src/       kernel/ user/ (eklenti), Paxor3/ (veritabanı aracı)
strings/   çeviri kaynakları (İngilizce referans, Türkçe, eski Lehçe örnek)
tools/     Paxor.exe ve make_db.bat
build.sh   eklenti derleme betiği
```
